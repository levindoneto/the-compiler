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
    if (!node) return;

    int i = 0;
    int type;

    switch(node->type) {
        case AST_FUNCTIONDECLARATION:
	    printf("TODO");
        default:
            printf("Default case");
    }
}

int isBool(AST* node){
	if (!node) return 0;

	if (node->type == AST_PARENTHESIS) return isBool(node->son[0]);

	if (	node->type == AST_LESS							||
		node->type == AST_GREATER						||
		node->type == AST_AND							||
		node->type == AST_OR							||
		node->type == AST_NOT							||
		node->type == AST_LE							||
		node->type == AST_GE							||
		node->type == AST_EQ							||
		node->type == AST_DIFF							||
		node->type == AST_NOT							||

		(node->type 		== AST_SYMBOL 				&&
		 (node->symbol->type 		==	SYMBOL_SCALAR	||
		  node->symbol->type 		==	SYMBOL_VECTOR	||
		  node->symbol->type 		==	SYMBOL_FUNCTION)	&&
		 node->symbol->datatype == DATATYPE_BOOL)				||

		(node->type 		== AST_SYMBOL 				&&
		 node->symbol->type 	== SYMBOL_LITBOOL)				||

		(node->type 		== AST_VECTORPOS			&&
		 node->symbol->type 	== SYMBOL_VECTOR			&&
		 node->symbol->datatype == DATATYPE_BOOL)				||

		(node->type		== AST_FUNCTIONCALL			&&
		 node->symbol->type 	== SYMBOL_FUNCTION			&&
		 node->symbol->datatype == DATATYPE_BOOL)
		) return 1;

	return 0;
}

int isNotBool(AST* node){
	if (!node) return 0;

	if (node->type == AST_PARENTHESIS) return isNotBool(node->son[0]);

	if (	node->type == AST_ADD							||
		node->type == AST_SUB							||
		node->type == AST_MUL							||
		node->type == AST_DIV							||

	 	 (node->type 		== AST_SYMBOL 				&&
		 (node->symbol->type 		==	SYMBOL_SCALAR	||
		  node->symbol->type 		==	SYMBOL_VECTOR	||
		  node->symbol->type 		==	SYMBOL_FUNCTION)	&&
		  (node->symbol->datatype == DATATYPE_INT		||
		  node->symbol->datatype == DATATYPE_FLOAT		||
		  node->symbol->datatype == DATATYPE_LONG))				||

		(node->type 			== AST_SYMBOL 			&&
		 (node->symbol->type 		== SYMBOL_LITINT	||
		  node->symbol->type 		== SYMBOL_LITREAL	||
		  node->symbol->type 		== SYMBOL_LITSTRING))			||

		(node->type 			== AST_VECTORPOS		&&
		 node->symbol->type 		== SYMBOL_VECTOR		&&
		 (node->symbol->datatype == DATATYPE_INT		||
		  node->symbol->datatype == DATATYPE_FLOAT		||
		  node->symbol->datatype == DATATYPE_LONG))				||

		(node->type			== AST_FUNCTIONCALL		&&
		 node->symbol->type 		== SYMBOL_FUNCTION		&&
		 (node->symbol->datatype == DATATYPE_INT		||
		  node->symbol->datatype == DATATYPE_FLOAT		||
		  node->symbol->datatype == DATATYPE_LONG))
		) return 1;
	return 0;
}

int confirmType(AST* node, int datatype){
	if (!node) return 0;

	if (datatype == DATATYPE_BOOL) return isBool(node->son[0]);
	else return isNotBool(node->son[0]);
}

int checkReturn(AST* node, int datatype){
	if (!node) return 0;

	if (node->son[0]->type == AST_RETURN) return confirmType(node->son[0], datatype);
	else return checkReturn(node->son[1], datatype);
}

int verifyReturn(AST* node){
	if (!node || !node->son[2] || node->son[2]->son[0]) return 0;

	if (node->son[2]->son[0]->type == AST_RETURN) return confirmType(node->son[2]->son[0], node->symbol->datatype);
	else return checkReturn(node->son[2]->son[0], node->symbol->datatype);
}


