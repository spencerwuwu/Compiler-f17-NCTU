#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "semcheck.h"
#include "symtab.h"
#include "codegen.h"
#include <dirent.h>

extern int wrt_fp;
extern char wrt_name[256];
extern char fileName[256];
extern __BOOLEAN i2f_flag;
extern __BOOLEAN oper_onstack;
extern __BOOLEAN read_flag;
extern __BOOLEAN i2f_ed;

void writeline( char* line ) {
    if( wrt_fp > 0 ) {
        write( wrt_fp, line, strlen(line));
    }
}

void stop_outputing() {
    if( wrt_fp > 0 ) {
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
        wrt_fp = -1;
    }
}

void fileName_to_codegenName( ) {
    char *ptr, *ptr_n;
    ptr = fileName;
    ptr_n = wrt_name;
    while( *ptr!='\0' ) {
        *ptr_n = *ptr;
        ptr++;
        ptr_n++;
    }
    *ptr_n++ = '.';
    *ptr_n++ = 'j';
    *ptr_n++ = '\0';
}

void init_codegen() {
    char tmp[300];
    sprintf( tmp, "; %s\n", wrt_name);
    writeline( tmp );
    sprintf( tmp, ".class public %s\n.super java/lang/Object\n.field public static _sc Ljava/util/Scanner;\n", fileName);
    writeline( tmp );
}

void variable_decl( int scope, char *name, struct PType *type, struct SymNode *newNode ) {
    char tmp[300];
    if( scope == 0 ) {
        sprintf( tmp, ".field public static %s ", name);
        switch( type->type ) {
            case VOID_t:
                strcat( tmp, "V\n" );
                break;
            case INTEGER_t:
                strcat( tmp, "I\n" );
                break;
            case BOOLEAN_t:
                strcat( tmp, "Z\n" );
                break;
            case REAL_t:
                strcat( tmp, "F\n" );
                break;
            case STRING_t:
                strcat( tmp, "S\n" );
                break;
        }
        write( wrt_fp, tmp, strlen(tmp) );
    }
    else {
        sprintf( tmp, "%s = %d, next number %d\n",  newNode->name, newNode->local_num, newNode->local_num+1);
    }
}

void enter_block( int var_tmp ) {
    char tmp[300];
    sprintf( tmp, "entering block, next number %d\n", var_tmp+1);
    //write( wrt_fp, tmp, strlen(tmp) );
}

void leave_block( struct SymTable *table, int __scope ) {
    char tmp[300];
    struct SymNode *ptr;
    int i;
    sprintf( tmp, "leaving block, symbol table entries:\n");
    //write( wrt_fp, tmp, strlen(tmp) );

    for (i = 0; i < HASHBUNCH; i++) {
        for (ptr = (table->entry[0]); ptr != 0; ptr = (ptr->next)) {
            if (ptr->scope == __scope) {
                sprintf( tmp, "<\"%s\", variable, ", ptr->name);
                switch( ptr->type->type ) {
                    case VOID_t:
                        strcat( tmp, "void," );
                        break;
                    case INTEGER_t:
                        strcat( tmp, "integer, " );
                        break;
                    case BOOLEAN_t:
                        strcat( tmp, "boolean, " );
                        break;
                    case REAL_t:
                        strcat( tmp, "real, " );
                        break;
                    case STRING_t:
                        strcat( tmp, "string, " );
                        break;
                }
                char tmp_tail[100];
                sprintf( tmp_tail, "%d>\n", ptr->local_num );
                strcat( tmp, tmp_tail );
                //write( wrt_fp, tmp, strlen(tmp) );
            }
        }
    }
}

struct SymNode *getLocalDecl(struct SymTable *table, const char *id, int scope ) {
    int index = hashFunc(id);

    struct SymNode *nodePtr;
    for (nodePtr = (table->entry[index]); nodePtr != NULL; nodePtr = (nodePtr->next))
    {
        if (((nodePtr->scope) == scope) && !strcmp(nodePtr->name, id))
        {
            return nodePtr;
        }
    }
    while( scope > 1 ) {
        if( (nodePtr = lookupSymbol(table, id, scope - 1, __FALSE)) != NULL ) {
            return  nodePtr;
        }
        scope--;
    }
    return NULL;
}

