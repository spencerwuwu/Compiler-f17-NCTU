%{
/**
 * Introduction to Compiler Design by Prof. Yi Ping You
 * Project 3 YACC sample
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>


#include "header.h"
#include "symtab.h"
#include "semcheck.h"
#include "codegen.h"

int yydebug;

extern int linenum;     /* declared in lex.l */
extern FILE *yyin;      /* declared by lex */
extern char *yytext;    /* declared by lex */
extern char buf[256];   /* declared in lex.l */
extern int yylex(void);
int yyerror(char*);

int scope = 0;
int var_tmp = 0;
int var_store = 0;
int Opt_D = 1;                  // symbol table dump option
int wrt_fp = 0;
__BOOLEAN func_decl;
char wrt_name[256];
char fileName[256];             // filename of input file
struct SymTable *symbolTable;	// main symbol table
struct LabelTable *labelTable;
__BOOLEAN paramError;			// indicate is parameter have any error?
struct PType *funcReturn;		// record return type of function, used at 'return statement' production rule
%}

%union {
	int intVal;
	float realVal;
	//__BOOLEAN booleanVal;
	char *lexeme;
	struct idNode_sem *id;
	//SEMTYPE type;
	struct ConstAttr *constVal;
	struct PType *ptype;
	struct param_sem *par;
	struct expr_sem *exprs;
	/*struct var_ref_sem *varRef; */
	struct expr_sem_node *exprNode;
};

/* tokens */
%token ARRAY BEG BOOLEAN DEF DO ELSE END FALSE FOR INTEGER IF OF PRINT READ REAL RETURN STRING THEN TO TRUE VAR WHILE
%token OP_ADD OP_SUB OP_MUL OP_DIV OP_MOD OP_ASSIGN OP_EQ OP_NE OP_GT OP_LT OP_GE OP_LE OP_AND OP_OR OP_NOT
%token MK_COMMA MK_COLON MK_SEMICOLON MK_LPAREN MK_RPAREN MK_LB MK_RB

%token <lexeme>ID
%token <intVal>INT_CONST 
%token <realVal>FLOAT_CONST
%token <realVal>SCIENTIFIC
%token <lexeme>STR_CONST

%type<id> id_list
%type<constVal> literal_const
%type<ptype> type scalar_type array_type opt_type
%type<par> param param_list opt_param_list
%type<exprs> var_ref boolean_expr boolean_term boolean_factor relop_expr expr term factor boolean_expr_list opt_boolean_expr_list
%type<intVal> dim mul_op add_op rel_op array_index loop_param

/* start symbol */
%start program
%%

program			: ID
			{
			  struct PType *pType = createPType (VOID_t);
			  struct SymNode *newNode = createProgramNode ($1, scope, pType);
			  insertTab (symbolTable, newNode);

			  if (strcmp(fileName, $1)) {
				fprintf (stdout, "<Error> found in Line %d: program beginning ID inconsist with file name\n", linenum);
			  }
			  fileName_to_codegenName();

				// find file
				DIR *dp;
				struct dirent *dirp;
				if( (dp = opendir(".")) != NULL ) {
					while( (dirp=readdir(dp)) != NULL ) {
						if( strcmp( dirp->d_name, wrt_name )==0 ) {
							remove( wrt_name );
							break;
						}
					}
				}

			  wrt_fp = open( wrt_name,  O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR );
			  if( wrt_fp < 0 ) {
				fprintf(stdout, "write  file  error\n");
				exit(1);
			  }

			  init_codegen();
			}
			  MK_SEMICOLON
			  program_body
			  END ID
			{
			  if (strcmp($1, $6)) {
                  fprintf (stdout, "<Error> found in Line %d: program end ID inconsist with the beginning ID\n", linenum);
              }
			  if (strcmp(fileName, $6)) {
				  fprintf (stdout, "<Error> found in Line %d: program end ID inconsist with file name\n", linenum);
			  }
			  // dump symbol table
			  if( Opt_D == 1 )
				printSymTable( symbolTable, scope );
        	  close(wrt_fp);
			}
			;

program_body		: opt_decl_list opt_func_decl_list compound_stmt
			;

