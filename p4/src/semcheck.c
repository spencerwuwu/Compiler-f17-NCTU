#include <stdio.h>
#include "semcheck.h"
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "symtab.h"

extern int linenum;
extern __BOOLEAN performError;

void printOperator( OPERATOR op )
{
	switch( op ) {
	 case ADD_t:
		fprintf(stdout,"+");
		break;
	 case SUB_t:
		fprintf(stdout,"-");
		break;
	 case MUL_t:
		fprintf(stdout,"*");
		break;
	 case DIV_t:
		fprintf(stdout,"/");
		break;
	 case MOD_t:
		fprintf(stdout,"mod");
		break;
	 case LT_t:
		fprintf(stdout,"<");
		break;
	 case LE_t:
		fprintf(stdout,"<=");
		break;
	 case EQ_t:
		fprintf(stdout,"=");
		break;
	 case GE_t:
		fprintf(stdout,">=");
		break;
	 case GT_t:
		fprintf(stdout,">");
		break;
	 case NE_t:
		fprintf(stdout,"<>");
		break;
	 case AND_t:
		fprintf(stdout,"and");
		break;
	 case OR_t:
	 	fprintf(stdout,"or");
		break;
	 default:
		/* FIXME */
		break;
	}
}

struct idNode_sem *createIdList( const char *str )
{
	struct idNode_sem *newNode = (struct idNode_sem *)malloc(sizeof(struct idNode_sem));
	newNode->value = (char *)malloc(sizeof(char)*(strlen(str)+1));
	strcpy( newNode->value, str );
	newNode->next = 0;

	return newNode;
}

struct ConstAttr *createConstAttr( SEMTYPE type, void *value )
{
	struct ConstAttr *result = (struct ConstAttr *)malloc(sizeof(struct ConstAttr));
	result->category = type;

	result->hasMinus = __FALSE;

	switch( type ) {
	 case INTEGER_t:
		result->value.integerVal = *(int*)value;
		if( *(int*)value < 0 )
			result->hasMinus = __TRUE;
		break;
	 case REAL_t:
		result->value.realVal = *(float*)value;
		if( *(float*)value < 0.0 )
			result->hasMinus = __TRUE;
		break;
	 case STRING_t:
		result->value.stringVal = (char *)malloc(sizeof(char)*(strlen((char *)value)+1));
		strcpy( result->value.stringVal, (char *)value );
		break;
	 case BOOLEAN_t:
		result->value.booleanVal = *(__BOOLEAN*)value;
		break;
	 default:
		/* FIXME */
		break;
	}
	return result;
}

struct param_sem *createParam( struct idNode_sem *ids, struct PType *pType )
{
	struct param_sem *result = (struct param_sem *)malloc(sizeof(struct param_sem));
	result->idlist = ids;
	result->pType = pType;
	result->next = 0;

	return result;
}

struct expr_sem *createExprSem( const char *id ) 
{
	struct expr_sem *result = (struct expr_sem *)malloc(sizeof(struct expr_sem));
	// setup beginningOp
	result->beginningOp = NONE_t;
	// setup isDeref
	result->isDeref = __FALSE;
	// setup varRef
	result->varRef = (struct var_ref_sem *)malloc(sizeof(struct var_ref_sem));
	result->varRef->id = (char *)malloc(sizeof(char)*(strlen(id)+1));
	strcpy( result->varRef->id, id );
	result->varRef->dimNum = 0;
	result->varRef->dim = 0;
	// setup pType
	result->pType = 0;

	result->next = 0;

	return result;
}

void increaseDim( struct expr_sem* expr, SEMTYPE dimType )
{
	struct typeNode *newNode = (struct typeNode *)malloc(sizeof(struct typeNode));
	newNode->value = dimType;
	newNode->next = 0;

	if( expr->varRef->dim == 0 ) {	// the first dim
		++(expr->varRef->dimNum);
		expr->varRef->dim = newNode;
	}
	else {	// others, attached after the last
		struct typeNode *typePtr;
		for( typePtr=(expr->varRef->dim) ; (typePtr->next)!=0 ; typePtr=(typePtr->next) );
		typePtr->next = newNode;
		++(expr->varRef->dimNum);
	}
}