struct SymNode *getGlobalDecl(struct SymTable *table, const char *id, int scope ) {
    int index = hashFunc(id);

    struct SymNode *nodePtr;
    for (nodePtr = (table->entry[index]); nodePtr != NULL; nodePtr = (nodePtr->next))
    {
        if (((nodePtr->scope) == scope) && !strcmp(nodePtr->name, id))
        {
            return nodePtr;
        }
    }
    while( scope >= 1 ) {
        if( (nodePtr = lookupSymbol(table, id, scope - 1, __FALSE)) != NULL ) {
            return  nodePtr;
        }
        scope--;
    }
    return NULL;
}

int getNoneuseReg(struct SymTable *table, int scope ) {
	struct SymNode *nodePtr;
    int num = 1;
	for( nodePtr = (table->entry[0]); nodePtr != NULL; nodePtr = (nodePtr->next) ) {
		if ( nodePtr->scope == scope ) {
            num++;
		}
	}
    return num;
}


void arith_codegen( struct SymTable *table, struct expr_sem *op1, struct expr_sem *op2, OPERATOR operator, int scope ) {
    char tmp[300];
    struct SymNode *ptr;
    __BOOLEAN i2f = __FALSE;
    int operType = 0;
    if( op1->pType->type == REAL_t || op2->pType->type == REAL_t ) {
        i2f = __TRUE;
        operType = 2;
    }
    else {
        operType = 1;
    }

    if( op1->pType->type == REAL_t && op2->pType->type == INTEGER_t ) {
        sprintf( tmp, "\ti2f\n");
        writeline( tmp );
        i2f_ed = __TRUE;
    }
    else if( i2f_flag && op2->pType->type == REAL_t ) {
        int num = getNoneuseReg( table, scope );
        sprintf( tmp, "\tfstore %d\n", num);
        writeline( tmp );
        sprintf( tmp, "\ti2f\n");
        writeline( tmp );
        sprintf( tmp, "\tfload %d\n", num);
        writeline( tmp );
        i2f_flag = __FALSE;
        i2f_ed = __TRUE;
    }

    switch( operator ) {
        case ADD_t:
            if( operType == 1 )
                sprintf( tmp, "\tiadd\n");
            else
                sprintf( tmp, "\tfadd\n");
            break;
        case SUB_t:
            if( operType == 1 )
                sprintf( tmp, "\tisub\n");
            else
                sprintf( tmp, "\tfsub\n");
            break;
        case MUL_t:
            if( operType == 1 )
                sprintf( tmp, "\timul\n");
            else
                sprintf( tmp, "\tfmul\n");
            break;
        case DIV_t:
            if( operType == 1 )
                sprintf( tmp, "\tidiv\n");
            else
                sprintf( tmp, "\tfdiv\n");
            break;
        case MOD_t:
            if( operType == 1 )
                sprintf( tmp, "\tirem\n");
            else
                sprintf( tmp, "\tfrem\n");
            break;
        case AND_t:
            sprintf( tmp, "\tiand\n");
            break;
        case OR_t:
            sprintf( tmp, "\tior\n");
            break;
    }
    writeline( tmp );
}