opt_decl_list		: decl_list
			| /* epsilon */
			;

decl_list		: decl_list decl
			| decl
			;

decl			: VAR id_list MK_COLON scalar_type MK_SEMICOLON       /* scalar type declaration */
			{
			  // insert into symbol table
			  struct idNode_sem *ptr;
			  struct SymNode *newNode;
			  for( ptr=$2 ; ptr!=0; ptr=(ptr->next) ) {
			  	if( verifyRedeclaration(symbolTable, ptr->value, scope) == __TRUE ) {
					newNode = createVarNode (ptr->value, scope, $4);
					insertTab( symbolTable, newNode);
					if( scope != 0 ) {
						var_tmp++;
					}
					newNode->local_num = var_tmp;
			  		variable_decl( scope, ptr->value, $4, newNode );
				}
			  }
			  
			  deleteIdList( $2 );
			}
			| VAR id_list MK_COLON array_type MK_SEMICOLON        /* array type declaration */
			{
			  verifyArrayType( $2, $4 );
			  // insert into symbol table
			  struct idNode_sem *ptr;
			  struct SymNode *newNode;
			  for( ptr=$2 ; ptr!=0 ; ptr=(ptr->next) ) {
			  	if( $4->isError == __TRUE ) { }
				else if( verifyRedeclaration( symbolTable, ptr->value, scope ) ==__FALSE ) { }
				else {
					newNode = createVarNode( ptr->value, scope, $4 );
					insertTab( symbolTable, newNode );
				}
			  }
			  
			  deleteIdList( $2 );
			}
			| VAR id_list MK_COLON literal_const MK_SEMICOLON     /* const declaration */
			{
			  struct PType *pType = createPType( $4->category );
			  // insert constants into symbol table
			  struct idNode_sem *ptr;
			  struct SymNode *newNode;
			  for( ptr=$2 ; ptr!=0 ; ptr=(ptr->next) ) {
			  	if( verifyRedeclaration( symbolTable, ptr->value, scope ) ==__FALSE ) { }
				else {
					newNode = createConstNode( ptr->value, scope, pType, $4 );
					insertTab( symbolTable, newNode );
				}
			  }
			  
			  deleteIdList( $2 );
			}
			;

literal_const		: INT_CONST
			{
			  int tmp = $1;
			  $$ = createConstAttr( INTEGER_t, &tmp );
			}
			| OP_SUB INT_CONST
			{
			  int tmp = -$2;
			  $$ = createConstAttr( INTEGER_t, &tmp );
			}
			| FLOAT_CONST
			{
			  float tmp = $1;
			  $$ = createConstAttr( REAL_t, &tmp );
			}
			| OP_SUB FLOAT_CONST
			{
			  float tmp = -$2;
			  $$ = createConstAttr( REAL_t, &tmp );
			}
			| SCIENTIFIC 
			{
			  float tmp = $1;
			  $$ = createConstAttr( REAL_t, &tmp );
			}
			| OP_SUB SCIENTIFIC
			{
			  float tmp = -$2;
			  $$ = createConstAttr( REAL_t, &tmp );
			}
			| STR_CONST
			{
			  $$ = createConstAttr( STRING_t, $1 );
			}
			| TRUE
			{
			  __BOOLEAN tmp = __TRUE;
			  $$ = createConstAttr( BOOLEAN_t, &tmp );
			}
			| FALSE
			{
			  __BOOLEAN tmp = __FALSE;
			  $$ = createConstAttr( BOOLEAN_t, &tmp );
			}
			;

opt_func_decl_list	: func_decl_list
			| /* epsilon */
			;

func_decl_list		: func_decl_list func_decl
			| func_decl
			;

