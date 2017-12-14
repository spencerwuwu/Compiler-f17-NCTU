%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symstruct.h"
#include "symfunc.h"

extern int linenum;		    /* declared in lex.l */
extern FILE *yyin;		    /* declared by lex */
extern char *yytext;	    /* declared by lex */
extern char buf[256];		/* declared in lex.l */
extern int Opt_D;			/* list dump option */
extern int func_scope;
extern int yylex(void);
int yyerror(char* );

char fileName[256];

int func_attr_flag = 0;

Symbol_list *new_symbol_list() {
    Symbol_list *rv = malloc(sizeof(Symbol_list));
    rv->head = 0;
    rv->tail = &rv->head;
    return rv;
};

int current_level = 0;

Scope_list *Scopes_l, *Scopes_s;// Scope of linked list and stack
Scope_node *current_stack; 		// keep trace of the top of the stack
For_it_list *for_list;


// initialization of scope
Scope_node *_new_scope_node() {
    Scope_node *rv = malloc(sizeof(Scope_node));
    rv->next = 0;
	rv->level = current_level;
	rv->Variable_list = new_symbol_list();
    return rv;
};

Scope_node* _copy_scope_node() {
    Scope_node *rv = _new_scope_node();
	rv->level = current_stack->level;
	rv->Variable_list = current_stack->Variable_list;
    rv->next = 0;
    return rv;
};

Scope_list *new_scope_list() {
    Scope_list *rv = malloc(sizeof(Scope_list));
    rv->head = 0;
    rv->tail = &rv->head;
    return rv;
};

void scope_list_push(Scope_node* node) {			
	Scope_node* sp = Scopes_l->head;
	if (sp != 0) { // already initialized
		while (sp->next != 0) {
			sp = sp->next;
		}
		node->prev = sp;
		sp->next = node;
		Scopes_l->tail = &node->next;
	}
	else {
		sp = node;
		sp->next = 0;
		Scopes_s->tail = &sp->next;
	}
}

void scope_stack_init() {
	Scope_node* sp = _new_scope_node();
	Scopes_s->head = sp;
	sp->prev = 0;
	sp->next = 0;
	current_stack = sp;
	Scopes_s->tail = &sp->next;
}

void scope_stack_push() {			
	Scope_node* sp = Scopes_s->head;
	while (sp->next != 0) {
		sp = sp->next;
	}
	Scope_node* node = _new_scope_node();
	node->prev = sp;
	sp->next = node;
	current_stack = node;
	Scopes_s->tail = &node->next;
}

void scope_set_name(char* name) {
	strcpy(current_stack->name, name);
}

void scope_stack_pop() {
	Scope_node* node = _copy_scope_node();
	scope_list_push(node);
	dumpSymbol();

	if (current_stack->prev != 0) {
		current_stack = current_stack->prev;
		current_stack->next = 0;
		Scopes_s->tail = &current_stack->next;
	}
	else {
		current_stack = 0;
	}
}


Id_list *new_id_list() {
    Id_list *rv = malloc(sizeof(Id_list));
    rv->head = 0;
    rv->tail = &rv->head;
    return rv;
};

Id_list *iden_list;

void id_push_back(char* name) {
	Id_node* sp = iden_list->head;
	if (sp != 0) {
		while (sp->next != 0) {
			sp = sp->next;
		}
		Id_node *node = malloc(sizeof(Id_node));
		node->next = 0;
		strcpy(node->name, name);
		sp->next = node;
		iden_list->tail = &node->next;
	}
	else {
		sp = malloc(sizeof(Id_node));
		strcpy(sp->name, name);
		sp->next = 0;
		iden_list->head = sp;
		iden_list->tail = &sp->next;
	}
}

For_it_list *new_for_list() {
    For_it_list *rv = malloc(sizeof(For_it_list));
    rv->head = 0;
    rv->tail = &rv->head;
    return rv;
};