void var_codegen( struct SymTable *table, struct expr_sem *op1, int scope ) {
    char tmp[300];
    struct SymNode *ptr;
    int operType = 0;
    if( read_flag ) {
        read_flag = __FALSE;
        return;
    }
    if( op1->isDeref==__TRUE ) {
        if( op1->pType->type == STRING_t ) {
            sprintf( tmp, "\tldc \"%s\"\n", op1->const_value->value);
            writeline( tmp );
        }
        else if( op1->pType->type == INTEGER_t ) {
                sprintf( tmp, "\tldc %d\n", op1->const_value->value);
            writeline( tmp );
        }
        else if( op1->pType->type == REAL_t ) {

                sprintf( tmp, "\tldc %f\n", op1->const_value->value.realVal);
            writeline( tmp );
        }
        else if( op1->pType->type == BOOLEAN_t ) {
            sprintf( tmp, "\ticonst_%d\n", op1->const_value->value);
            writeline( tmp );
        }
    }
    else {
        if( (ptr = getLocalDecl( table, op1->varRef->id, scope )) != NULL ) {
            if( ptr->category == CONSTANT_t ) {
                if( ptr->type->type == REAL_t ) {
                    sprintf( tmp, "\tldc %d\n", ptr->attribute->constVal->value.realVal);
                }
                else if ( ptr->type->type == STRING_t ) {
                    sprintf( tmp, "\tldc \"%s\"\n", ptr->attribute->constVal->value);
                }
                else {
                    sprintf( tmp, "\tldc %d\n", ptr->attribute->constVal->value);
                }
            }
            else {
                if( ptr->type->type == STRING_t ) {
                    sprintf( tmp, "\tldc \"%s\"\n", ptr->attribute->constVal->value);
                }
                else if( ptr->type->type == INTEGER_t ) {
                    sprintf( tmp, "\tiload %d\n", ptr->local_num);
                }
                else if( ptr->type->type == BOOLEAN_t ) {
                    sprintf( tmp, "\tiload %d\n", ptr->local_num);
                }
                else if( ptr->type->type == REAL_t ) {
                    sprintf( tmp, "\tfload %d\n", ptr->local_num);
                }
                else {
                    sprintf(tmp, "gg %s %d %d\n", ptr->name, ptr->local_num , ptr->category);
                }
            }
            writeline( tmp );
        }
        else if( (ptr = lookupLoopVar( table, op1->varRef->id)) != NULL ) {
            if( ptr->type->type == STRING_t ) {
                sprintf( tmp, "\tldc \"%s\"\n", ptr->attribute->constVal->value);
            }
            else if( ptr->type->type == INTEGER_t ) {
                sprintf( tmp, "\tiload %d\n", ptr->local_num);
            }
            else if( ptr->type->type == BOOLEAN_t ) {
                sprintf( tmp, "\tiload %d\n", ptr->local_num);
            }
            else if( ptr->type->type == REAL_t ) {
                sprintf( tmp, "\tfload %d\n", ptr->local_num);
            }
            else {
                sprintf(tmp, "gg %s %d %d\n", ptr->name, ptr->local_num , ptr->category);
            }
            writeline( tmp );
        }
        else if( (ptr = getGlobalDecl( table, op1->varRef->id, scope )) != NULL ){
            if( ptr->category == CONSTANT_t ) {
                if( ptr->type->type == REAL_t ) {
                    sprintf( tmp, "\tldc %d\n", ptr->attribute->constVal->value.realVal);
                }
                else if ( ptr->type->type == STRING_t ) {
                    sprintf( tmp, "\tldc \"%s\"\n", ptr->attribute->constVal->value);
                }
                else {
                    sprintf( tmp, "\tldc %d\n", ptr->attribute->constVal->value);
                }
            }
            else {
                if( ptr->type->type == STRING_t ) {
                    sprintf( tmp, "\tldc \"%s\"\n", ptr->attribute->constVal->value);
                }
                else if( ptr->type->type == INTEGER_t ) {
                    sprintf( tmp, "\tgetstatic %s/%s", fileName, ptr->name);
                    strcat( tmp, " I\n" );
                }
                else if( ptr->type->type == REAL_t ) {
                    sprintf( tmp, "\tgetstatic %s/%s", fileName, ptr->name);
                    strcat( tmp, " F\n" );
                }
                else {
                    sprintf(tmp, "gg %s %d %d\n", ptr->name, ptr->local_num , ptr->category);
                }
            }
            writeline( tmp );
        }
    }
}
void function_call_codegen(const char *id, struct expr_sem *exprList, struct SymTable *table, int scope) {
    char tmp[300];
    struct SymNode *node = 0;
    struct PTypeList *funcAttr;
    struct expr_sem *exprPtr;
    node = lookupSymbol(table, id, 0, __FALSE); // function always in scope 0 (global scope)
    int i;

    sprintf( tmp, "\tinvokestatic %s/%s(", fileName, id );
    for( i = 0, funcAttr = node->attribute->formalParam->params; i < node->attribute->formalParam->paramNum; i++, funcAttr = funcAttr->next ) {
        switch( funcAttr->value->type ) {
            case STRING_t:
                strcat( tmp, "V" );
                break;
            case INTEGER_t:
                strcat( tmp, "I" );
                break;
            case REAL_t:
                strcat( tmp, "F" );
                break;
            case BOOLEAN_t:
                strcat( tmp, "I" );
                break;
            case VOID_t:
                strcat( tmp, "V" );
                break;
        }
    }
    strcat( tmp, ")" );
    switch( node->type->type ) {
        case STRING_t:
            strcat( tmp, "V\n" );
            break;
        case INTEGER_t:
            strcat( tmp, "I\n" );
            break;
        case REAL_t:
            strcat( tmp, "F\n" );
            break;
        case BOOLEAN_t:
            strcat( tmp, "Z\n" );
            break;
        case VOID_t:
            strcat( tmp, "\n" );
            break;
    }
    writeline( tmp );
}

