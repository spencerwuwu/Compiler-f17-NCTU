/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

/**
 * Introduction to Compiler Design by Prof. Yi Ping You
 * Project 2 YACC sample
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int linenum;		    /* declared in lex.l */
extern FILE *yyin;		    /* declared by lex */
extern char *yytext;	    /* declared by lex */
extern char buf[256];		/* declared in lex.l */
extern int Opt_D;			/* list dump option */
extern int func_scope;
extern int yylex(void);
int yyerror(char* );


int func_attr_flag = 0;

// Definition of structs
typedef struct symbol_node {
    struct  symbol_node     *next;
    char    name[33];
    char    kind[200];
    char    type[200];
    char    attr[600];
	int 	attr_int;
	double	attr_double;
	int		type_no;
	// type_no:	0 - string
	//			1 - int
	//			2 - double
} Symbol_node;

typedef struct symbol_list {
    Symbol_node  *head, **tail;
} Symbol_list;

typedef struct scope_node {
	int level;
	char name[33];
	Symbol_list* Variable_list;
	struct scope_node	*next;
	struct scope_node	*prev;
} Scope_node;

typedef struct scope_list {
	Scope_node *head, **tail;
} Scope_list;

typedef struct id_node {		// Used for: var a, b, c = int;
    struct id_node      *next;
    char name[33];
} Id_node;

typedef struct id_list {
    Id_node *head, **tail;
} Id_list;

typedef struct for_it_node {		// Used for 'for' loop
    struct for_it_node      *next;
    struct for_it_node      *prev;
    char name[33];
} For_it_node;

typedef struct for_it_list {
    Id_node *head, **tail;
} For_it_list;



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
typedef struct array_node {
	struct array_node* next;
	struct array_node* prev;
	int size;
} Array_node;

typedef struct array_list {
	Array_node* head;
	char type[100];
} Array_list;

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

typedef struct func_node {
	char name[33];
	char type[100];
	char attr[200];
	int attr_flag;	// determine whether  attr is initialized
} Func_node;

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




#line 657 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ARRAY = 258,
    BEG = 259,
    BOOLEAN = 260,
    DEF = 261,
    DO = 262,
    ELSE = 263,
    END = 264,
    FALSE = 265,
    FOR = 266,
    INTEGER = 267,
    IF = 268,
    OF = 269,
    PRINT = 270,
    READ = 271,
    REAL = 272,
    RETURN = 273,
    STRING = 274,
    THEN = 275,
    TO = 276,
    TRUE = 277,
    VAR = 278,
    WHILE = 279,
    ID = 280,
    OCTAL_CONST = 281,
    INT_CONST = 282,
    FLOAT_CONST = 283,
    SCIENTIFIC = 284,
    STR_CONST = 285,
    OP_ADD = 286,
    OP_SUB = 287,
    OP_MUL = 288,
    OP_DIV = 289,
    OP_MOD = 290,
    OP_ASSIGN = 291,
    OP_EQ = 292,
    OP_NE = 293,
    OP_GT = 294,
    OP_LT = 295,
    OP_GE = 296,
    OP_LE = 297,
    OP_AND = 298,
    OP_OR = 299,
    OP_NOT = 300,
    MK_COMMA = 301,
    MK_COLON = 302,
    MK_SEMICOLON = 303,
    MK_LPAREN = 304,
    MK_RPAREN = 305,
    MK_LB = 306,
    MK_RB = 307
  };
#endif
/* Tokens.  */
#define ARRAY 258
#define BEG 259
#define BOOLEAN 260
#define DEF 261
#define DO 262
#define ELSE 263
#define END 264
#define FALSE 265
#define FOR 266
#define INTEGER 267
#define IF 268
#define OF 269
#define PRINT 270
#define READ 271
#define REAL 272
#define RETURN 273
#define STRING 274
#define THEN 275
#define TO 276
#define TRUE 277
#define VAR 278
#define WHILE 279
#define ID 280
#define OCTAL_CONST 281
#define INT_CONST 282
#define FLOAT_CONST 283
#define SCIENTIFIC 284
#define STR_CONST 285
#define OP_ADD 286
#define OP_SUB 287
#define OP_MUL 288
#define OP_DIV 289
#define OP_MOD 290
#define OP_ASSIGN 291
#define OP_EQ 292
#define OP_NE 293
#define OP_GT 294
#define OP_LT 295
#define OP_GE 296
#define OP_LE 297
#define OP_AND 298
#define OP_OR 299
#define OP_NOT 300
#define MK_COMMA 301
#define MK_COLON 302
#define MK_SEMICOLON 303
#define MK_LPAREN 304
#define MK_RPAREN 305
#define MK_LB 306
#define MK_RB 307

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 592 "parser.y" /* yacc.c:355  */

	double double_val;
	int int_val;
	char* str;
	struct symbol_node	*symb;
	struct id_node		*iden; 
	struct array_list	*arr;