func_decl		: ID MK_LPAREN opt_param_list
			{
			  // check and insert parameters into symbol table
			  paramError = insertParamIntoSymTable( symbolTable, $3, scope+1 );
			}
			  MK_RPAREN opt_type 
			{
			  // check and insert function into symbol table
			  if( paramError == __TRUE ) {
			  	printf("<Error> found in Line %d: param(s) with several error\n", linenum);
			  } else if( $6->isArray == __TRUE ) {
					
					printf("<Error> found in Line %d: a function cannot return an array type\n", linenum);
				} else {
					
				insertFuncIntoSymTable( symbolTable, $1, $3, $6, scope );
			  }
			  funcReturn = $6;
			  function_heading_codegen( $1, $3, $6 );
			  func_decl = __TRUE;
			}
			  MK_SEMICOLON
			  compound_stmt
			  END ID
			{
			  if( strcmp($1,$11) ) {
				fprintf( stdout, "<Error> found in Line %d: the end of the functionName mismatch\n", linenum );
			  }
			  funcReturn = 0;
			  function_ending_codegen();
			  func_decl = __FALSE;
			}
			;

opt_param_list		: param_list { $$ = $1; }
			| /* epsilon */ { $$ = 0; }
			;

param_list		: param_list MK_SEMICOLON param
			{
			  param_sem_addParam( $1, $3 );
			  $$ = $1;
			}
			| param { $$ = $1; }
			;

param			: id_list MK_COLON type { $$ = createParam( $1, $3 ); }
			;

id_list			: id_list MK_COMMA ID
			{
			  idlist_addNode( $1, $3 );
			  $$ = $1;
			}
			| ID { $$ = createIdList($1); }
			;

opt_type		: MK_COLON type { $$ = $2; }
			| /* epsilon */ { $$ = createPType( VOID_t ); }
			;

type			: scalar_type { $$ = $1; }
			| array_type { $$ = $1; }
			;

scalar_type		: INTEGER { $$ = createPType (INTEGER_t); }
			| REAL { $$ = createPType (REAL_t); }
			| BOOLEAN { $$ = createPType (BOOLEAN_t); }
			| STRING { $$ = createPType (STRING_t); }
			;

array_type		: ARRAY array_index TO array_index OF type
			{
				verifyArrayDim ($6, $2, $4);
				increaseArrayDim ($6, $2, $4);
				$$ = $6;
			}
			;

array_index		: INT_CONST { $$ = $1; }
			;

stmt			: compound_stmt
			| simple_stmt
			| cond_stmt
			| while_stmt
			| for_stmt
			| return_stmt
			| proc_call_stmt
			;

compound_stmt		: 
			{ 
			  if( scope == 0 && !func_decl ) {
				char tmp[300];
				sprintf( tmp, ".method public static main([Ljava/lang/String;)V\n\t.limit stack 100\n\t.limit local 100\n");
				write( wrt_fp, tmp, strlen(tmp) );
			  }
			  scope++;
			  var_store = var_tmp;
			  enter_block( var_tmp );
			}
			  BEG
			  opt_decl_list
			  opt_stmt_list
			  END 
			{ 
			  // print contents of current scope
			  if( Opt_D == 1 )
			  	printSymTable( symbolTable, scope );

			  leave_block( symbolTable, scope );
			  deleteScope( symbolTable, scope );	// leave this scope, delete...
			  scope--; 
			  if( scope == 0 && !func_decl ) {
				char tmp[300];
				sprintf( tmp, "\treturn\n.end method\n");
				write( wrt_fp, tmp, strlen(tmp) );
			  }
			  var_tmp = var_store;
			}
			;

opt_stmt_list		: stmt_list
			| /* epsilon */
			;

stmt_list		: stmt_list stmt
			| stmt
			;

simple_stmt		: var_ref OP_ASSIGN boolean_expr MK_SEMICOLON
			{
			  // check if LHS exists
			  __BOOLEAN flagLHS = verifyExistence( symbolTable, $1, scope, __TRUE );
			  // id RHS is not dereferenced, check and deference
			  __BOOLEAN flagRHS = __TRUE;
			  if( $3->isDeref == __FALSE ) {
				flagRHS = verifyExistence( symbolTable, $3, scope, __FALSE );
			  }
			  // if both LHS and RHS are exists, verify their type
			  if( flagLHS==__TRUE && flagRHS==__TRUE )
				verifyAssignmentTypeMatch( $1, $3 );
			  
			  assign_codegen( symbolTable, $1, $3, scope );
			}
			| PRINT boolean_expr MK_SEMICOLON { 
				verifyScalarExpr( $2, "print" ); 
			    print_codegen( symbolTable, $2, scope );
			}
 			| READ boolean_expr MK_SEMICOLON { 
				verifyScalarExpr( $2, "read" ); 
			}
			;