void function_heading_codegen( const char *id, struct param_sem *params, struct PType *retType ) {
    char tmp[300];
    sprintf( tmp, ".method public static %s(", id );

    if (params != NULL) {
        struct param_sem *parPtr;
        struct idNode_sem *idPtr;

        for (parPtr = params; parPtr != NULL; parPtr = (parPtr->next)) {
            for (idPtr = (parPtr->idlist); idPtr != NULL; idPtr = (idPtr->next)) {
                switch( parPtr->pType->type ) {
                    case STRING_t:
                        //strcat( tmp, "V" );
                        //break;
                    case INTEGER_t:
                        strcat( tmp, "I" );
                        break;
                    case REAL_t:
                        strcat( tmp, "F" );
                        break;
                    case BOOLEAN_t:
                        strcat( tmp, "Z" );
                        break;
                    case VOID_t:
                        strcat( tmp, "V" );
                        break;
                }
            }
        }
    }

    strcat( tmp, ")" );
    switch( retType->type ) {
        case STRING_t:
            strcat( tmp, "V\n" );
            break;
        case INTEGER_t:
            strcat( tmp, "I\n" );
            break;
        case REAL_t:
            strcat( tmp, "F\n" );
            break;
        case BOOLEAN_t:
            strcat( tmp, "Z\n" );
            break;
        case VOID_t:
            strcat( tmp, "V\n" );
            break;
    }
    writeline( tmp );

    sprintf( tmp, "\t.limit stack 100\n\t.limit locals 100\n" );
    writeline( tmp );

}
void function_ending_codegen( struct PType *retType ) {
    char tmp[300];
    if( retType->type == VOID_t ) {
        sprintf( tmp, "\treturn\n" );
        writeline( tmp );
    }
    sprintf( tmp, ".end method\n\n" );
    writeline( tmp );
}

void return_codegen(struct expr_sem *expr, struct PType *funcReturn) {
    char tmp[300];
    if (funcReturn->type == VOID_t) {
        sprintf( tmp, "\treturn\n" );
    }
    else if (funcReturn->type == INTEGER_t) {
        sprintf( tmp, "\tireturn\n" );
    }
    else if (funcReturn->type == REAL_t) {
        sprintf( tmp, "\tfreturn\n" );
    }
    writeline( tmp );
}


void assign_codegen( struct SymTable *table, struct expr_sem *lhs, struct expr_sem *rhs, int scope ) {
    char tmp[300];
    struct SymNode *ptr;
    if( !i2f_ed && lhs->pType->type == REAL_t && rhs->pType->type == INTEGER_t ) {
        sprintf( tmp, "i2f\n" );
        writeline( tmp );
    }
    i2f_ed = __FALSE;
    if( (ptr = getLocalDecl( table, lhs->varRef->id, scope )) != NULL ) {
        if( ptr->category == STRING_t ) {
        }
        else if( ptr->type->type == INTEGER_t ) {
            sprintf( tmp, "\tistore %d\n", ptr->local_num);
        }
        else if( ptr->type->type == REAL_t ) {
            sprintf( tmp, "\tfstore %d\n", ptr->local_num);
        }
        else if( ptr->type->type == BOOLEAN_t ) {
            sprintf( tmp, "\tistore %d\n", ptr->local_num);
        }
        writeline( tmp );
    }
}

void print_heading_codegen() {
    char tmp[300];
    struct SymNode *ptr;
    sprintf( tmp, "\tgetstatic java/lang/System/out Ljava/io/PrintStream;\n" );
    writeline( tmp );
}

