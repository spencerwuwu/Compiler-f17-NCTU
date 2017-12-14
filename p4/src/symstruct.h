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

typedef struct func_node {
	char name[33];
	char type[100];
	char attr[200];
	int attr_flag;	// determine whether  attr is initialized
} Func_node;

typedef struct type_node {
    int emptyness;
	char type[100];
} Type_node;
