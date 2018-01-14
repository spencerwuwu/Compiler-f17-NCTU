# Usage:
./parser  [filename]

# Platform: 
Linux, Unix like system

# Abilities:
This is the extension semantic check or the previous project.
In this project, following functions are added to complete the 
abilities required in the spec, which generates jasmin code.

```
	void stop_outputing();
	void writeline( char* line );

	void fileName_to_codegenName( );
	void init_codegen( );

	void variable_decl( int scope, char *name, struct PType *type, struct SymNode *newNode);

	int get_localVar_num( struct SymTable *table, struct expr_sem *expr, int scope, __BOOLEAN isAssignmentLHS);

	struct SymNode *getLocalDecl( struct SymTable *table, const char *id, int scope );
	struct SymNode *getGlobalDecl( struct SymTable *table, const char *id, int scope );
	int getNoneuseReg( struct SymTable *table, int scope );

	void enter_block( int var_tmp );
	void leave_block( struct SymTable *table, int __scope );

	void var_codegen( struct SymTable *table, struct expr_sem *op1, int scope );
	void function_call_codegen(const char *id, struct expr_sem *exprList, struct SymTable *table, int scope);
	void assign_codegen( struct SymTable *table, struct expr_sem *lhs, struct expr_sem *rhs, int scope );
	void print_heading_codegen();
	void print_codegen( struct SymTable *table, struct expr_sem *rhs, int scope );
	void read_codegen( struct SymTable *table, struct expr_sem *rhs, int scope );
	void return_codegen(struct expr_sem *expr, struct PType *funcReturn);
	void function_heading_codegen( const char *id, struct param_sem *params, struct PType *retType );
	void function_ending_codegen( struct PType *retType );
	void ifwhile_heading_codegen( struct LabelTable *table );
	void if_ending_codegen( struct LabelTable *table );
	void else_codegen( struct LabelTable *table );
	void int_codegen( int value );
	void rel_op_codegen( struct SymTable *table, struct expr_sem *op1, struct expr_sem *op2, OPERATOR operator_, int scope, struct LabelNode *node );
	void for_heading_codegen( struct LabelTable *table, char *id, int lo, int hi, struct SymTable *stable );
	void for_ending_codegen( struct LabelTable *table, char *id, struct SymTable *stable );
	void while_ending_codegen( struct LabelTable *table );
	void neg_codegen( struct expr_sem *ptr );

	void arith_codegen( struct SymTable *table, struct expr_sem *op1, struct expr_sem *op2, OPERATOR _operator, int scope );

	struct LabelNode *createLabel( int num, char* type, __BOOLEAN condition  );
	void insertLabel(struct LabelTable *table, struct LabelNode *newNode);
	struct LabelNode * getCurrentLabel( struct LabelTable *table );
	char* getStartLabel( struct LabelTable *table );
	char* getTrueLabel( struct LabelTable *table );
	char* getFalseLabel( struct LabelTable *table );
	char* getExitLabel( struct LabelTable *table );
	void removeLabel( struct LabelTable *table );
	void write_label_starter( struct LabelTable *table );
```