void idlist_addNode( struct idNode_sem *node, const char *string )
{
	struct idNode_sem *newNode = 0;
	newNode = (struct idNode_sem *)malloc(sizeof(struct idNode_sem ));
	newNode->value = (char *)malloc(sizeof(char)*(strlen(string)+1));
	strcpy( newNode->value, string );
	newNode->next = 0;

	struct idNode_sem *ptr;
	for( ptr=node ; (ptr->next)!=0 ; ptr=(ptr->next) );
	// add into idlist
	ptr->next = newNode;
}

void param_sem_addParam( struct param_sem *lhs, struct param_sem *rhs )
{
	struct param_sem *ptr;
	for( ptr=lhs ; (ptr->next)!=0 ; ptr=(ptr->next) );
	ptr->next = rhs;

}

struct PType *createPType( SEMTYPE type )
{
	struct PType *result = (struct PType *)malloc(sizeof(struct PType));
	result->isError = __FALSE;
	result->isArray = __FALSE;	// scalar
	result->type = type;
	result->dimNum = 0;
	result->dim = 0;		// null
	
	return result;
}

void increaseArrayDim( struct PType *pType, int lo, int hi )
{
	if( pType->isArray == __FALSE )
		pType->isArray = __TRUE;
	
	/* increase # of dim */
	++(pType->dimNum);
	// setup properties of newDim
	struct ArrayDimNode *newDim = (struct ArrayDimNode *)malloc(sizeof(struct ArrayDimNode));
    if( lo >= hi ) {
        fprintf( stdout, "<Error> found in Line %d: array lower bound must be smaller than uppder bound\n", linenum );
    }
    if( lo < 0 || hi < 0 ) {
        fprintf( stdout, "<Error> found in Line %d: array lower bound and uppder bound must be greater or equal than 0\n", linenum );
    }
	newDim->low = lo;
	newDim->high = hi;
	newDim->size = hi-lo+1;
	newDim->next = 0;	
	// add newDim into pType, in the front of list
	newDim->next = pType->dim;
	pType->dim = newDim;
}

struct ArrayDimNode *copyArrayDimList( struct ArrayDimNode *src )
{
	if( src == 0 ) {
		return 0;
	}
	else {
		// copy the first element
		struct ArrayDimNode *dest = (struct ArrayDimNode *)malloc(sizeof(struct ArrayDimNode));;
		dest->low = src->low;
		dest->high = src->high;
		dest->size = src->size;
		dest->next = 0;

		struct ArrayDimNode *ptr = dest;	// ptr: points to the last element of new list
		struct ArrayDimNode *arrPtr;
		for( arrPtr=(src->next) ; arrPtr!=0 ; arrPtr=(arrPtr->next) ) {
				struct ArrayDimNode *newNode = (struct ArrayDimNode *)malloc(sizeof(struct ArrayDimNode));
				newNode->low = arrPtr->low;
				newNode->high = arrPtr->high;
				newNode->size = arrPtr->size;
				newNode->next = 0;

				ptr->next = newNode;	// attach to list
				ptr = newNode;
		}
		return dest;
	}
}

struct PType *copyPType( struct PType *src )
{
	if( src == 0 ) {
		return 0;
	}
	else {
		struct PType *dest = (struct PType *)malloc(sizeof(struct PType));
		dest->isError = src->isError;
		dest->isArray = src->isArray;
		dest->type = src->type;
		dest->dimNum = src->dimNum;
		dest->dim = copyArrayDimList( src->dim );
		return dest;
	}
}

/* verification(s) */


__BOOLEAN verifyRedeclaration( struct SymTable *table, const char *str, int scope )
{
	__BOOLEAN result = __TRUE;
	// first check loop variable(s)
	if( lookupLoopVar( table, str ) != 0 ) {
		fprintf( stdout, "<Error> found in Line %d: symbol '%s' is redeclared\n", linenum, str );
		result = __FALSE;
	}
	else {	// then check normal variable(s)
		if( lookupSymbol( table, str, scope, __TRUE ) == 0 ) {	// only search current scope
			result =  __TRUE;
		}
		else {
		    fprintf( stdout, "<Error> found in Line %d: symbol '%s' is redeclared\n", linenum, str );
			result = __FALSE;
		}
	}
	return result;
}