proc_call_stmt		: ID MK_LPAREN opt_boolean_expr_list MK_RPAREN MK_SEMICOLON
			{
			  verifyFuncInvoke( $1, $3, symbolTable, scope );
			}
			;

cond_stmt	: IF_header
			  IF_else
			  END IF
			  {
				removeLabel( labelTable );
			  }
			;

IF_header	: IF_ condition THEN 
			  opt_stmt_list
		  {
			if_ending_codegen( labelTable );
		  }
		  ;

IF_		: IF
		  {
		  	struct LabelNode *node = createLabel ( linenum, "if", __FALSE );
			insertLabel( labelTable, node );
			if_heading_codegen( labelTable );
		  }
		;

IF_else	:	ELSE_
			opt_stmt_list
		|
		;
ELSE_	:	ELSE
	  	{
			else_codegen( labelTable );
		}
		;

condition		: boolean_expr { verifyBooleanExpr( $1, "if" ); } 
			;

while_stmt		: WHILE condition_while DO
			  opt_stmt_list
			  END DO
			;

condition_while		: boolean_expr { verifyBooleanExpr( $1, "while" ); } 
			;

for_stmt		: FOR ID 
			{ 
			  insertLoopVarIntoTable( symbolTable, $2 );
			  struct LabelNode *node = createLabel ( linenum, "for", __FALSE );
			  insertLabel( labelTable, node );
			}
			  OP_ASSIGN loop_param TO loop_param
			{
			  verifyLoopParam( $5, $7 );
			  for_heading_codegen( labelTable, $5, $7 );
			}
			  DO
			  opt_stmt_list
			  END DO
			{
			  popLoopVar( symbolTable );
			  for_ending_codegen( labelTable );
			  removeLabel( labelTable );
			}
			;

loop_param		: INT_CONST 
				{ 
					$$ = $1; 
				}
			| OP_SUB INT_CONST 
			{ 
				$$ = -$2; 
			}
			;

return_stmt		: RETURN boolean_expr MK_SEMICOLON
			{
			  verifyReturnStatement( $2, funcReturn );
			  return_codegen( $2, funcReturn );
			}
			;

opt_boolean_expr_list	: boolean_expr_list { $$ = $1; }
			| /* epsilon */ { $$ = 0; }	// null
			;

boolean_expr_list	: boolean_expr_list MK_COMMA boolean_expr
			{
			  struct expr_sem *exprPtr;
			  for( exprPtr=$1 ; (exprPtr->next)!=0 ; exprPtr=(exprPtr->next) );
			  exprPtr->next = $3;
			  $$ = $1;
			}
			| boolean_expr
			{
			  $$ = $1;
			}
			;

boolean_expr		: boolean_expr OP_OR boolean_term
			{
			  verifyAndOrOp( $1, OR_t, $3 );
			  $$ = $1;
		  	  struct LabelNode *node = createLabel ( linenum, "", __TRUE );
			  rel_op_codegen( symbolTable, $1, $3, OR_t, scope, node );
			  free( node );
			}
			| boolean_term { $$ = $1; }
			;

boolean_term		: boolean_term OP_AND boolean_factor
			{
			  verifyAndOrOp( $1, AND_t, $3 );
			  $$ = $1;
		  	  struct LabelNode *node = createLabel ( linenum, "", __TRUE );
			  rel_op_codegen( symbolTable, $1, $3, AND_t, scope, node );
			  free( node );
			}
			| boolean_factor { $$ = $1; }
			;

boolean_factor		: OP_NOT boolean_factor 
			{
			  verifyUnaryNOT( $2 );
			  $$ = $2;
			}
			| relop_expr { $$ = $1; }
			;

relop_expr		: expr rel_op expr
			{
			  verifyRelOp( $1, $2, $3 );
			  $$ = $1;
		  	  struct LabelNode *node = createLabel ( linenum, "", __TRUE );
			  rel_op_codegen( symbolTable, $1, $3, $2, scope, node );
			  free( node );
			}
			| expr { $$ = $1; }
			;

