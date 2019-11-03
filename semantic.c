#include "semantic.h"

int errorsSemantic = INIT;

void checkUndeclared(void){
    errorsSemantic += hashGetNumberUndeclared();
}

int getNumberErrorSemantic(void){
    return errorsSemantic;
}

void checkAndSetTypes(AST*node) {
    int exp;
    if (!node) {
	return;
    }
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
            if(node->son[SON_LEFT]->type==AST_BOOL)
                node->symbol->datatype = DATATYPE_BOOL;
            if(node->son[SON_LEFT]->type==AST_BYTE)
                node->symbol->datatype = DATATYPE_BYTE;
            if(node->son[SON_LEFT]->type == AST_INT)
                node->symbol->datatype = DATATYPE_INT;
            if(node->son[SON_LEFT]->type==AST_LONG)
                node->symbol->datatype = DATATYPE_LONG;
            if(node->son[SON_LEFT]->type == AST_FLOAT )
                node->symbol->datatype = DATATYPE_FLOAT;
        }
    }
    for (exp = INIT; exp < MAX_SONS; ++exp) {
        checkAndSetTypes(node->son[exp]);
    }
}


void checkOperands(AST* node) {
    if (!node) {
	return;
    }
    int type;
    int exp; // expression iterator

    switch(node->type) {
        case AST_FUNCTIONDECLARATION:
	    printf("TODO");
	case AST_LESS:
	    for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isBool(node->son[exp])) {
                    errorsSemantic++;
                }
            }
            break;
        case AST_GREATER:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isBool(node->son[exp])) {
                    errorsSemantic++;
                }
            }
            break;
        case AST_LE:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isBool(node->son[exp])) {
                    errorsSemantic++;
                }
            }
            break;
        case AST_GE:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isBool(node->son[exp])) {
                    errorsSemantic++;
                }
            }
            break;
        case AST_NOT:
            if(isNotBool(node->son[SON_LEFT])){
                errorsSemantic++;
            }
            break;
        case AST_AND:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isNotBool(node->son[exp])) {
                    errorsSemantic++;
                }
            }
            break;
        case AST_OR:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isNotBool(node->son[exp])) {
                    errorsSemantic++;
                }
            }
            break;
        case AST_DIFF:
            if(isNotBool(node->son[SON_LEFT])!=isNotBool(node->son[SON_RIGHT])){
                errorsSemantic++;
            }
            if(isBool(node->son[SON_LEFT])!=isBool(node->son[SON_RIGHT])){
                errorsSemantic++;
            }
            break;
        case AST_EQ:
            if(isNotBool(node->son[SON_LEFT]) != isNotBool(node->son[SON_RIGHT])){
                errorsSemantic++;
            }
            if(isBool(node->son[SON_LEFT]) != isBool(node->son[SON_RIGHT])){
                errorsSemantic++;
            }
            break;
	case AST_SUB:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isBool(node->son[exp])){
                    errorsSemantic++;
                }
            }
            break;
        case AST_ADD:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isBool(node->son[exp])){
                    errorsSemantic++;
                }
            }
            break;
        case AST_MUL:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isBool(node->son[exp])){
                    errorsSemantic++;
                }
            }
            break;
        case AST_DIV: // deixei quieto para deixar o exemplo do sor ,depois muda
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isBool(node->son[exp])){
                    errorsSemantic++;
                }
            }
            break;
	case AST_SYMBOL:
            if((node->symbol->type == SYMBOL_FUNCTION|| node->symbol->type == SYMBOL_VECTOR)){
                errorsSemantic++;
            }
            break;
	// TODO: CHECK THE CASES BELOW <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        case AST_VARIABLEDECLARATION :
            if((node->symbol->type == SYMBOL_FUNCTION|| node->symbol->type == SYMBOL_VECTOR)) {
                errorsSemantic++;
            }
            if (isBool(node->son[SON_LEFT])) {
                if(node->symbol->datatype==DATATYPE_INT ||
                   node->symbol->datatype==DATATYPE_FLOAT ||
                   node->symbol->datatype==DATATYPE_LONG ||
                   node->symbol->datatype==DATATYPE_BYTE) {
                       errorsSemantic++;
                }
            }
            if (isNotBool(node->son[SON_LEFT])){
                if(node->symbol->datatype ==DATATYPE_BOOL) {
                    errorsSemantic++;
                }
            }
            break;
        case AST_VECTORDECLARATION:
            if(isBool(node->son[SON_LEFT])) { // index==bool
                errorsSemantic++;
            }
            if(node->symbol->type != SYMBOL_VECTOR) {
                errorsSemantic++;
            }
            break;
        default:
            printf("Default case");
    }
    for(exp = INIT; exp < MAX_SONS; exp++) {
        checkOperands(node->son[exp]);
    }
}

int isBool(AST* node){
	if (!node) return 0;

	if (node->type == AST_PARENTHESIS) return isBool(node->son[SON_LEFT]);

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

	if (node->type == AST_PARENTHESIS) return isNotBool(node->son[SON_LEFT]);

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

	if (datatype == DATATYPE_BOOL) return isBool(node->son[SON_LEFT]);
	else return isNotBool(node->son[SON_LEFT]);
}

int checkReturn(AST* node, int datatype){
	if (!node) return 0;

	if (node->son[SON_LEFT]->type == AST_RETURN) return confirmType(node->son[SON_LEFT], datatype);
	else return checkReturn(node->son[1], datatype);
}

int verifyReturn(AST* node){
	if (!node || !node->son[2] || node->son[2]->son[SON_LEFT]) return 0;

	if (node->son[2]->son[SON_LEFT]->type == AST_RETURN) return confirmType(node->son[2]->son[SON_LEFT], node->symbol->datatype);
	else return checkReturn(node->son[2]->son[SON_LEFT], node->symbol->datatype);
}