void print_codegen( struct SymTable *table, struct expr_sem *rhs, int scope ) {
    char tmp[300];
    struct SymNode *ptr;

    if( !rhs->isDeref ) {
        if( (ptr = getLocalDecl( table, rhs->varRef->id, scope )) != NULL ) {
        }
        else if( (ptr = lookupLoopVar( table, rhs->varRef->id )) != NULL ) {
        }
        else if( (ptr = getGlobalDecl( table, rhs->varRef->id, scope )) != NULL ){
            sprintf( tmp, "\tgetstatic %s/%s", fileName, ptr->name);
            writeline( tmp );
        }
        if( ptr->type->type == STRING_t ) {
            sprintf( tmp, "\tinvokevirtual java/io/PrintStream/print(Ljava/lang/String;)V\n");
            writeline( tmp );
        }
        else if( ptr->type->type == INTEGER_t || ptr->type->type == BOOLEAN_t ) {
            sprintf( tmp, "\tinvokevirtual java/io/PrintStream/print(I)V\n");
            writeline( tmp );
        }
        else if( ptr->type->type == REAL_t ) {
            sprintf( tmp, "\tinvokevirtual java/io/PrintStream/print(F)V\n");
            writeline( tmp );
        }
    }
    else {
        if( rhs->pType->type == STRING_t ) {
            sprintf( tmp, "\tinvokevirtual java/io/PrintStream/print(Ljava/lang/String;)V\n");
            writeline( tmp );
        }
        else if( rhs->pType->type == INTEGER_t || rhs->pType->type == BOOLEAN_t ) {
            sprintf( tmp, "\tinvokevirtual java/io/PrintStream/print(I)V\n");
            writeline( tmp );
        }
        else if( rhs->pType->type == REAL_t ) {
            sprintf( tmp, "\tinvokevirtual java/io/PrintStream/print(F)V\n");
            writeline( tmp );
        }
    }

}

struct LabelNode *createLabel( int num, char* type, __BOOLEAN condition ) {
    struct LabelNode *newNode = (struct LabelNode *)malloc(sizeof(struct LabelNode));
    newNode->Label_start = malloc(sizeof(char));
    newNode->Label_true = malloc(sizeof(char));
    newNode->Label_false = malloc(sizeof(char));
    newNode->Label_else = malloc(sizeof(char));
    newNode->Label_exit = malloc(sizeof(char));
    sprintf( newNode->Label_start, "L%s_%d", type, num );
    sprintf( newNode->Label_true, "Ltrue_%d", num );
    sprintf( newNode->Label_false, "Lfalse_%d", num );
    sprintf( newNode->Label_else, "Lelse_%d", num );
    sprintf( newNode->Label_exit, "Lexit_%d", num );
    newNode->prev = 0;
    newNode->next = 0;
    newNode->Be_condition = condition;

    return newNode;
}

void insertLabel(struct LabelTable *table, struct LabelNode *newNode) {
    struct LabelNode *nodePtr;
    char tmp[300];
    if( table->entry != 0 ) {
        for (nodePtr = table->entry; (nodePtr->next) != 0; nodePtr = nodePtr->next)
            ;
        nodePtr->next = newNode;
        newNode->prev = nodePtr;
    }
    else {
        table->entry = newNode;
    }

}

void write_label_starter( struct LabelTable *table ) {
    char tmp[300];
    sprintf( tmp, "%s:\n", getStartLabel( table ) );
    writeline( tmp ) ;
}

struct LabelNode * getCurrentLabel( struct LabelTable *table ) {
    struct LabelNode *nodePtr;
    for (nodePtr = table->entry; (nodePtr->next) != 0; nodePtr = nodePtr->next)
        ;
    return nodePtr;
}

char* getStartLabel( struct LabelTable *table ) {
    struct LabelNode *nodePtr;
    for (nodePtr = table->entry; (nodePtr->next) != 0; nodePtr = nodePtr->next)
        ;
    return nodePtr->Label_start;
}

char* getTrueLabel( struct LabelTable *table ) {
    struct LabelNode *nodePtr;
    for (nodePtr = table->entry; (nodePtr->next) != 0; nodePtr = nodePtr->next)
        ;
    return nodePtr->Label_true;
}

char* getFalseLabel( struct LabelTable *table ) {
    struct LabelNode *nodePtr;
    for (nodePtr = table->entry; (nodePtr->next) != 0; nodePtr = nodePtr->next)
        ;
    return nodePtr->Label_false;
}

