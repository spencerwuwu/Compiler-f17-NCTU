#ifndef _SEMCHECK_H_
#define _SEMCHECK_H_
#include "header.h"
#include "symtab.h"

struct idNode_sem *createIdList( const char *str );
void idlist_addNode( struct idNode_sem *node, const char *string );

struct ConstAttr *createConstAttr( SEMTYPE type, void *value );
struct PType *createPType( SEMTYPE type );
struct expr_sem *createExprSem( const char *id ); 
void increaseArrayDim( struct PType *pType, int lo, int hi );

void increaseDim( struct expr_sem* expr, SEMTYPE dimType );

struct param_sem *createParam( struct idNode_sem *ids, struct PType *pType );
void param_sem_addParam( struct param_sem *lhs, struct param_sem *rhs );

__BOOLEAN insertParamIntoSymTable( struct SymTable *table, struct param_sem *params, int scope );
void insertFuncIntoSymTable( struct SymTable *table, const char *id, struct param_sem *params, struct PType *retType, int scope );
void insertLoopVarIntoTable( struct SymTable *table, const char *id );
/* verification */
__BOOLEAN verifyRedeclaration( struct SymTable *table, const char *str, int scope );

struct PType *copyPType( struct PType *src );
struct ArrayDimNode *copyArrayDimList( struct ArrayDimNode *src );

void deletePType( struct PType *type );
void deleteSymAttr( union SymAttr *attr, SEMTYPE category );
void deleteIdList( struct idNode_sem *idlist );

void checkExpr_sem( struct SymTable *table, struct expr_sem *var, struct expr_sem *stmt, int scope );
__BOOLEAN verifyVarDeclaration( struct SymTable *table, const char *str, int scope );
SEMTYPE getDeclarationType( struct SymTable *table, const char *str, int scope );
struct PType* verifyFuncInvoke( char *id, struct expr_sem *exprPtr, struct SymTable *table, int scope );
__BOOLEAN verifyFuncInvokeAttr( struct SymNode *target, struct expr_sem *exprPtr, struct SymTable *table, int scope );

char *getTypeString( SEMTYPE type );

#endif