__BOOLEAN insertParamIntoSymTable( struct SymTable *table, struct param_sem *params, int scope )
{
	__BOOLEAN result = __FALSE;

	// without parameters
	if( params == 0 ) {
		result = __FALSE;
	}
	else {
		struct param_sem *parPtr;
		struct idNode_sem *idPtr;
		struct SymNode *newNode;
		for( parPtr=params ; parPtr!=0 ; parPtr=(parPtr->next) ) {

            if( parPtr->pType->isError == __TRUE ) {
				result = __TRUE;
			}	// array_type error ?
			else {
				for( idPtr=(parPtr->idlist) ; idPtr!=0 ; idPtr=(idPtr->next) ) {
					if( verifyRedeclaration( table, idPtr->value, scope ) ==__FALSE ) { result = __TRUE;  }
					else {	// without error, insert into symbol table
						newNode = createParamNode( idPtr->value, scope, parPtr->pType );
						insertTab( table, newNode );
					}
				}
			}
		}
	}
	return result;	// __TRUE: with some error(s)
}

void insertFuncIntoSymTable( struct SymTable *table, const char *id, struct param_sem *params, struct PType *retType, int scope )
{
	if( verifyRedeclaration( table, id, scope ) == __FALSE ) { return; }
	else {
		struct FuncAttr *formalParam = (struct FuncAttr *)malloc(sizeof(struct FuncAttr ));
		formalParam->paramNum = 0;
		formalParam->params = 0;

		if( params == 0 ) {
			// without parameters
		}
		else {
			struct param_sem *parPtr;
			struct idNode_sem *idPtr;

			struct PTypeList *lastPtr = 0;	// incicate the last element in PType list

			for( parPtr=params ; parPtr!=0 ; parPtr=(parPtr->next) ) {
				for( idPtr=(parPtr->idlist) ; idPtr!=0 ; idPtr=(idPtr->next) ) {
					if( formalParam->paramNum == 0 ) {	// add the first entry
						formalParam->params = (struct PTypeList *)malloc(sizeof(struct PTypeList));
						formalParam->params->value = copyPType( parPtr->pType );
						formalParam->params->next = 0;

						++(formalParam->paramNum);

						lastPtr = formalParam->params;
					}
					else {
						struct PTypeList *newPTypeList = (struct PTypeList *)malloc(sizeof(struct PTypeList));
						newPTypeList->value = copyPType( parPtr->pType );
						newPTypeList->next = 0;
						lastPtr->next = newPTypeList;
						++(formalParam->paramNum);

						lastPtr = (lastPtr->next);
					}
				}
			}
		}

		struct SymNode *newNode = createFuncNode( id, scope, retType, formalParam );
		insertTab( table, newNode );
	}
}

void insertLoopVarIntoTable( struct SymTable *table, const char *id )
{
	struct SymNode *nodePtr = 0;
	nodePtr = lookupLoopVar( table, id );
	if( nodePtr != 0 ) {
		fprintf( stdout, "<Error> found in Line %d: symbol '%s' is redeclared\n", linenum, id );
        // Push it even redeclared, it will be pop put when leave
        pushLoopVar( table, createLoopVarNode(id) );
	}
	else {
		//struct SymNode *newNode = createLoopVarNode( id );
		pushLoopVar( table, createLoopVarNode(id) );
	}
}

//TODO
// major bug(s)......
void deletePType( struct PType *type )
{
	if( type != 0 ) {
	//struct ArrayDimNode *current, *previous;
		if( type->dim == 0 ) {  }
		else if( type->dim->next == 0 )	{ 
			free( type->dim ); 
		}
		else {
			//struct ArrayDimNode *current, *previous, *nextt;
			//int i;
			//for( i=0, current=type->dim ; i<(type->dimNum) ; i++ ) {
			//	nextt = current->next;
			//	printf("(%d,%d,%d)", current->low, current->high, current->size);
				//free(current);
			//	current = nextt;
			//}
			//printf("\n\n");

		//	for( current=(type->dim->next), previous=(type->dim) ; current!=0 ; previous=current, current=(current->next) ) {
				//free( previous );
		//	}
		// for the last object...
			//free( previous );
		}

		//free( type );

	}
	//type->dim = 0;
}