char* getElseLabel( struct LabelTable *table ) {
    struct LabelNode *nodePtr;
    for (nodePtr = table->entry; (nodePtr->next) != 0; nodePtr = nodePtr->next)
        ;
    return nodePtr->Label_else;
}

char* getExitLabel( struct LabelTable *table ) {
    struct LabelNode *nodePtr;
    for (nodePtr = table->entry; (nodePtr->next) != 0; nodePtr = nodePtr->next)
        ;
    return nodePtr->Label_exit;
}

void removeLabel( struct LabelTable *table ) {
    char tmp[300];
    struct LabelNode *nodePtr;
    for (nodePtr = table->entry; (nodePtr->next) != 0; nodePtr = nodePtr->next)
        ;
    sprintf( tmp, "%s:\n", nodePtr->Label_exit);
    writeline( tmp );

    if( nodePtr->prev != 0 ) {
        struct LabelNode *nodePre = nodePtr->prev;
        nodePre->next = 0;
    }
    else {
        table->entry = 0;
    }
    free( nodePtr );
}

void ifwhile_heading_codegen( struct LabelTable *table ) {
    char tmp[300];
    sprintf( tmp, "\tifeq %s\n", getElseLabel( table ) );
    writeline( tmp );
}

void if_ending_codegen( struct LabelTable *table ) {
    char tmp[300];
    sprintf( tmp, "\tgoto %s\n", getExitLabel( table ) );
    writeline( tmp );
}

void else_codegen( struct LabelTable *table ) {
    char tmp[300];
    sprintf( tmp, "%s:\n", getElseLabel( table ) );
    writeline( tmp );
}

void rel_op_codegen( struct SymTable *table, struct expr_sem *op1, struct expr_sem *op2, OPERATOR operator, int scope, struct LabelNode *node ) {
    char tmp[300];
    struct SymNode *ptr;
    __BOOLEAN i2f = __FALSE;
    int operType = 0;
    if( op1->pType->type == REAL_t || op2->pType->type == REAL_t ) {
        i2f = __TRUE;
        operType = 2;
    }
    else {
        operType = 1;
    }

    if( op1->pType->type == REAL_t && op2->pType->type == INTEGER_t ) {
        sprintf( tmp, "\ti2f\n");
        writeline( tmp );
        i2f_ed = __TRUE;
    }
    else if( op2->pType->type == REAL_t && op1->pType->type == INTEGER_t ) {
        int num = getNoneuseReg( table, scope );
        sprintf( tmp, "\tfstore %d\n", num);
        writeline( tmp );
        sprintf( tmp, "\ti2f\n");
        writeline( tmp );
        sprintf( tmp, "\tfload %d\n", num);
        writeline( tmp );
        i2f_ed = __TRUE;
    }

    if( operType == 1 )
        sprintf( tmp, "\tisub\n");
    else
        sprintf( tmp, "\tfsub\n");
    writeline( tmp );

    switch( operator ) {
        case LT_t:
            sprintf( tmp, "\tiflt ");
            break;
        case LE_t:
            sprintf( tmp, "\tifle ");
            break;
        case EQ_t:
            sprintf( tmp, "\tifeq ");
            break;
        case GE_t:
            sprintf( tmp, "\tifge ");
            break;
        case GT_t:
            sprintf( tmp, "\tifgt ");
            break;
        case NE_t:
            sprintf( tmp, "\tifne ");
            break;
    }
    writeline( tmp );

    sprintf( tmp, "%s\n\ticonst_0\n\tgoto %s\n", node->Label_true, node->Label_false );
    writeline( tmp );

    sprintf( tmp, "%s:\n\ticonst_1\n%s:\n", node->Label_true, node->Label_false );
    writeline( tmp );

}

void int_codegen( int value ) {
    char tmp[300];
    sprintf( tmp, "\tldc %d\n", value );
    writeline( tmp );
}