void for_it_push(char* it) {
	For_it_node* sp = for_list->head;
	if (sp == 0) {
		sp = malloc(sizeof(For_it_node));
		strcpy(sp->name, it);
		sp->next = 0;
		sp->prev = 0;
		for_list->head = sp;
		for_list->tail = &sp->next;
	}
	else {
		int break_flag = 0;
		while (sp->next != 0){
			if (!break_flag) {
				if(strcmp(it, sp->name) == 0) {
					char error_msg[400];
					printf( "<Error> found in Line %d: symbol %s is redeclared\n", linenum, it);
					break_flag = 1;
					break;
				}
			}
			sp = sp->next;
		} // push into for whatever conflict or not
		if(strcmp(it, sp->name) == 0 && !break_flag) {
			char error_msg[400];
			printf( "<Error> found in Line %d: symbol %s is redeclared\n", linenum, it);
		}
		For_it_node* node = malloc(sizeof(For_it_node));
		strcpy(node->name, it);
		node->next = 0;
		node->prev = sp;
		sp->next = node;
		for_list->tail = &node->next;
	}
}

void for_it_pop() {
	For_it_node* sp = for_list->head;
	while (sp->next != 0){
		sp = sp->next;
	}
	if (sp->prev != 0) {
		sp->prev->next = 0;
	}
	sp = 0;
}

void symbol_push_back(char* name, char* type, char* kind, char* attr) {
	Symbol_node *sp;
	Symbol_node *node = malloc(sizeof(Symbol_node));
	int break_flag = 0;

	node->next = 0;
	strcpy(node->name, name);
	if (strlen(name) >= 32) {
		node->name[32] = '\0';
	}
	strcpy(node->type, type);
	strcpy(node->kind, kind);
	strcpy(node->attr, attr);
	node->type_no = 0;

	if (func_attr_flag == 1) {
		func_push_attr(type);
	}

	// check whether conflicted with for it
	For_it_node* fp = for_list->head;
	if (fp != 0) {
		while(fp != 0) {
			if (strcmp(fp->name, name) == 0) {
				char error_msg[400];
				printf( "<Error> found in Line %d: symbol %s is redeclared\n", linenum, name);
				break_flag = 1;
				break;
			}
			fp = fp->next;
		}
	}

	sp = current_stack->Variable_list->head;
	if (sp == 0) {
		if (break_flag == 0) {
			current_stack->Variable_list->head = node;	
			current_stack->Variable_list->tail = &node->next;
		}
	}
	else {
		while (sp->next != 0 && !break_flag) {
			if (strcmp(node->name, sp->name) == 0) {
				char error_msg[400];
				printf( "<Error> found in Line %d: symbol %s is redeclared\n", linenum, node->name);
				break_flag = 1;
				break;
			}
			sp = sp->next;
		}
		if (strcmp(node->name, sp->name) == 0 && !break_flag) {
			char error_msg[400];
			printf( "<Error> found in Line %d: symbol %s is redeclared\n", linenum, node->name);
			break_flag = 1;
		}
		if (break_flag == 0) {
			sp->next = node;
			current_stack->Variable_list->tail = &node->next;
		}
	}
}

void symbols_push_back(char* type, char* kind, char* attr) {
    Id_node *node;
    node = iden_list->head;
    while (node != 0) {
        symbol_push_back(node->name, type, kind, attr);
		node = node->next;
    }
	free(iden_list);
}

void symbol_int_push_back(char* name, char* type, char* kind, int attr) {
	Symbol_node *sp;
	Symbol_node *node = malloc(sizeof(Symbol_node));
	node->next = 0;
	strcpy(node->name, name);
	if (strlen(name) >= 32) {
		node->name[32] = '\0';
	}
	strcpy(node->type, type);
	strcpy(node->kind, kind);
	node->attr_int = attr;
	node->type_no = 1;

	sp = current_stack->Variable_list->head;
	if (sp == 0) {
		current_stack->Variable_list->head = node;	
	}
	else {
		int break_flag = 0;
		while (sp->next != 0) {
			sp = sp->next;
			if (strcmp(node->name, sp->name) == 0) {
				char error_msg[400];
				sprintf(error_msg, "symbol %s is redeclared", node->name);
				printf("<Error> found in Line %d: %s\n", linenum, error_msg);
				break_flag = 1;
				break;
			}
		}
		if (break_flag == 0) {
			sp->next = node;
			current_stack->Variable_list->tail = &node->next;
		}
	}
}