void deleteSymAttr( union SymAttr *attr, SEMTYPE category )
//void deleteSymAttr( union SymAttr *attr, SEMTYPE category )
{
	if( attr != 0 ) {

		//if( attr->constVal != 0 ) {
		if( category == CONSTANT_t ) {
			if( attr->constVal->category == STRING_t ) {
				free( attr->constVal->value.stringVal );
			}

			free( attr->constVal );
		}
		else if( category == FUNCTION_t ) {
		//else if( attr->formalParam != 0 ) {
			if( attr->formalParam->params == 0 ) {  }
			else if( attr->formalParam->params->next == 0 ) {
				deletePType( attr->formalParam->params->value );
			}
			else {
				struct PTypeList *current, *previous;
				for( current=(attr->formalParam->params->next), previous=(attr->formalParam->params) ; current!=0 ; current=(current->next), previous=(previous->next) ) {
					deletePType( previous->value );
					free( previous );
				}
				// last object...
				deletePType( previous->value );
				free( previous );
			}
			free( attr->formalParam );
		}
	free( attr );

	}
}

void deleteIdList( struct idNode_sem *idlist )
{
	if( idlist == 0 )	return;
	else if( idlist->next == 0 ) {
		if( idlist->value != 0 )
			free( idlist->value );
		free( idlist );
	}
	else {
		struct idNode_sem *current, *previous;
		for( current=(idlist->next), previous=idlist ; current!=0 ; previous=current, current=(current->next) ) {
			if( previous->value != 0 )
				free( previous->value );
		}
		if( previous->value != 0 )
			free( previous->value );

		free( idlist );
	}
}

void checkExpr_sem( struct SymTable *table, struct expr_sem *var, struct expr_sem *stmt, int scope) {
    char var_name[200];
    struct PType *var_type, *stmt_type;
    __BOOLEAN no_error = __TRUE;
    strcpy(var_name, var->varRef->id);
    if( verifyVarDeclaration(table, var, scope) ) {
        var_type = getDeclarationType( table, var_name, scope );
    }
    else {
        no_error = __FALSE;
    }
    
    if( stmt->varRef != 0) {
        if( verifyVarDeclaration(table, var, scope) ) {
            stmt_type = getDeclarationType( table, var_name, scope );
        }
    }
    else {
        stmt_type = stmt->pType;
    }

    __BOOLEAN result = __FALSE;
    if( no_error ) {
        if( var_type->type == REAL_t) {
            if (stmt_type->type == REAL_t || stmt_type->type == INTEGER_t) {
                result = __TRUE;
            }
        }
        else {
            if( var_type->type == stmt_type->type ) {
                result = __TRUE;
            }
        }
    }

    if( no_error && result == __FALSE) {
	    printf("<Error> found in Line %d: type mismatch, LHS= %s, RHS= %s\n", linenum, getTypeString(var_type), getTypeString(stmt_type));
    }

}

__BOOLEAN verifyVarDeclaration( struct SymTable *table, struct expr_sem *exprPtr, int scope )
{
	__BOOLEAN result = __TRUE;
    struct SymNode* node;
    char str[200];
    strcpy(str, exprPtr->varRef->id);
	// first check loop variable(s)
	if( lookupLoopVar( table, str ) != 0 ) {
		result = __TRUE;
	}
	else {	// then check normal variable(s)
		if( (node = lookupSymbOrFunc( table, str, scope )) == 0 ) {	// only search current scope
		    fprintf( stdout, "<Error> found in Line %d: variable '%s' had not be declared\n", linenum, str );
			result =  __FALSE;
		}
        else if( node->category == CONSTANT_t ) {
		    fprintf( stdout, "<Error> found in Line %d: constant '%s' cannot be assigned\n", linenum, str );
        }
		else {
			result = __TRUE;
		}
	}
	return result;
}

struct PType* getDeclarationType( struct SymTable *table, const char *str, int scope )
{
	struct PType *result;
    struct SymNode *target;
	// first check loop variable(s)
	if( lookupLoopVar( table, str ) != 0 ) {
		result = createPType( INTEGER_t );
	}
	else {	// then check normal variable(s)
		result = lookupSymbOrFunc( table, str, scope )->type;
	}
	return result;
}