#line 811 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 828 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   202

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  61
/* YYNRULES -- Number of rules.  */
#define YYNRULES  122
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  212

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   307

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   673,   673,   672,   693,   698,   699,   703,   702,   708,
     708,   718,   715,   721,   730,   727,   736,   733,   739,   745,
     753,   754,   757,   758,   761,   762,   763,   764,   767,   770,
     771,   774,   775,   778,   783,   790,   798,   804,   789,   811,
     815,   819,   824,   830,   829,   836,   836,   844,   850,   854,
     860,   865,   868,   869,   872,   873,   874,   875,   878,   880,
     885,   891,   892,   893,   894,   895,   896,   897,   901,   900,
     914,   915,   918,   919,   922,   923,   924,   927,   930,   935,
     938,   944,   943,   955,   958,   959,   962,   963,   966,   967,
     970,   971,   974,   975,   978,   979,   982,   983,   984,   985,
     986,   987,   990,   991,   994,   995,   998,   999,  1002,  1003,
    1004,  1007,  1008,  1009,  1010,  1011,  1012,  1013,  1014,  1015,
    1018,  1019,  1022
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ARRAY", "BEG", "BOOLEAN", "DEF", "DO",
  "ELSE", "END", "FALSE", "FOR", "INTEGER", "IF", "OF", "PRINT", "READ",
  "REAL", "RETURN", "STRING", "THEN", "TO", "TRUE", "VAR", "WHILE", "ID",
  "OCTAL_CONST", "INT_CONST", "FLOAT_CONST", "SCIENTIFIC", "STR_CONST",
  "OP_ADD", "OP_SUB", "OP_MUL", "OP_DIV", "OP_MOD", "OP_ASSIGN", "OP_EQ",
  "OP_NE", "OP_GT", "OP_LT", "OP_GE", "OP_LE", "OP_AND", "OP_OR", "OP_NOT",
  "MK_COMMA", "MK_COLON", "MK_SEMICOLON", "MK_LPAREN", "MK_RPAREN",
  "MK_LB", "MK_RB", "$accept", "program", "$@1", "program_body",
  "opt_decl_list", "decl_list", "$@2", "$@3", "decl", "$@4", "$@5", "$@6",
  "int_const", "int_literal_const", "d_literal_const", "str_literal_const",
  "bool_literal_const", "opt_func_decl_list", "func_decl_list",
  "func_decl", "$@7", "$@8", "$@9", "function_body", "opt_param_list",
  "param_list", "$@10", "$@11", "param", "id_list", "opt_type", "type",
  "scalar_type", "sort_array_type", "array_type", "stmt", "compound_stmt",
  "$@12", "opt_stmt_list", "stmt_list", "simple_stmt", "proc_call_stmt",
  "cond_stmt", "while_stmt", "for_stmt", "$@13", "return_stmt",
  "opt_boolean_expr_list", "boolean_expr_list", "boolean_expr",
  "boolean_term", "boolean_factor", "relop_expr", "rel_op", "expr",
  "add_op", "term", "mul_op", "factor", "var_ref", "dim", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307
};
# endif

#define YYPACT_NINF -123

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-123)))