void symbols_int_push_back(char* type, char* kind, int attr) {
    Id_node *node;
    node = iden_list->head;
    while (node != 0) {
        symbol_int_push_back(node->name, type, kind, attr);
		node = node->next;
    }
	free(iden_list);
}

void symbol_double_push_back(char* name, char* type, char* kind, double attr) {
	Symbol_node *sp;
	Symbol_node *node = malloc(sizeof(Symbol_node));
	node->next = 0;
	strcpy(node->name, name);
	if (strlen(name) >= 32) {
		node->name[32] = '\0';
	}
	strcpy(node->type, type);
	strcpy(node->kind, kind);
	node->attr_double = attr;
	node->type_no = 2;

	sp = current_stack->Variable_list->head;
	if (sp == 0) {
		current_stack->Variable_list->head = node;	
	}
	else {
		int break_flag = 0;
		while (sp->next != 0) {
			sp = sp->next;
			if (strcmp(node->name, sp->name) == 0) {
				char error_msg[400];
				sprintf(error_msg, "symbol %s is redeclared", node->name);
				printf("<Error> found in Line %d: %s\n", linenum, error_msg);
				break_flag = 1;
				break;
			}
		}
		if (break_flag == 0) {
			sp->next = node;
			current_stack->Variable_list->tail = &node->next;
		}
	}
}

void symbols_double_push_back(char* type, char* kind, double attr) {
    Id_node *node;
    node = iden_list->head;
    while (node != 0) {
        symbol_double_push_back(node->name, type, kind, attr);
		node = node->next;
    }
	free(iden_list);
}

void global_push_back(char* name, char* type, char* kind, char* attr) {
	Symbol_node *sp;
	Symbol_node *node = malloc(sizeof(Symbol_node));
	node->next = 0;
	strcpy(node->name, name);
	strcpy(node->type, type);
	strcpy(node->kind, kind);
	strcpy(node->attr, attr);

	sp = Scopes_s->head->Variable_list->head;
	if (sp == 0) {
		current_stack->Variable_list->head = node;	
	}
	else {
		while (sp->next != 0) {
			sp = sp->next;
		}
		sp->next = node;
	}
	current_stack->Variable_list->tail = &node->next;
}

void print_symbol_list() {
    Symbol_node* local_variable;

	local_variable = current_stack->Variable_list->head;
	while (local_variable != 0) {
		fprintf( stdout, "%-33s", local_variable->name);
		fprintf( stdout, "%-11s", local_variable->kind);
		if (current_stack->level == 0) 
			fprintf( stdout, "%d%-10s", 0, "(global)");
		else 
			fprintf( stdout, "%d%-10s", current_stack->level, "(local)");
		fprintf( stdout, "%-17s", local_variable->type);
		if (local_variable->type_no == 0) {
				if (strlen(local_variable->attr) != 0)
					fprintf( stdout, "%-11s", local_variable->attr);
			}
		else if (local_variable->type_no == 1)
			fprintf( stdout, "%d", local_variable->attr_int);
		else
			fprintf( stdout, "%f", local_variable->attr_double);
		fprintf( stdout, "\n");
		local_variable = local_variable->next;
    }
}

void dumpSymbol () {
	if (Opt_D && !func_scope) {
		int i;
		for(i=0;i< 110;i++)
			fprintf( stdout, "=");
		fprintf( stdout, "\n");
		fprintf( stdout, "%-33s%-11s%-11s%-17s%-11s\n","Name","Kind","Level","Type","Attribute");
		for(i=0;i< 110;i++)
			fprintf( stdout, "-");
		fprintf( stdout, "\n");

		print_symbol_list();

		for(i=0;i< 110;i++)
			fprintf( stdout, "-");
		fprintf( stdout, "\n");
	}
}

// small scale functions

Array_list* arrays;

Array_list* new_array() {
	Array_list* array = malloc(sizeof(Array_list));
	array->head = 0;
	return array;
}

void array_push(int num) {
	Array_node* node = arrays->head;
	if (node != 0) {
		while (node->next != 0) {
			node = node->next;
		}
		Array_node* n_node = malloc(sizeof(Array_node));
		n_node->size = num;
		n_node->prev = node;
		n_node->next = 0;
		node->next = n_node;
	}
	else {
		node = malloc(sizeof(Array_node));
		node->size = num;
		node->next = 0;
		node->prev = 0;
		arrays->head = node;
	}
}