rel_op			: OP_LT { $$ = LT_t; }
			| OP_LE { $$ = LE_t; }
			| OP_EQ { $$ = EQ_t; }
			| OP_GE { $$ = GE_t; }
			| OP_GT { $$ = GT_t; }
			| OP_NE { $$ = NE_t; }
			;

expr			: expr add_op term
			{
			  verifyArithmeticOp( $1, $2, $3 );
			  arith_codegen( symbolTable, $1, $3, $2, scope );
			  $$ = $1;
			}
			| term { $$ = $1; }
			;

add_op			: OP_ADD { $$ = ADD_t; }
			| OP_SUB { $$ = SUB_t; }
			;

term			: term mul_op factor
			{
			  if( $2 == MOD_t ) {
				verifyModOp( $1, $3 );
			  }
			  else {
				verifyArithmeticOp( $1, $2, $3 );
			  }
			  $$ = $1;
			  arith_codegen( symbolTable, $1, $3, $2, scope );
			}
			| factor { 
			  $$ = $1; 
			}
			;

mul_op			: OP_MUL { $$ = MUL_t; }
			| OP_DIV { $$ = DIV_t; }
			| OP_MOD { $$ = MOD_t; }
			;

factor			: var_ref
			{
			  verifyExistence( symbolTable, $1, scope, __FALSE );
			  $$ = $1;
			  $$->beginningOp = NONE_t;
			  var_codegen( symbolTable, $$, scope );
			}
			| OP_SUB var_ref
			{
			  if( verifyExistence( symbolTable, $2, scope, __FALSE ) == __TRUE )
				verifyUnaryMinus( $2 );
			  $$ = $2;
			  $$->beginningOp = SUB_t;
			  var_codegen( symbolTable, $$, scope );
			}
			| MK_LPAREN boolean_expr MK_RPAREN 
			{
			  $2->beginningOp = NONE_t;
			  $$ = $2; 
			  var_codegen( symbolTable, $$, scope );
			}
			| OP_SUB MK_LPAREN boolean_expr MK_RPAREN
			{
			  verifyUnaryMinus( $3 );
			  $$ = $3;
			  $$->beginningOp = SUB_t;
			  var_codegen( symbolTable, $$, scope );
			}
			| ID MK_LPAREN opt_boolean_expr_list MK_RPAREN
			{
			  $$ = verifyFuncInvoke( $1, $3, symbolTable, scope );
			  $$->beginningOp = NONE_t;
			  //var_codegen( symbolTable, $$, scope );
			}
			| OP_SUB ID MK_LPAREN opt_boolean_expr_list MK_RPAREN
			{
			  $$ = verifyFuncInvoke( $2, $4, symbolTable, scope );
			  $$->beginningOp = SUB_t;
			  //var_codegen( symbolTable, $$, scope );
			}
			| literal_const
			{
			  $$ = (struct expr_sem *)malloc(sizeof(struct expr_sem));
			  $$->isDeref = __TRUE;
			  $$->varRef = 0;
			  $$->pType = createPType( $1->category );
			  $$->next = 0;
			  if( $1->hasMinus == __TRUE ) {
			  	$$->beginningOp = SUB_t;
			  }
			  else {
				$$->beginningOp = NONE_t;
			  }
			  $$->const_value = $1;
			  var_codegen( symbolTable, $$, scope );
			}
			;

var_ref			: ID
			{
			  $$ = createExprSem( $1 );
			}
			| var_ref dim
			{
			  increaseDim( $1, $2 );
			  $$ = $1;
			}
			;

dim			: MK_LB boolean_expr MK_RB
			{
			  $$ = verifyArrayIndex( $2 );
			}
			;

%%

int yyerror( char *msg )
{
	(void) msg;
	fprintf( stderr, "\n|--------------------------------------------------------------------------\n" );
	fprintf( stderr, "| Error found in Line #%d: %s\n", linenum, buf );
	fprintf( stderr, "|\n" );
	fprintf( stderr, "| Unmatched token: %s\n", yytext );
	fprintf( stderr, "|--------------------------------------------------------------------------\n" );

    if (wrt_fp > 0) {
        close(wrt_fp);
        remove(wrt_name);
    }
	exit(-1);
}