#define YYTABLE_NINF -46

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -6,  -123,    24,     1,  -123,    35,    67,    76,    74,    82,
      89,  -123,   117,    76,  -123,    82,   104,  -123,  -123,    88,
    -123,  -123,  -123,  -123,  -123,    26,   113,    35,   114,    15,
      90,    93,   104,    91,  -123,    60,  -123,  -123,  -123,  -123,
    -123,  -123,  -123,  -123,  -123,  -123,  -123,    53,  -123,    95,
      97,  -123,  -123,  -123,    98,  -123,   100,  -123,  -123,    72,
     119,    39,    39,    39,    39,    39,    99,  -123,  -123,   133,
      91,  -123,  -123,  -123,  -123,  -123,  -123,   -20,   128,  -123,
    -123,  -123,  -123,  -123,   102,   107,   109,  -123,   105,   110,
     104,   105,  -123,   115,    28,    39,    39,  -123,  -123,  -123,
      -8,   108,  -123,  -123,    94,    65,  -123,   101,   -18,    -9,
       2,     4,    39,  -123,  -123,    39,    39,  -123,    60,  -123,
    -123,  -123,  -123,  -123,  -123,  -123,  -123,  -123,   123,    39,
     116,    39,   101,  -123,   -27,    91,    39,    39,  -123,  -123,
    -123,  -123,  -123,  -123,  -123,  -123,    64,    64,  -123,  -123,
    -123,    64,  -123,  -123,  -123,    91,   112,   120,   124,    30,
     -23,   149,   163,    60,   121,    39,   -22,  -123,   103,   108,
    -123,    96,    65,  -123,   160,   122,    39,  -123,  -123,   105,
      35,   151,  -123,   125,  -123,    91,   161,   169,  -123,   124,
    -123,  -123,    91,   168,    60,  -123,   170,  -123,  -123,   117,
    -123,   171,   167,  -123,   172,    91,  -123,   157,   174,  -123,
     177,  -123
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     2,     0,     0,     1,     6,     0,    32,     5,     0,
       0,    35,     0,    31,    34,     0,     0,    10,     3,     0,
      68,     4,    33,     8,    49,     0,    42,     6,     0,     0,
       0,    41,     0,    71,    48,     0,    56,    30,    54,    55,
      57,    29,    21,    20,    24,    26,    28,     0,    22,     0,
       0,    14,    16,    11,     0,    58,    51,    43,    46,     0,
       0,     0,     0,     0,     0,     0,   120,    73,    61,     0,
      70,    62,    67,    63,    64,    65,    66,     0,     0,    25,
      27,    23,    18,    19,     0,     0,     0,    13,     0,     0,
       0,     0,    81,   120,     0,     0,     0,   117,   118,   119,
       0,    89,    91,    93,    95,   103,   107,   111,     0,     0,
       0,     0,    85,    69,    72,     0,     0,   121,     0,    15,
      17,    12,    50,    52,    53,    36,    44,    47,     0,    85,
     120,     0,   112,    92,     0,    71,     0,     0,   104,   105,
      98,   101,   100,    96,    99,    97,     0,     0,   108,   109,
     110,     0,    75,    76,    83,    71,     0,    84,    87,     0,
       0,     0,     0,     0,     0,    85,     0,   113,     0,    88,
      90,    94,   102,   106,     0,     0,     0,    74,   122,     0,
       6,     0,   115,     0,   114,    71,     0,     0,    77,    86,
      59,    60,    71,     0,     0,   116,     0,    79,    80,    40,
      37,     0,     0,    39,     0,    71,    78,     0,     0,    38,
       0,    82
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -123,  -123,  -123,  -123,   -24,  -123,  -123,  -123,   173,  -123,
    -123,  -123,   -33,   156,   162,   164,  -123,  -123,  -123,   176,
    -123,  -123,  -123,  -123,  -123,  -123,  -123,  -123,   106,   178,
    -123,   111,   -25,   166,     7,   127,   -12,  -123,  -122,  -123,
    -123,  -123,  -123,  -123,  -123,  -123,  -123,  -114,  -123,   -56,
      54,   -85,  -123,  -123,    46,  -123,    51,  -123,    48,   -32,
    -123
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     6,     7,     8,    15,     9,    17,    86,
      84,    85,    48,    97,    98,    99,    52,    12,    13,    14,
      19,   162,   204,   193,    30,    31,    90,    32,    58,    59,
      89,   122,   123,   124,    55,    67,    68,    27,    69,    70,
      71,    72,    73,    74,    75,   128,    76,   156,   157,   158,
     101,   102,   103,   146,   104,   147,   105,   151,   106,   107,
     117
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      21,    77,    78,    33,    53,   100,   108,   109,   110,   111,
     133,   155,   135,   168,    81,   164,   115,   136,    35,     1,
      36,   136,   136,   167,     4,    37,   136,    38,   184,   178,
     152,   116,    39,   174,    40,   136,   136,    41,    77,   153,
     134,    42,    43,    44,    45,    46,   136,    47,   136,     5,
     154,   183,   170,   130,    42,    43,    79,    80,    -9,   159,
     160,    81,   132,   196,    93,    42,    43,    44,    45,    46,
     199,    94,    28,    29,   136,   166,    10,   131,   177,    42,
      43,    79,    80,   208,    95,   161,    42,    43,    96,    93,
      42,    43,    44,    45,    46,    20,    94,    -7,   148,   149,
     150,    11,    60,    77,    61,    16,    62,    63,    35,    64,
      36,   185,   186,    96,    18,    65,    66,    38,    28,    91,
     189,    20,    39,    77,    40,   138,   139,   138,   139,    24,
     181,   140,   141,   142,   143,   144,   145,    26,   -45,    34,
      56,    57,   113,    82,    92,    83,    87,    88,   112,   118,
     119,   137,   116,    77,   190,   120,   192,   121,   125,   163,
      77,   201,   175,   179,   129,   165,   176,   180,   136,   187,
     188,   182,   194,    77,   197,   195,   198,   200,   205,   202,
     206,   207,   209,   210,   211,    49,   191,   203,    23,    22,
     169,    50,   171,    51,    25,    54,   126,   114,   172,   173,
       0,     0,   127
};