struct PType* verifyFuncInvoke( char *id, struct expr_sem *exprPtr, struct SymTable *table, int scope ) {
	__BOOLEAN result = __TRUE;
    struct SymNode *target;
	// first check loop variable(s)
    if( ( target = lookupSymbOrFunc( table, id, scope ) ) == 0 ) {	// only search current scope
        fprintf( stdout, "<Error> found in Line %d: function '%s' had not been declared\n", linenum, id );
        result =  __FALSE;
    }
    else {
        if( verifyFuncInvokeAttr( target, exprPtr, table, scope ) ){
            result = __TRUE;
        }
        else {
            result = __FALSE;
        }
    }
    return target;
}

__BOOLEAN verifyFuncInvokeAttr( struct SymNode *target, struct expr_sem *exprList, struct SymTable *table, int scope ) {
	struct expr_sem *exprPtr;
    struct PTypeList *targetPtr;
	struct PType *typePtr, *targetType;
    if ( target->attribute->formalParam->paramNum != 0 ) {
        for( targetPtr=target->attribute->formalParam->params ; (targetPtr->next)!=0; targetPtr=(targetPtr->next) ) {
            targetType = targetPtr->value;
            // determine whether a const or a var
            if ( exprList->isDeref ) {
                typePtr = exprList->pType;
            }
            else {
                typePtr = getDeclarationType( table, exprList->varRef->id, scope );
            }
            // Compare type
            if( typePtr->type != targetType->type ) {
                if( targetType->type == REAL_t ) {
                    if( typePtr->type != INTEGER_t || typePtr->type != REAL_t ) {
                        fprintf( stdout, "<Error> found in Line %d: parameter type mismatch\n", linenum );
                    }
                }
                else {
                    fprintf( stdout, "<Error> found in Line %d: parameter type mismatch\n", linenum );
                }
                return __FALSE;
                break;
            }
            else {
                if( (targetPtr->next)!=0 ) {
                    if( (exprList->next) == 0) {
                        fprintf( stdout, "<Error> found in Line %d: too few arguments to function '%s'\n", linenum, target->name );
                    }
                }
                else if( (targetPtr->next)!=0 ) {
                    if( (exprList->next) != 0) {
                        fprintf( stdout, "<Error> found in Line %d: too few arguments to function '%s'\n", linenum, target->name );
                    }
                }
            }
        }
    }
    else { // if empty params
        if ( exprList != 0 ) {
            fprintf( stdout, "<Error> found in Line %d: too few arguments to function '%s'\n", linenum, target->name );
        }
    }
    return __TRUE;
}

char *getTypeString( struct PType* ttype ) {
    SEMTYPE type = ttype->type;
    char *result;
    result = malloc(sizeof(char));
    if( type == VOID_t ) {
        strcpy(result, "void");
    }
    else if( type == INTEGER_t ) {
        strcpy(result, "integer");
    }
    else if( type == BOOLEAN_t ) {
        strcpy(result, "boolean");
    }
    else if( type == REAL_t ) {
        strcpy(result, "real");
    }
    return result;
}

void verifyArrayIndex( struct SymTable *table, struct expr_sem *exprs, int scope ) {
    if( exprs->isDeref ) {
        if( exprs->pType->type != INTEGER_t ) {
            fprintf( stdout, "<Error> found in Line %d: each index of array references must be an integer.\n", linenum );
        }
    }
    else {
        if( verifyVarDeclaration( table, exprs, scope ) ) {
            if( getDeclarationType( table, exprs->varRef->id, scope )->type != INTEGER_t ) {
                fprintf( stdout, "<Error> found in Line %d: each index of array references must be an integer.\n", linenum );
            }
        }
    }
}

struct PType* checkArithmetic( struct SymTable *table, struct expr_sem *varA, struct expr_sem *varB, int scope) {
    struct PType *varTypeA, *varTypeB;
    __BOOLEAN flagA = __TRUE;
    __BOOLEAN flagB = __TRUE;
    char varNameA[200], varNameB[200];
    if( varA->isDeref ) {
        varTypeA = varA->pType;
    }
    else {
        strcpy(varNameA, varA->varRef->id);
        if( verifyVarDeclaration(table, varA, scope) ) {
            varTypeA = getDeclarationType( table, varNameA, scope );
        }
        else {
            flagA = __FALSE;
        }
    }

    if( varB->isDeref ) {
        varTypeB = varB->pType;
    }
    else {
        strcpy(varNameB, varB->varRef->id);
        if( verifyVarDeclaration(table, varB, scope) ) {
            varTypeB = getDeclarationType( table, varNameB, scope );
        }
        else {
            flagB = __FALSE;
        }
    }