void sort_array(char* place) {
	strcpy(place, arrays->type);
	strcat(place, " ");
	Array_node* node = arrays->head;
	char num[10];
	while (node->next != 0) { // swift to the last of node
		node = node->next;
	}
	while (node != 0) {
		sprintf(num, "[%d]", node->size);
		strcat(place, num);
		node = node->prev;
	}
}

Func_node* current_func;

Func_node* new_func() {
	Func_node* func = malloc(sizeof(Func_node));
	func->attr_flag = 0;
	return func;
}

void func_set_name(char *name) {
	strcpy(current_func->name, name);
	strcpy(current_func->type, "void"); // initial with void
}

void func_set_type(char *name) {
	strcpy(current_func->type, name);
}


void func_push_attr(char* type) {
	if (current_func->attr_flag == 0) {
		strcpy(current_func->attr, type);
		current_func->attr_flag = 1;
	}
	else {
		strcat(current_func->attr, ", ");
		strcat(current_func->attr, type);
	}
}

int string_to_oct(char* input) {
	char c = *input;
	int ans = 0;
	while (c != '\0') {
		ans = ans * 8 + c - '0';
		c = *input++;
	}
	return ans;
}

// p4 new functions
Type_node* look_up_id_type(char *id) {
	Symbol_node *sp;
	sp = current_stack->Variable_list->head;
	Type_node *object;
	object = malloc(sizeof(Type_node));
	int find_flag = 0;
	int break_flag = 0;
	if (sp != 0) {
		while (sp->next != 0 && !break_flag) {
			if (strcmp(id, sp->name) == 0) {
				find_flag = 1;
				break_flag = 1;
				object->emptyness = 0;
				strcpy(object->type, sp->type);
				break;
			}
		}
	}
	if (!break_flag) {
		if (strcmp(id, sp->name) == 0) {
			find_flag = 1;
			object->emptyness = 0;
			strcpy(object->type, sp->type);
		}
	}
	if (!find_flag) {
		object->emptyness = 1;
		strcpy(object->type, "");
	}
	return object;
}

Type_node* choose_type(Type_node *a, Type_node *b) {
	Type_node *ans;
	ans = malloc(sizeof(Type_node));
	int empty_flag = 1;
	if (!a->emptyness && !b->emptyness) {
		if (strcmp(a->type,"real") == 0) {
			if (strcmp(b->type,"string") != 0 && strcmp(b->type,"boolean") != 0) {
				strcpy(ans->type, "real");
				ans->emptyness = 0;
				empty_flag = 0;
			}
		}
		else if (strcmp(a->type,"integer") == 0) {
			if (strcmp(b->type,"string") != 0 && strcmp(b->type,"boolean") != 0) {
				strcpy(ans->type, "real");
				ans->emptyness = 0;
				empty_flag = 0;
			}
		}
		else if (strcmp(a->type,"string") == 0) {
			if (strcmp(b->type,"string") == 0) {
				strcpy(ans->type, "string");
				ans->emptyness = 0;
				empty_flag = 0;
			}
		}
		else if (strcmp(a->type,"boolean") == 0) {
			if (strcmp(b->type,"boolean") == 0) {
				strcpy(ans->type, "boolean");
				ans->emptyness = 0;
				empty_flag = 0;
			}
		}
	}
	if (empty_flag) {
		strcpy(ans->type, "");
		ans->emptyness = 1;
	}
	return ans;
}

// Other global varaibles
char programName[256];
char funcName[256];
char funcType[256];
int funcInner_flag = 0;

char idType[100];
int for_param[2];

%}

%union {
	double double_val;
	int int_val;
	char* str;
	struct symbol_node	*symb;
	struct id_node		*iden; 
	struct array_list	*arr;
	struct type_node    *var_type;
}
/* tokens */
%token ARRAY
%token BEG
%token <str> BOOLEAN
%token DEF
%token DO
%token ELSE
%token END
%token <str> FALSE
%token FOR
%token <str> INTEGER
%token IF
%token OF
%token PRINT
%token READ
%token <str> REAL
%token RETURN
%token <str> STRING
%token THEN
%token TO
%token <str> TRUE
%token VAR
%token WHILE