static const yytype_int16 yycheck[] =
{
      12,    33,    35,    27,    29,    61,    62,    63,    64,    65,
      95,     7,    20,   135,    47,   129,    36,    44,     3,    25,
       5,    44,    44,    50,     0,    10,    44,    12,    50,    52,
      48,    51,    17,   155,    19,    44,    44,    22,    70,    48,
      96,    26,    27,    28,    29,    30,    44,    32,    44,    48,
      48,   165,   137,    25,    26,    27,    28,    29,    23,   115,
     116,    94,    94,   185,    25,    26,    27,    28,    29,    30,
     192,    32,    46,    47,    44,   131,     9,    49,    48,    26,
      27,    28,    29,   205,    45,   118,    26,    27,    49,    25,
      26,    27,    28,    29,    30,     4,    32,    23,    33,    34,
      35,    25,    11,   135,    13,    23,    15,    16,     3,    18,
       5,     8,     9,    49,    25,    24,    25,    12,    46,    47,
     176,     4,    17,   155,    19,    31,    32,    31,    32,    25,
     163,    37,    38,    39,    40,    41,    42,    49,    25,    25,
      50,    48,     9,    48,    25,    48,    48,    47,    49,    21,
      48,    43,    51,   185,   179,    48,   180,    48,    48,    36,
     192,   194,    50,    14,    49,    49,    46,     4,    44,     9,
      48,    50,    21,   205,    13,    50,     7,     9,     7,     9,
      13,     9,    25,     9,     7,    29,   179,   199,    15,    13,
     136,    29,   146,    29,    16,    29,    90,    70,   147,   151,
      -1,    -1,    91
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    25,    54,    55,     0,    48,    56,    57,    58,    60,
       9,    25,    70,    71,    72,    59,    23,    61,    25,    73,
       4,    89,    72,    61,    25,    82,    49,    90,    46,    47,
      77,    78,    80,    57,    25,     3,     5,    10,    12,    17,
      19,    22,    26,    27,    28,    29,    30,    32,    65,    66,
      67,    68,    69,    85,    86,    87,    50,    48,    81,    82,
      11,    13,    15,    16,    18,    24,    25,    88,    89,    91,
      92,    93,    94,    95,    96,    97,    99,   112,    65,    28,
      29,    65,    48,    48,    63,    64,    62,    48,    47,    83,
      79,    47,    25,    25,    32,    45,    49,    66,    67,    68,
     102,   103,   104,   105,   107,   109,   111,   112,   102,   102,
     102,   102,    49,     9,    88,    36,    51,   113,    21,    48,
      48,    48,    84,    85,    86,    48,    81,    84,    98,    49,
      25,    49,   112,   104,   102,    20,    44,    43,    31,    32,
      37,    38,    39,    40,    41,    42,   106,   108,    33,    34,
      35,   110,    48,    48,    48,     7,   100,   101,   102,   102,
     102,    65,    74,    36,   100,    49,   102,    50,    91,   103,
     104,   107,   109,   111,    91,    50,    46,    48,    52,    14,
       4,    65,    50,   100,    50,     8,     9,     9,    48,   102,
      85,    87,    57,    76,    21,    50,    91,    13,     7,    91,
       9,    65,     9,    89,    75,     7,    13,     9,    91,    25,
       9,     7
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    53,    55,    54,    56,    57,    57,    59,    58,    60,
      58,    62,    61,    61,    63,    61,    64,    61,    61,    61,
      65,    65,    66,    66,    67,    67,    67,    67,    68,    69,
      69,    70,    70,    71,    71,    73,    74,    75,    72,    76,
      76,    77,    77,    79,    78,    80,    78,    81,    82,    82,
      83,    83,    84,    84,    85,    85,    85,    85,    86,    87,
      87,    88,    88,    88,    88,    88,    88,    88,    90,    89,
      91,    91,    92,    92,    93,    93,    93,    94,    95,    95,
      96,    98,    97,    99,   100,   100,   101,   101,   102,   102,
     103,   103,   104,   104,   105,   105,   106,   106,   106,   106,
     106,   106,   107,   107,   108,   108,   109,   109,   110,   110,
     110,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     112,   112,   113
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     6,     3,     1,     0,     0,     3,     0,
       2,     0,     6,     5,     0,     6,     0,     6,     5,     5,
       1,     1,     1,     2,     1,     2,     1,     2,     1,     1,
       1,     1,     0,     2,     1,     0,     0,     0,    14,     3,
       2,     1,     0,     0,     4,     0,     2,     3,     3,     1,
       2,     0,     1,     1,     1,     1,     1,     1,     1,     6,
       6,     1,     1,     1,     1,     1,     1,     1,     0,     5,
       1,     0,     2,     1,     4,     3,     3,     5,     8,     6,
       6,     0,    11,     3,     1,     0,     3,     1,     3,     1,
       3,     1,     2,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     2,     3,     4,     4,     5,     1,     1,     1,
       1,     2,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 673 "parser.y" /* yacc.c:1646  */
    { 
					Scopes_l = new_scope_list();
					Scopes_s = new_scope_list();
					current_level = 0;

					scope_stack_init();
					iden_list = new_id_list();
					for_list = new_for_list();

					global_push_back((yyvsp[0].str), "void", "program", "");
				}
#line 2081 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 687 "parser.y" /* yacc.c:1646  */
    {	
					current_level--;
					scope_stack_pop();	
				}
#line 2090 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 703 "parser.y" /* yacc.c:1646  */
    { 
		   			iden_list = new_id_list();  
					arrays = new_array();
				}
#line 2099 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 708 "parser.y" /* yacc.c:1646  */
    { 
					iden_list = new_id_list(); 
					arrays = new_array();
				}
#line 2108 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 718 "parser.y" /* yacc.c:1646  */
    {
					symbols_push_back((yyvsp[0].str), "variable", "");
				}
#line 2116 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 724 "parser.y" /* yacc.c:1646  */
    { 
					symbols_push_back((yyvsp[-1].str), "variable", "");
				}
#line 2124 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 730 "parser.y" /* yacc.c:1646  */
    { 
					symbols_push_back("string", "constant", (yyvsp[0].str));
				}
#line 2132 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 736 "parser.y" /* yacc.c:1646  */
    { 
					symbols_push_back("boolean", "constant", (yyvsp[0].str));
				}
#line 2140 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 742 "parser.y" /* yacc.c:1646  */
    { 
					symbols_int_push_back("integer", "constant", (yyvsp[-1].int_val));
				}
#line 2148 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 748 "parser.y" /* yacc.c:1646  */
    { 
					symbols_double_push_back("real", "constant", (yyvsp[-1].double_val));
				}
#line 2156 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 753 "parser.y" /* yacc.c:1646  */
    { (yyval.int_val) = (yyvsp[0].int_val); }
#line 2162 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 754 "parser.y" /* yacc.c:1646  */
    { (yyval.int_val) = string_to_oct((yyvsp[0].str)); }
#line 2168 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 757 "parser.y" /* yacc.c:1646  */
    { (yyval.int_val) = (yyvsp[0].int_val); }
#line 2174 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 758 "parser.y" /* yacc.c:1646  */
    { (yyval.int_val) = -(yyvsp[0].int_val); }
#line 2180 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 761 "parser.y" /* yacc.c:1646  */
    { (yyval.double_val) = (yyvsp[0].double_val); }
#line 2186 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 762 "parser.y" /* yacc.c:1646  */
    { (yyval.double_val) = -(yyvsp[0].double_val); }
#line 2192 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 763 "parser.y" /* yacc.c:1646  */
    { (yyval.double_val) = (yyvsp[0].double_val); }
#line 2198 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 764 "parser.y" /* yacc.c:1646  */
    { (yyval.double_val) = -(yyvsp[0].double_val); }
#line 2204 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 767 "parser.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 2210 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 770 "parser.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 2216 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 771 "parser.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 2222 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 780 "parser.y" /* yacc.c:1646  */
    {
						global_push_back(current_func->name, current_func->type, "function", current_func->attr);
					}
#line 2230 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 784 "parser.y" /* yacc.c:1646  */
    {
						global_push_back(current_func->name, current_func->type, "function", current_func->attr);
					}
#line 2238 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 790 "parser.y" /* yacc.c:1646  */
    {
				current_level++;
				scope_stack_push();
				current_func = new_func();
				func_set_name((yyvsp[0].str));
				func_attr_flag = 1;
			}
#line 2250 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 798 "parser.y" /* yacc.c:1646  */
    {
				func_attr_flag = 0;
			}
#line 2258 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 804 "parser.y" /* yacc.c:1646  */
    { 
				current_level--;
				scope_stack_pop();
			}
#line 2267 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 820 "parser.y" /* yacc.c:1646  */
    {
					arrays = new_array();
				}
#line 2275 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 824 "parser.y" /* yacc.c:1646  */
    {
					arrays = new_array();
				}
#line 2283 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 830 "parser.y" /* yacc.c:1646  */
    {
					iden_list = new_id_list();
					arrays = new_array();
				}
#line 2292 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 836 "parser.y" /* yacc.c:1646  */
    {
					iden_list = new_id_list();
					arrays = new_array();
				}
#line 2301 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 845 "parser.y" /* yacc.c:1646  */
    {
			symbols_push_back((yyvsp[0].str), "parameter", "");
		}
#line 2309 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 851 "parser.y" /* yacc.c:1646  */
    { 
				id_push_back((yyvsp[0].str)); 
			}
#line 2317 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 855 "parser.y" /* yacc.c:1646  */
    { 
				id_push_back((yyvsp[0].str)); 
			}
#line 2325 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 861 "parser.y" /* yacc.c:1646  */
    { 
				(yyval.str) = (yyvsp[0].str); 
				func_set_type((yyvsp[0].str));
			}
#line 2334 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 872 "parser.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str);}
#line 2340 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 873 "parser.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str);}
#line 2346 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 874 "parser.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str);}
#line 2352 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 875 "parser.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str);}
#line 2358 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 878 "parser.y" /* yacc.c:1646  */
    { sort_array((yyval.str)); }
#line 2364 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 881 "parser.y" /* yacc.c:1646  */
    {
					array_push((yyvsp[-2].int_val)-(yyvsp[-4].int_val)+1);
					strcpy(arrays->type, (yyvsp[0].str));
				}
#line 2373 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 886 "parser.y" /* yacc.c:1646  */
    {
					array_push((yyvsp[-2].int_val)-(yyvsp[-4].int_val)+1);
				}
#line 2381 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 901 "parser.y" /* yacc.c:1646  */
    { 
					current_level++;
					scope_stack_push();
				}
#line 2390 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 908 "parser.y" /* yacc.c:1646  */
    { 
					current_level--;
					scope_stack_pop();
				}
#line 2399 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 944 "parser.y" /* yacc.c:1646  */
    {
					for_it_push((yyvsp[0].str));
				}
#line 2407 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 950 "parser.y" /* yacc.c:1646  */
    {
					for_it_pop();
				}
#line 2415 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2419 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1025 "parser.y" /* yacc.c:1906  */


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

