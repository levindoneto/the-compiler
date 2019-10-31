#include "semantic.h"

int errorsSemantic = INIT;

void checkUndeclared(void){
    errorsSemantic += hashGetNumberUndeclared();
}

int getNumberErrorSemantic(void){
    return errorsSemantic;
}

void checkAndSetTypes(AST*node) {
    int i;
    if (!node) return;
    if(node->type == AST_DECLARATION || node->type == AST_VARIABLEDECLARATION || node->type == AST_FUNCTIONDECLARATION ||node->type == AST_VECTORDECLARATION || node->type == AST_PARAM || node->type == AST_PARAM) {
        if(node->symbol && node->symbol->type != AST_SYMBOL && node->type!= AST_PARAM) {
            fprintf(stderr, "Semantic ERROR: Symbol %s already declared. \n", node->symbol->value);
            errorsSemantic++;
        }
        if(node->type == AST_DECLARATION)
            node->symbol->type = SYMBOL_SCALAR;
        if(node->type == AST_PARAM)
            node->symbol->type = SYMBOL_SCALAR;
        if(node->type == AST_FUNCTIONDECLARATION)
            node->symbol->type = SYMBOL_FUNCTION;
        if(node->type == AST_VECTORDECLARATION)
            node->symbol->type = SYMBOL_VECTOR;

        if(node->type!= AST_PARAM) {
            if(node->son[0]->type==AST_BOOL)
                node->symbol->datatype = DATATYPE_BOOL;
            if(node->son[0]->type==AST_BYTE)
                node->symbol->datatype = DATATYPE_BYTE;
            if(node->son[0]->type == AST_INT)
                node->symbol->datatype = DATATYPE_INT;
            if(node->son[0]->type==AST_LONG)
                node->symbol->datatype = DATATYPE_LONG;
            if(node->son[0]->type == AST_FLOAT )
                node->symbol->datatype = DATATYPE_FLOAT;
        }
    }
    for (int i = 0; i < MAX_SONS; ++i) {
        checkAndSetTypes(node->son[i]);
    }
}


void checkOperands(AST* node) {
    printf("TODO");
}