%token <str> ID
%token <str> OCTAL_CONST
%token <int_val> INT_CONST
%token <double_val> FLOAT_CONST
%token <double_val> SCIENTIFIC
%token <str> STR_CONST

%token OP_ADD
%token OP_SUB
%token OP_MUL
%token OP_DIV
%token OP_MOD
%token OP_ASSIGN
%token OP_EQ
%token OP_NE
%token OP_GT
%token OP_LT
%token OP_GE
%token OP_LE
%token OP_AND
%token OP_OR
%token OP_NOT

%token MK_COMMA
%token MK_COLON
%token MK_SEMICOLON
%token MK_LPAREN
%token MK_RPAREN
%token MK_LB
%token MK_RB

%type <str> opt_type
%type <str> scalar_type
%type <str> sort_array_type
%type <arr> array_type
%type <str> type
%type <str> str_literal_const
%type <str> bool_literal_const
%type <int_val> int_literal_const
%type <int_val> int_const
%type <double_val> d_literal_const
%type <str> decl

%type <var_type> boolean_expr
%type <var_type> boolean_term
%type <var_type> boolean_factor
%type <var_type> factor
%type <var_type> term
%type <var_type> expr
%type <var_type> relop_expr
%type <var_type> var_ref

/* start symbol */
%start program
%%

program			: ID 
		  		{ 
					Scopes_l = new_scope_list();
					Scopes_s = new_scope_list();
					current_level = 0;

					scope_stack_init();
					iden_list = new_id_list();
					for_list = new_for_list();

					global_push_back($1, "void", "program", "");
                    strcpy(programName, $1);
					if (strcmp(programName, fileName) != 0) {
						printf("<Error> found in Line %d: program beginning ID inconsist with file name\n", linenum);
					}
				} 
				MK_SEMICOLON
			  	program_body
			  	END ID
				{	
					current_level--;
					scope_stack_pop();	
					if (strcmp($6, programName) != 0) {
						printf("<Error> found in Line %d: program end ID inconsist with the beginning ID\n", linenum);
					}
					if (strcmp($6, fileName) != 0) {
						printf("<Error> found in Line %d: program beginning ID inconsist with file name\n", linenum);
					}
				}
			;

program_body	: opt_decl_list
			 	opt_func_decl_list 
				compound_stmt
			;

opt_decl_list	: decl_list
				| /* epsilon */
			;

decl_list		: decl_list 
		   		{ 
		   			iden_list = new_id_list();  
					arrays = new_array();
				} 
				decl
				| { 
					iden_list = new_id_list(); 
					arrays = new_array();
				} 
				decl
			;

decl		: VAR 
	  		id_list MK_COLON scalar_type       
	  		/* scalar type declaration */
	  			{
					symbols_push_back($4, "variable", "");
				} MK_SEMICOLON
			| VAR 
			id_list MK_COLON sort_array_type MK_SEMICOLON        
			/* array type declaration */
	  			{ 
					symbols_push_back($4, "variable", "");
				}
			| VAR 
			id_list MK_COLON str_literal_const     
			/* int const declaration */
	  			{ 
					symbols_push_back("string", "constant", $4);
				} MK_SEMICOLON
			| VAR 
			id_list MK_COLON bool_literal_const     
			/* int const declaration */
	  			{ 
					symbols_push_back("boolean", "constant", $4);
				} MK_SEMICOLON
			| VAR 
			id_list MK_COLON int_literal_const MK_SEMICOLON     
			/* int const declaration */
	  			{ 
					symbols_int_push_back("integer", "constant", $4);
				}
			| VAR 
			id_list MK_COLON d_literal_const MK_SEMICOLON     
			/* double const declaration */
	  			{ 
					symbols_double_push_back("real", "constant", $4);
				}
			;

int_const	:	INT_CONST	{ $$ = $1; }
			|	OCTAL_CONST	{ $$ = string_to_oct($1); }
			;

int_literal_const		: int_const { $$ = $1; }
				| OP_SUB int_const { $$ = -$2; }
				;

d_literal_const		:  FLOAT_CONST { $$ = $1; }
				| OP_SUB FLOAT_CONST { $$ = -$2; }
				| SCIENTIFIC { $$ = $1; }
				| OP_SUB SCIENTIFIC { $$ = -$2; }
				;

