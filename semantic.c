#include "semantic.h"

int errorsSemantic = INIT;
AST* rootNode = 0;

void checkUndeclared(void) {
    errorsSemantic += hashGetNumberUndeclared();
}
int getNumberErrorSemantic(void) {
    return errorsSemantic;
}
void checkAndSetTypes(AST*node) {
    int exp;
    if (!node) {
	return;
    }
    if (!rootNode){
	rootNode = node;
    }
    if( node->type == AST_DECLARATION ||
	node->type == AST_VARIABLEDECLARATION ||
	node->type == AST_FUNCTIONDECLARATION ||
	node->type == AST_VECTORDECLARATION ||
	node->type == AST_PARAM ) {
        if(node->symbol && node->symbol->type != SYMBOL_IDENTIFIER && node->type!= AST_PARAM) {
            fprintf(stderr, "SEMANTIC ERROR: Symbol %s already declared. LINE: %d\n", node->symbol->value, node->lineNumber);
            errorsSemantic++;
        }
	//declare symbol type and datatype
	switch (node->type) {
		case AST_VARIABLEDECLARATION:
        		node->symbol->type = SYMBOL_SCALAR;
		break;
		case AST_FUNCTIONDECLARATION:
        		node->symbol->type = SYMBOL_FUNCTION;
		break;
		case AST_VECTORDECLARATION:
        		node->symbol->type = SYMBOL_VECTOR;
		break;
		case AST_PARAM:
        		node->symbol->type = SYMBOL_SCALAR;
		break;
	}
	switch(node->son[SON_LEFT]->type) {
		case AST_BOOL:
                	node->symbol->datatype = DATATYPE_BOOL;
		break;
		case AST_BYTE:
                	node->symbol->datatype = DATATYPE_BYTE;
		break;
		case AST_INT:
                	node->symbol->datatype = DATATYPE_INT;
		break;
		case AST_LONG:
                	node->symbol->datatype = DATATYPE_LONG;
		break;
		case AST_FLOAT:
                	node->symbol->datatype = DATATYPE_FLOAT;
		break;
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
	// Item 1 - Declarações
	case AST_SYMBOL:
		if(node->symbol->type == SYMBOL_IDENTIFIER) {
        		fprintf(stderr, "SEMANTIC ERROR: Symbol %s has not been declared. LINE: %d\n", node->symbol->value, node->lineNumber);
            		errorsSemantic++;
		}
		if (node->symbol->type == SYMBOL_FUNCTION) {
        		fprintf(stderr, "SEMANTIC ERROR: Symbol %s is a function and should be followed by (). LINE: %d\n", node->symbol->value, node->lineNumber);
            		errorsSemantic++;
		}
		break;
	case AST_FOR:
		if (isNotBool(node->son[1]) || isNotBool(node->son[2])){
        		fprintf(stderr, "SEMANTIC ERROR: For conditions should be bool. LINE: %d\n", node->lineNumber);
            		errorsSemantic++;
		}
	case AST_ATTR:
		if (node->symbol->datatype != DATATYPE_BOOL && isBool(node->son[0])){
        		fprintf(stderr, "SEMANTIC ERROR: Symbol %s cannot receive boolean values. LINE: %d\n", node->symbol->value, node->lineNumber);
            		errorsSemantic++;
		} else if (node->symbol->datatype == DATATYPE_BOOL && isNotBool(node->son[0])){
        		fprintf(stderr, "SEMANTIC ERROR: Symbol %s cannot receive non boolean values. LINE: %d\n", node->symbol->value, node->lineNumber);
            		errorsSemantic++;
		}
	case AST_READ:
		if (node->symbol->type != SYMBOL_SCALAR){
        		fprintf(stderr, "SEMANTIC ERROR: Symbol %s should be scalar in scalar attributions, reads and fors. LINE: %d\n", node->symbol->value, node->lineNumber);
            		errorsSemantic++;
		}
        break;
	// Utilização de vetor
	case AST_VECTORATTR:
		if (node->symbol->datatype != DATATYPE_BOOL && isBool(node->son[1])){
        		fprintf(stderr, "SEMANTIC ERROR: Symbol %s cannot receive boolean values. LINE: %d\n", node->symbol->value, node->lineNumber);
            		errorsSemantic++;
		} else if (node->symbol->datatype == DATATYPE_BOOL && isNotBool(node->son[1])){
        		fprintf(stderr, "SEMANTIC ERROR: Symbol %s cannot receive non boolean values. LINE: %d\n", node->symbol->value, node->lineNumber);
            		errorsSemantic++;
		}
	case AST_VECTORPOS:
		if (isBool(node->son[0])){
        		fprintf(stderr, "SEMANTIC ERROR: Symbol %s, a vector cannot have a boolean position. LINE: %d\n", node->symbol->value, node->lineNumber);
            		errorsSemantic++;
		}
		if (node->symbol->type != SYMBOL_VECTOR){
        		fprintf(stderr, "SEMANTIC ERROR: Symbol %s should be vector in vector attributions and positions. LINE: %d\n", node->symbol->value, node->lineNumber);
            		errorsSemantic++;
		}
        break;
	// Utilização de função
	case AST_FUNCTIONCALL:
		if (node->symbol->type != SYMBOL_FUNCTION){
        		fprintf(stderr, "SEMANTIC ERROR: Symbol %s should be function in function calls. LINE: %d\n", node->symbol->value, node->lineNumber);
            		errorsSemantic++;
		}
		if(checkFunctionParams(node) == 0){
        		fprintf(stderr, "SEMANTIC ERROR: Symbol %s function parameters not matching. LINE: %d\n", node->symbol->value, node->lineNumber);
                	errorsSemantic++;
		}
        break;
	case AST_IFTHEN:
	case AST_IFTHENELSE:
	case AST_WHILE:
		if (isNotBool(node->son[0])){
        		fprintf(stderr, "SEMANTIC ERROR: While and If Parameter should be Boolean. LINE: %d\n", node->lineNumber);
            		errorsSemantic++;
		}
		break;
	case AST_LESS:
	    for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isBool(node->son[exp])) {
        		fprintf(stderr, "SEMANTIC ERROR: < operands should not be Boolean. LINE: %d\n", node->lineNumber);
                    errorsSemantic++;
                }
            }
            break;
        case AST_GREATER:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isBool(node->son[exp])) {
        		fprintf(stderr, "SEMANTIC ERROR: > operands should not be Boolean. LINE: %d\n", node->lineNumber);
                    errorsSemantic++;
                }
            }
            break;
        case AST_LE:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isBool(node->son[exp])) {
        		fprintf(stderr, "SEMANTIC ERROR: <= operands should not be Boolean. LINE: %d\n", node->lineNumber);
                    errorsSemantic++;
                }
            }
            break;
        case AST_GE:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isBool(node->son[exp])) {
        		fprintf(stderr, "SEMANTIC ERROR: >= operands should not be Boolean. LINE: %d\n", node->lineNumber);
                    errorsSemantic++;
                }
            }
            break;
        case AST_NOT:
            if(isNotBool(node->son[SON_LEFT])){
        		fprintf(stderr, "SEMANTIC ERROR: ~ operand should be Boolean. LINE: %d\n", node->lineNumber);
                errorsSemantic++;
            }
            break;
        case AST_AND:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isNotBool(node->son[exp])) {
        		fprintf(stderr, "SEMANTIC ERROR: . operands should be Boolean. LINE: %d\n", node->lineNumber);
                    errorsSemantic++;
                }
            }
            break;
        case AST_OR:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isNotBool(node->son[exp])) {
        		fprintf(stderr, "SEMANTIC ERROR: v operands should be Boolean. LINE: %d\n", node->lineNumber);
                    errorsSemantic++;
                }
            }
            break;
        case AST_DIFF:
            if(isNotBool(node->son[SON_LEFT])!=isNotBool(node->son[SON_RIGHT])){
        		fprintf(stderr, "SEMANTIC ERROR: != operands should match. LINE: %d\n", node->lineNumber);
                errorsSemantic++;
            }
            if(isBool(node->son[SON_LEFT])!=isBool(node->son[SON_RIGHT])){
        		fprintf(stderr, "SEMANTIC ERROR: != operands should match. LINE: %d\n", node->lineNumber);
                errorsSemantic++;
            }
            break;
        case AST_EQ:
            if(isNotBool(node->son[SON_LEFT]) != isNotBool(node->son[SON_RIGHT])){
        		fprintf(stderr, "SEMANTIC ERROR: == operands should match. LINE: %d\n", node->lineNumber);
                errorsSemantic++;
            }
            if(isBool(node->son[SON_LEFT]) != isBool(node->son[SON_RIGHT])){
        		fprintf(stderr, "SEMANTIC ERROR: == operands should match. LINE: %d\n", node->lineNumber);
                errorsSemantic++;
            }
            break;
	case AST_SUB:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isBool(node->son[exp])){
        		fprintf(stderr, "SEMANTIC ERROR: - operands should not be Boolean. LINE: %d\n", node->lineNumber);
                    errorsSemantic++;
                }
            }
            break;
        case AST_ADD:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isBool(node->son[exp])){
        		fprintf(stderr, "SEMANTIC ERROR: + operands should not be Boolean. LINE: %d\n", node->lineNumber);
                    errorsSemantic++;
                }
            }
            break;
        case AST_MUL:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isBool(node->son[exp])){
        		fprintf(stderr, "SEMANTIC ERROR: * operands should not be Boolean. LINE: %d\n", node->lineNumber);
                    errorsSemantic++;
                }
            }
            break;
        case AST_DIV:
            for(exp = INIT; exp < MAX_COMPARE; exp++){
                if(isBool(node->son[exp])){
        		fprintf(stderr, "SEMANTIC ERROR: / operands should not be Boolean. LINE: %d\n", node->lineNumber);
                    errorsSemantic++;
                }
            }
            break;
        case AST_VARIABLEDECLARATION :
            if((node->symbol->type == SYMBOL_FUNCTION|| node->symbol->type == SYMBOL_VECTOR)) {
        	fprintf(stderr, "SEMANTIC ERROR: %s declaration. LINE: %d\n", node->symbol->value, node->lineNumber);
                errorsSemantic++;
            }
            if (isBool(node->son[SON_LEFT])) {
                if(node->symbol->datatype==DATATYPE_INT ||
                   node->symbol->datatype==DATATYPE_FLOAT ||
                   node->symbol->datatype==DATATYPE_LONG ||
                   node->symbol->datatype==DATATYPE_BYTE) {
        		fprintf(stderr, "SEMANTIC ERROR: %s value should be Boolean. LINE: %d\n", node->symbol->value, node->lineNumber);
                       errorsSemantic++;
                }
            }
            if (isNotBool(node->son[SON_LEFT])){
                if(node->symbol->datatype ==DATATYPE_BOOL) {
        		fprintf(stderr, "SEMANTIC ERROR: %s value should not be Boolean. LINE: %d\n", node->symbol->value, node->lineNumber);
                    errorsSemantic++;
                }
            }
            break;
        case AST_VECTORDECLARATION:
            if(isBool(node->son[SON_LEFT])) {
        	fprintf(stderr, "SEMANTIC ERROR: %s declaration. LINE: %d\n", node->symbol->value, node->lineNumber);
                errorsSemantic++;
            }
            if(node->symbol->type != SYMBOL_VECTOR) {
        	fprintf(stderr, "SEMANTIC ERROR: %s declaration. LINE: %d\n", node->symbol->value, node->lineNumber);
                errorsSemantic++;
            }
		AST* auxnode = node->son[2];
		while (auxnode){
			if(	node->symbol->datatype != DATATYPE_BOOL && isBool(auxnode->son[0])	||
				node->symbol->datatype == DATATYPE_BOOL && isNotBool(auxnode->son[0])) {errorsSemantic++;
        			fprintf(stderr, "SEMANTIC ERROR: %s values should match its type. LINE: %d\n", node->symbol->value, node->lineNumber);
			}
			auxnode = auxnode->son[1];
		}
            break;
        case AST_FUNCTIONDECLARATION:
            if(node->symbol->type != SYMBOL_FUNCTION) {
        	fprintf(stderr, "SEMANTIC ERROR: %s declaration. LINE: %d\n", node->symbol->value, node->lineNumber);
                errorsSemantic++;
            }
	    if(verifyReturn(node) == 0){
        	fprintf(stderr, "SEMANTIC ERROR: %s return does not match. LINE: %d\n", node->symbol->value, node->lineNumber);
                errorsSemantic++;
	    }
            break;
    }
    for(exp = INIT; exp < MAX_SONS; exp++) {
        checkOperands(node->son[exp]);
    }
}
int isBool(AST* node){
	if (!node) return 0;
	if (node->type == AST_PARENTHESIS) return isBool(node->son[SON_LEFT]);
	if ( node->type == AST_LESS ||
		node->type == AST_GREATER ||
		node->type == AST_AND ||
		node->type == AST_OR ||
		node->type == AST_NOT ||
		node->type == AST_LE ||
		node->type == AST_GE ||
		node->type == AST_EQ ||
		node->type == AST_DIFF ||
		node->type == AST_NOT ||
		(node->type == AST_SYMBOL &&
		 (node->symbol->type == SYMBOL_SCALAR ||
		  node->symbol->type == SYMBOL_VECTOR ||
		  node->symbol->type == SYMBOL_FUNCTION) &&
		 node->symbol->datatype == DATATYPE_BOOL) ||
		(node->type == AST_SYMBOL &&
		 node->symbol->type == SYMBOL_LITBOOL) ||
		(node->type == AST_VECTORPOS &&
		 node->symbol->type == SYMBOL_VECTOR &&
		 node->symbol->datatype == DATATYPE_BOOL) ||
		(node->type == AST_FUNCTIONCALL &&
		 node->symbol->type == SYMBOL_FUNCTION &&
		 node->symbol->datatype == DATATYPE_BOOL)
		) return 1;
	return 0;
}
int isNotBool(AST* node){
	if (!node) return 0;
	if (node->type == AST_PARENTHESIS) return isNotBool(node->son[SON_LEFT]);
	if ( node->type == AST_ADD ||
		node->type == AST_SUB ||
		node->type == AST_MUL ||
		node->type == AST_DIV ||
	 	 (node->type == AST_SYMBOL &&
		 (node->symbol->type == SYMBOL_SCALAR ||
		  node->symbol->type == SYMBOL_VECTOR ||
		  node->symbol->type == SYMBOL_FUNCTION) &&
		  (node->symbol->datatype == DATATYPE_INT ||
		  node->symbol->datatype == DATATYPE_FLOAT ||
		  node->symbol->datatype == DATATYPE_BYTE ||
		  node->symbol->datatype == DATATYPE_LONG)) ||
		(node->type == AST_SYMBOL &&
		 (node->symbol->type == SYMBOL_LITINT ||
		  node->symbol->type == SYMBOL_LITREAL)) ||
		(node->type == AST_VECTORPOS &&
		 node->symbol->type == SYMBOL_VECTOR &&
		 (node->symbol->datatype == DATATYPE_INT ||
		  node->symbol->datatype == DATATYPE_FLOAT ||
		  node->symbol->datatype == DATATYPE_BYTE ||
		  node->symbol->datatype == DATATYPE_LONG)) ||
		(node->type == AST_FUNCTIONCALL &&
		 node->symbol->type == SYMBOL_FUNCTION &&
		 (node->symbol->datatype == DATATYPE_INT ||
		  node->symbol->datatype == DATATYPE_FLOAT ||
		  node->symbol->datatype == DATATYPE_BYTE ||
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
	if (!node || !node->son[SON_LEFT]) return 1;

	if (node->son[0]->type == AST_RETURN) return confirmType(node->son[0], datatype);
	else return checkReturn(node->son[1], datatype);
}
int verifyReturn(AST* node){
	if (!node || !node->son[2] || !node->son[2]->son[0]){return 1;}
	if (node->son[2]->son[0]->type == AST_RETURN) return confirmType(node->son[2]->son[0], node->symbol->datatype);
	else return checkReturn(node->son[2]->son[1], node->symbol->datatype);
}
int checkFunctionParams(AST* node){
	AST* auxnode = rootNode;
	while (auxnode) {
		if (!auxnode || !auxnode->son[0]) return 0;
		if (auxnode->son[0]->type == AST_FUNCTIONDECLARATION) {
			if (strcmp(auxnode->son[0]->symbol->value, node->symbol->value) == 0){
			 	break;
			}
			auxnode = auxnode->son[1];
		}
		else auxnode = auxnode->son[1];
	}
	if (!auxnode || !auxnode->son[0]) return 0;
	auxnode = auxnode->son[0];
	if (!auxnode->son[1] && !node->son[0]) return 1;
	if (!auxnode->son[1] || !node->son[0]) return 0;
	auxnode = auxnode->son[1];
	node = node->son[0];
	while (auxnode->son[0] && node->son[0]){
		if (!( (auxnode->son[0]->son[0]->type == AST_BOOL && isBool(node->son[0])) ||
			(!(auxnode->son[0]->son[0]->type == AST_BOOL) && isNotBool(node->son[0]))
		      )) return 0;

		if (!auxnode->son[1] && !node->son[1]) return 1;
		if (!auxnode->son[1] || !node->son[1]) return 0;
		auxnode = auxnode->son[1];
		node = node->son[1];
	}
	if (!auxnode && !node) return 1;
	return 0;
}