void for_heading_codegen( struct LabelTable *table, char *id, int lo, int hi, struct SymTable *stable ) {
    char tmp[300];
    char *start_l = getTrueLabel(table);
    char *true_l = getTrueLabel(table);
    char *false_l = getFalseLabel(table);
    char *exit_l = getExitLabel(table);
    int num = lookupLoopVar( stable, id )->local_num;
    int_codegen( lo );
    sprintf( tmp , "\tistore %d\n", num );
    writeline( tmp );

    write_label_starter( table );
    sprintf( tmp , "\tiload %d\n", num );
    writeline( tmp );
    int_codegen( hi );

    sprintf( tmp, "\tisub\n\tifle %s\n\ticonst_0\n\tgoto %s\n%s:\n\ticonst_1\n%s:\n\tifeq %s\n", start_l, false_l, true_l, false_l, exit_l );
    writeline( tmp );
}

void for_ending_codegen( struct LabelTable *table, char *id, struct SymTable *stable ) {
    char tmp[300];
    char *start_l = getStartLabel(table);
    char *exit_l = getExitLabel(table);
    int num = lookupLoopVar( stable, id )->local_num;

    sprintf( tmp, "\tiload %d\n\tsipush 1\n\tiadd\n\tistore %d\n\tgoto %s\n%s:\n", num, num, start_l, exit_l );
    writeline( tmp );
}

void while_ending_codegen( struct LabelTable *table ) {
    char tmp[300];
    char *start_l = getStartLabel(table);
    char *else_l = getElseLabel(table);
    char *exit_l = getExitLabel(table);

    sprintf( tmp, "\tgoto %s\n%s:\n%s:\n", start_l, else_l, exit_l );
    writeline( tmp );
}

void read_codegen( struct SymTable *table, struct expr_sem *rhs, int scope ) {
    char tmp[300];
    struct SymNode *ptr;

    sprintf( tmp, "\t; invoke java.util.Scanner.nextXXX()\n\tgetstatic %s/_sc Ljava/util/Scanner;\n", fileName );
    writeline( tmp );

    if( !rhs->isDeref ) {
        if( (ptr = getLocalDecl( table, rhs->varRef->id, scope )) != NULL ) {
        }
        else if( (ptr = getGlobalDecl( table, rhs->varRef->id, scope )) != NULL ){
            sprintf( tmp, "\tgetstatic %s/%s", fileName, ptr->name);
            writeline( tmp );
        }
        if( ptr->type->type == STRING_t ) {
            sprintf( tmp, "\tinvokevirtual java/util/Scanner/nextLine()V\n");
            writeline( tmp );
        }
        else if( ptr->type->type == INTEGER_t || ptr->type->type == BOOLEAN_t ) {
            sprintf( tmp, "\tinvokevirtual java/util/Scanner/nextInt()I\n");
            writeline( tmp );
        }
        else if( ptr->type->type == REAL_t ) {
            sprintf( tmp, "\tinvokevirtual java/util/Scanner/nextFloat()F\n");
            writeline( tmp );
        }
    }
    else {
        if( rhs->pType->type == STRING_t ) {
            sprintf( tmp, "\tinvokevirtual java/util/Scanner/nextLine()V\n");
            writeline( tmp );
        }
        else if( rhs->pType->type == INTEGER_t || rhs->pType->type == BOOLEAN_t ) {
            sprintf( tmp, "\tinvokevirtual java/util/Scanner/nextInt()I\n");
            writeline( tmp );
        }
        else if( rhs->pType->type == REAL_t ) {
            sprintf( tmp, "\tinvokevirtual java/util/Scanner/nextFloat()F\n");
            writeline( tmp );
        }
    }

    if( (ptr = getLocalDecl( table, rhs->varRef->id, scope )) != NULL ) {
        if( ptr->category == STRING_t ) {
        }
        else if( ptr->type->type == INTEGER_t ) {
            sprintf( tmp, "\tistore %d\n", ptr->local_num);
        }
        else if( ptr->type->type == REAL_t ) {
            sprintf( tmp, "\tfstore %d\n", ptr->local_num);
        }
        else if( ptr->type->type == BOOLEAN_t ) {
            sprintf( tmp, "\tistore %d\n", ptr->local_num);
        }
        writeline( tmp );
    }

}

void neg_codegen( struct expr_sem *ptr ) {
    char tmp[100];
    if( ptr->pType->type == REAL_t ) {
        sprintf( tmp, "\tfneg\n" );
    }
    else {
        sprintf( tmp, "\tineg\n" );
    }

    writeline( tmp );
}
