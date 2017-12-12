%{
#include <stdio.h>
#include <stdlib.h>

extern int linenum;             /* declared in lex.l */
extern FILE *yyin;              /* declared by lex */
extern char *yytext;            /* declared by lex */
extern char buf[256];           /* declared in lex.l */
%}

%token SEMICOLON MOD KWBEGIN KWEND IDENT KWARRAY IDENTEQUAL LS LE SE NOT AND OR KWBOOLEAN KWDEF KWDO KWELSE KWFALSE KWFOR KWINTEGER KWIF KWOF KWPRINT KWREAD KWREAL KWSTRING KWTHEN KWTO KWTRUE KWRETURN KWVAR KWWHILE INTEGER OCTAL FLOAT SCIENTIFIC STRING

%left  LS LE SE NOT AND OR '-' '+' '<' '=' 

%start program

%%

program		: programname SEMICOLON programbody compound KWEND IDENT
		;

programname	: identifier
		;

programbody	: /* empty */
			| program_inside
			| programbody program_inside
			;

program_inside	: variable
			   	| function
				;



identifier	: IDENT
		;

function	: functionname '(' functionarguments ')' functionbody SEMICOLON
		 	| functionname '(' functionarguments ')' functionbody SEMICOLON compound KWEND IDENT
		 ;

functionname	: IDENT
			 ;

functionbody	: /* empty */
			 	| ':' type 
				;

type	: KWARRAY
	 	| KWBOOLEAN
		| KWINTEGER
		| KWSTRING
		| KWREAL
		| KWARRAY integer_constant KWTO integer_constant KWOF type 
		; 

type_num	: KWTRUE
		 	| KWFALSE
			| INTEGER
			| STRING
			| FLOAT
			| SCIENTIFIC
		; 


functionarguments	: /* empty */
				  	| functionargument
				  	| functionargument SEMICOLON functionarguments
				;

functionargument	: identifier_list ':' type
				 ;

identifier_list	: identifier
				| identifier ',' identifier_list
				;

variable		: KWVAR identifier_list ':' type SEMICOLON
		  		| KWVAR identifier_list ':' type_num SEMICOLON
	 			| KWVAR identifier_list ':' SEMICOLON
	 	;

integer_constant	: INTEGER
				 ;

literal_constant	: INTEGER
				 	| KWREAL
				 	| OCTAL
					| FLOAT
					| SCIENTIFIC
				 	| STRING
					| KWTRUE
					| KWFALSE
				;

statement	: function_invocation SEMICOLON
			| compound
		  	| simple
			| conditional
			| while
			| for
			| return
			;

compound	: KWBEGIN variables statements KWEND
		  ;

variables	: variable variables
		  	| variable
			| /* empty */
		;

simple		: variable_reference IDENTEQUAL expression SEMICOLON
			| KWPRINT variable_reference SEMICOLON
			| KWPRINT expression SEMICOLON
			| KWREAD variable_reference SEMICOLON
		;

variable_reference	: IDENT
				   	| array_reference
					| function_invocation
					;

array_reference	: IDENT arrays
				;

arrays	: array
	   	| array arrays
		;

array	: '[' integer_expression ']'
	  ;

integer_expression : integer_expression operator int_exp
				   | int_exp
				   | '-' int_exp
				;

int_exp	: INTEGER
		| IDENT
		| function_invocation
		| array_exp
	;

array_exp	: IDENT int_arr_expressions
		  ;

int_arr_expressions	: '[' integer_expression ']' int_arr_expressions
					| '[' integer_expression ']'
				;

expression : exp
		   | '(' exp ')'
		   | '-' exp
		   | '(' '-' exp ')'
		   | exp operator expression
		   | '(' expression ')'
		;

expr_b : exp
		   | '(' exp ')'
		   | exp ')' 
		   | '-' exp
		   | '(' '-' exp ')'
		   | exp operator_b expr_b
		   | '(' expr_b ')'
		;


exp	: literal_constant
	| IDENT
	| function_invocation
	| array_exp
	;


function_invocation	: IDENT '(' expressions ')'
				;

expressions	: expression ',' expressions
			| expression
			| /* empty */
		;

operator	: '-'
		 	| '*'
			| '/'
			| MOD
			| '+'
			| '<'
			| SE
			| '='
			| LE
			| '>'
			| LS
			| NOT
			| AND
			| OR
		;

operator_b	: '-'
		 	| '*'
			| '/'
			| MOD
			| '+'
		;


conditional	: KWIF boolean_expr KWTHEN statements KWELSE statements KWEND KWIF
			| KWIF boolean_expr KWTHEN statements KWEND KWIF
		;

statements	: /* empty */
		   | statement
		   | statement statements 
		   ;

boolean_expr : expr_b bool_operator expr_b
		;

bool_operator	: '<'
				| SE
				| '='
				| LE
				| '>'
				| LS
				| NOT
				| AND
				| OR
			;

while	: KWWHILE boolean_expr KWDO statements KWEND KWDO
	  ;

for	: KWFOR IDENT IDENTEQUAL integer_constant KWTO integer_constant KWDO statements KWEND KWDO
	;

return	: KWRETURN expression SEMICOLON
	   ;

%%

int yyerror( char *msg )
{
		fprintf( stderr, "\n|--------------------------------------------------------------------------\n" );
		fprintf( stderr, "| Error found in Line #%d: %s\n", linenum, buf );
		fprintf( stderr, "|\n" );
		fprintf( stderr, "| Unmatched token: %s\n", yytext );
		fprintf( stderr, "|--------------------------------------------------------------------------\n" );
		exit(-1);
}

int  main( int argc, char **argv ) {
	if( argc != 2 ) {
		fprintf(  stdout,  "Usage:  ./parser  [filename]\n"  );
		exit(0);
	}

	FILE *fp = fopen( argv[1], "r" );

	if( fp == NULL )  {
		fprintf( stdout, "Open  file  error\n" );
		exit(-1);
	}

	yyin = fp;
	yyparse();

	fprintf( stdout, "\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	fprintf( stdout, "|  There is no syntactic error!  |\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	exit(0);
}