str_literal_const	: STR_CONST { $$ = $1; }
				;

bool_literal_const	: TRUE { $$ = $1; }
					| FALSE { $$ = $1; }
				;

opt_func_decl_list	: func_decl_list
			| /* epsilon */
			;

func_decl_list		: func_decl_list 
					func_decl
					{
						global_push_back(current_func->name, current_func->type, "function", current_func->attr);
					}
					| func_decl
					{
						global_push_back(current_func->name, current_func->type, "function", current_func->attr);
					}
			;

func_decl	: ID 
			{
				current_level++;
				scope_stack_push();
				current_func = new_func();
				func_set_name($1);
				func_attr_flag = 1;
                funcInner_flag = 1;
                strcpy(funcName, $1);
			}
		  	MK_LPAREN opt_param_list MK_RPAREN opt_type MK_SEMICOLON
			{
				func_attr_flag = 0;
			}
			BEG
			function_body
			END
			{ 
				current_level--;
				scope_stack_pop();
			}
		  	END ID
            {
                funcInner_flag = 0;
                if (strcmp($14, funcName) != 0) {
					printf( "<Error> found in Line %d: function beginning ID inconsist with end ID\n", linenum);
                }
            }
		;

function_body	: opt_decl_list
				opt_stmt_list
				compound_stmt
				|
				opt_decl_list
				opt_stmt_list
				;

opt_param_list		: param_list
				{
					arrays = new_array();
				}
				| /* epsilon */
				{
					arrays = new_array();
				}
			;

param_list		:param_list MK_SEMICOLON 
				{
					iden_list = new_id_list();
					arrays = new_array();
				}
				param
				|
				{
					iden_list = new_id_list();
					arrays = new_array();
				}
				param
			;

param	: 
	  	id_list MK_COLON type 
		{
			symbols_push_back($3, "parameter", "");
		}
	;

id_list		: id_list MK_COMMA ID 
		 	{ 
				id_push_back($3); 
			}
			| ID 
			{ 
				id_push_back($1); 
			}
			;

opt_type	: MK_COLON scalar_type 
		 	{ 
				$$ = $2; 
				func_set_type($2);
                strcpy(funcType, $2);
			}
            | MK_COLON sort_array_type
		 	{ 
				$$ = $2; 
				func_set_type($2);
				printf("<Error> found in Line %d: function declaration can only be a scalar type\n", linenum);
			}
			| /* epsilon */
		;

type		: scalar_type
			| sort_array_type
			;

scalar_type	: INTEGER	{ $$ = $1;}
			| REAL			{ $$ = $1;}
			| BOOLEAN		{ $$ = $1;}
			| STRING		{ $$ = $1;}
		;

sort_array_type	: array_type	{ sort_array($$); }

array_type		: ARRAY int_const TO int_const OF scalar_type
				{
					array_push($4-$2+1);
					strcpy(arrays->type, $6);
				}
				|  ARRAY int_const TO int_const OF array_type
				{
					array_push($4-$2+1);
				}
			;

stmt			: compound_stmt
			| simple_stmt
			| cond_stmt
			| while_stmt
			| for_stmt
			| return_stmt
			| proc_call_stmt
			;

compound_stmt	: BEG
			  	{ 
					current_level++;
					scope_stack_push();
				}
				opt_decl_list
				opt_stmt_list
			  	END
			  	{ 
					current_level--;
					scope_stack_pop();
				}
			;

opt_stmt_list		: stmt_list
			| /* epsilon */
			;

stmt_list		: stmt_list stmt
			| stmt
			;

simple_stmt		:var_ref OP_ASSIGN boolean_expr MK_SEMICOLON
                | PRINT boolean_expr MK_SEMICOLON
                | READ boolean_expr MK_SEMICOLON
			;

proc_call_stmt		: ID MK_LPAREN opt_boolean_expr_list MK_RPAREN MK_SEMICOLON
			;

cond_stmt		: IF boolean_expr THEN
			  opt_stmt_list
			  ELSE
			  opt_stmt_list
			  END IF
			| IF boolean_expr THEN opt_stmt_list END IF
			;