    if( flagA && flagB ) {
        if( (varTypeA->type == INTEGER_t || varTypeA->type == REAL_t) && (varTypeB->type == INTEGER_t || varTypeB->type == REAL_t) ) {
            if( varTypeA->type == INTEGER_t && varTypeB->type == INTEGER_t ) {
                return varTypeA;
            }
            else if( varTypeA->type == REAL_t ) {
                return varTypeA;
            }
            else {
                return varTypeB;
            }
        }
        else {
	        printf("<Error> found in Line %d: for an arithmetic operotor, the operands must be integer or real types\n", linenum);
			struct PType *tt = createPType( VOID_t );
            tt->isError = __TRUE;
            return tt;
        }
    }
}

struct PType* checkAddArithmetic( struct SymTable *table, struct expr_sem *varA, struct expr_sem *varB, int scope) {
    struct PType *varTypeA, *varTypeB;
    __BOOLEAN flagA = __TRUE;
    __BOOLEAN flagB = __TRUE;
    char varNameA[200], varNameB[200];
    if( varA->isDeref ) {
        varTypeA = varA->pType;
    }
    else {
        strcpy(varNameA, varA->varRef->id);
        if( verifyVarDeclaration(table, varA, scope) ) {
            varTypeA = getDeclarationType( table, varNameA, scope );
        }
        else {
            flagA = __FALSE;
        }
    }

    if( varB->isDeref ) {
        varTypeB = varB->pType;
    }
    else {
        strcpy(varNameB, varB->varRef->id);
        if( verifyVarDeclaration(table, varB, scope) ) {
            varTypeB = getDeclarationType( table, varNameB, scope );
        }
        else {
            flagB = __FALSE;
        }
    }

    if( flagA && flagB ) {
        if( (varTypeA->type == INTEGER_t || varTypeA->type == REAL_t) && (varTypeB->type == INTEGER_t || varTypeB->type == REAL_t) ) {
            if( varTypeA->type == INTEGER_t && varTypeB->type == INTEGER_t ) {
                return varTypeA;
            }
            else if( varTypeA->type == REAL_t ) {
                return varTypeA;
            }
            else {
                return varTypeB;
            }
        }
        else if( varTypeA->type == STRING_t  && varTypeB->type == STRING_t ) {
            return varTypeA;
        }
        else {
	        printf("<Error> found in Line %d: for an arithmetic operotor, the operands must be integer or real types\n", linenum);
			struct PType *tt = createPType( VOID_t );
            return tt;
        }
    }
}

void checkReturnType( struct SymTable *table, struct expr_sem *exprs, struct PType *funcType, int scope ) {
    struct PType* result;
    __BOOLEAN flag = __TRUE;
    if( exprs->isDeref ) {
        result = exprs->pType;
    }
    else if( verifyVarDeclaration( table, exprs, scope ) ) {
        result = getDeclarationType( table, exprs->varRef->id, scope );
        // perform array check
        if( funcType->isArray ) {
        }
    }
    else {
        flag = __FALSE;
    }

    if( flag ) {
        if( result->type != funcType->type ) {
            fprintf( stdout, "<Error> found in Line %d: return type mismatch, function= %s, return= %s\n", linenum, getTypeString( result ), funcType->type );
        }
    }
}

struct PType* getRemainArray( struct SymTable *table, struct expr_sem *exprPtr, int scope ) {
    struct PType *orid, *result;
	struct ArrayDimNode *myDim;

    result = createPType( exprPtr->pType->type );
    orid = getDeclarationType( table, exprPtr->varRef->id, scope );
    if( orid->dimNum < exprPtr->pType->dimNum ) {
        result->isError = __TRUE;
        result->isArray = __TRUE;
    }
    else if( orid->dimNum == exprPtr->pType->dimNum ) {
        result->isError = __FALSE;
        result->isArray = __FALSE;
    }
    else {
        result->isArray = __TRUE;
        result->dimNum = orid->dimNum - exprPtr->pType->dimNum;
        int tmp = exprPtr->pType->dimNum;
        myDim = orid->dim;
        while( tmp > 0 ) {
            myDim = myDim->next;
            tmp--;
        }
        result->dim = myDim;
    }
    return result;
}