while_stmt		: WHILE boolean_expr DO
			  opt_stmt_list
			  END DO
			;

for_stmt		: FOR ID 
				{
					for_it_push($2);
				}
		  		OP_ASSIGN int_const TO int_const DO 
                {
                    for_param[0] = $5;
                    for_param[1] = $7;
                    if (for_param[0] < 0 || for_param[1] < 0) {
					    printf( "<Error> found in Line %d: The loop parameters used to compute an iteration count must be greater than or equal to zero.\n", linenum);
                    }
                    if (for_param[0] > for_param[1]) {
					    printf( "<Error> found in Line %d: The loop parameters used to compute an iteration count must be in the incremental order.\n", linenum);
                    }
                }
				opt_stmt_list
				END DO
				{
					for_it_pop();
				}
				;

return_stmt		: RETURN boolean_expr MK_SEMICOLON
			;

opt_boolean_expr_list	: boolean_expr_list
			| /* epsilon */
			;

boolean_expr_list	: boolean_expr_list MK_COMMA boolean_expr
			| boolean_expr
			;

boolean_expr	: boolean_expr OP_OR boolean_term
		 		{ $$ = choose_type($1, $3); }
				| boolean_term
				{ $$ = $1; }
			;

boolean_term	: boolean_term OP_AND boolean_factor
		 		{ $$ = choose_type($1, $3); }
				| boolean_factor
				{ $$ = $1; }
			;

boolean_factor	: OP_NOT boolean_factor 
				{ $$ = $2; }
				|  relop_expr
				{ $$ = $1; }
			;

relop_expr		: expr rel_op expr
		 	{ $$ = choose_type($1, $3); }
			|  expr
		 	{ $$ = $1; }
			;

rel_op			: OP_LT
			| OP_LE
			| OP_EQ
			| OP_GE
			| OP_GT
			| OP_NE
			;

expr		: expr add_op term
		 	{ $$ = choose_type($1, $3); }
			| term
		 	{ $$ = $1; }
		;

add_op		: OP_ADD
			| OP_SUB
		;

term		: term mul_op factor
		 	{ $$ = choose_type($1, $3); }
			| factor
		 	{ $$ = $1; }
		;

mul_op			: OP_MUL
			| OP_DIV
			| OP_MOD
			;

factor		: var_ref	
		 	{ $$ = $1; }
			| OP_SUB var_ref	
		 	{ $$ = $2; }
			| MK_LPAREN boolean_expr MK_RPAREN
		 	{ 
				Type_node* tmp; 
				tmp = malloc(sizeof(Type_node));
				tmp->emptyness = 0;
				strcpy(tmp->type, "string");
				$$ = tmp;
			}
			| OP_SUB MK_LPAREN boolean_expr MK_RPAREN
		 	{ 
				Type_node* tmp; 
				tmp = malloc(sizeof(Type_node));
				tmp->emptyness = 0;
				strcpy(tmp->type, "string");
				$$ = tmp;
			}
			| ID MK_LPAREN opt_boolean_expr_list MK_RPAREN
		 	{ $$ = look_up_id_type($1); }
			| OP_SUB ID MK_LPAREN opt_boolean_expr_list MK_RPAREN
		 	{ $$ = look_up_id_type($2); }
			| int_literal_const
		 	{ 
				Type_node* tmp; 
				tmp = malloc(sizeof(Type_node));
				tmp->emptyness = 0;
				strcpy(tmp->type, "string");
				$$ = tmp;
			}
			| d_literal_const
		 	{ 
				Type_node* tmp; 
				tmp = malloc(sizeof(Type_node));
				tmp->emptyness = 0;
				strcpy(tmp->type, "string");
				$$ = tmp;
			}
			| str_literal_const
		 	{ 
				Type_node* tmp; 
				tmp = malloc(sizeof(Type_node));
				tmp->emptyness = 0;
				strcpy(tmp->type, "string");
				$$ = tmp;
			}
		;

var_ref		: ID
		 	{
				printf("gg %s\n", $1);
				$$ = look_up_id_type($1);
			}
			| var_ref dim
		 	{
				$$ = $1;
			}
		;

dim			: MK_LB boolean_expr MK_RB
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
	exit(-1);
}

