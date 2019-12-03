#include "ast.h"

AST* astCreate(int type, HASHTABLE_NODE* symbol, AST* son0, AST* son1, AST* son2, AST* son3, int lineNumber) {
    AST* newNode = 0;
    newNode = (AST*) calloc(DEFAULT_N_OBJECTS, sizeof(AST)); // DEFAULT_N_OBJECTS from hash.h
    newNode->type = type;
    newNode->symbol = symbol;
    newNode->son[SON_ZERO] = son0;
    newNode->son[SON_ONE] = son1;
    newNode->son[SON_TWO] = son2;
    newNode->son[SON_THREE] = son3;
    newNode->lineNumber = lineNumber;

    return newNode;
}

void astPrint(AST *node, int level) {
    int i = 0;
    if(node == 0) {
        return;
    }
    // identation
    for(i = 0; i < level; i++) {
    	fprintf(stderr, "__");
    }
    switch (node->type) {
        case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL"); break;
        case AST_DECLARATION: fprintf(stderr, "AST_DECLARATION"); break;
        case AST_VARIABLEDECLARATION: fprintf(stderr, "AST_VARIABLEDECLARATION"); break;
        case AST_VECTORDECLARATION: fprintf(stderr, "AST_VECTORDECLARATION"); break;
        case AST_BOOL: fprintf(stderr, "AST_BOOL"); break;
        case AST_BYTE: fprintf(stderr, "AST_BYTE"); break;
        case AST_INT: fprintf(stderr, "AST_INT"); break;
        case AST_LONG: fprintf(stderr, "AST_LONG"); break;
        case AST_FLOAT: fprintf(stderr, "AST_FLOAT"); break;
        case AST_VECTORVALUE: fprintf(stderr, "AST_VECTORVALUE"); break;
        case AST_VECTORREMAINDER: fprintf(stderr, "AST_VECTORREMAINDER"); break;
        case AST_FUNCTIONDECLARATION: fprintf(stderr, "AST_FUNCTIONDECLARATION"); break;
        case AST_PARAMLIST: fprintf(stderr, "AST_PARAMLIST"); break;
        case AST_REMAINDER: fprintf(stderr, "AST_REMAINDER"); break;
        case AST_PARAM: fprintf(stderr, "AST_PARAM"); break;
        case AST_COMMANDBLOCK: fprintf(stderr, "AST_COMMANDBLOCK"); break;
        case AST_COMMANDREMAINDER: fprintf(stderr, "AST_COMMANDREMAINDER"); break;
        case AST_ATTR: fprintf(stderr, "AST_ATTR"); break;
        case AST_VECTORATTR: fprintf(stderr, "AST_VECTORATTR"); break;
        case AST_READ: fprintf(stderr, "AST_READ"); break;
        case AST_PRINT: fprintf(stderr, "AST_PRINT"); break;
        case AST_RETURN: fprintf(stderr, "AST_RETURN"); break;
        case AST_FLUXCONTROL: fprintf(stderr, "AST_FLUXCONTROL"); break;
        case AST_IFTHEN: fprintf(stderr, "AST_IFTHEN"); break;
        case AST_IFTHENELSE: fprintf(stderr, "AST_IFTHENELSE"); break;
        case AST_WHILE: fprintf(stderr, "AST_WHILE"); break;
        case AST_FOR: fprintf(stderr, "AST_FOR"); break;
        case AST_BREAK: fprintf(stderr, "AST_BREAK"); break;
        case AST_PRINTVALUE: fprintf(stderr, "AST_PRINTVALUE"); break;
        case AST_PRINTFINAL: fprintf(stderr, "AST_PRINTFINAL"); break;
        case AST_FUNCTIONCALL: fprintf(stderr, "AST_FUNCTIONCALL"); break;
        case AST_VECTORPOS: fprintf(stderr, "AST_VECTORPOS"); break;
        case AST_PARENTHESIS: fprintf(stderr, "AST_PARENTHESIS"); break;
        case AST_ADD: fprintf(stderr, "AST_ADD"); break;
        case AST_SUB: fprintf(stderr, "AST_SUB"); break;
        case AST_MUL: fprintf(stderr, "AST_MUL"); break;
        case AST_DIV: fprintf(stderr, "AST_DIV"); break;
        case AST_LESS: fprintf(stderr, "AST_LESS"); break;
        case AST_GREATER: fprintf(stderr, "AST_GREATER"); break;
        case AST_AND: fprintf(stderr, "AST_AND"); break;
        case AST_OR: fprintf(stderr, "AST_OR"); break;
        case AST_NOT: fprintf(stderr, "AST_NOT"); break;
        case AST_LE: fprintf(stderr, "AST_LE"); break;
        case AST_GE: fprintf(stderr, "AST_GE"); break;
        case AST_EQ: fprintf(stderr, "AST_EQ"); break;
        case AST_DIFF: fprintf(stderr, "AST_DIFF"); break;
        case AST_ARGLIST: fprintf(stderr, "AST_ARGLIST"); break;
        case AST_ARGREMAINDER: fprintf(stderr, "AST_ARGREMAINDER"); break;
        case AST_DECLARATIONLIST: fprintf(stderr, "AST_DECLARATIONLIST"); break;
        default: break;
    }

    if(node->symbol){
        fprintf(stderr, " - %s\n", node->symbol->value);
    } else{
        fprintf(stderr, "\n");
    }
    for(i=0; i < MAX_SONS; i++){
        astPrint(node->son[i], level+1);
    }
}

void astMakeProgram(AST* node, FILE* fileOut) {
	if(!node) {
        return;
    }
    switch(node->type){
		case AST_DECLARATIONLIST:
			astMakeProgram(node->son[0], fileOut);
			astMakeProgram(node->son[1], fileOut);
			break;
		case AST_DECLARATION:
			astMakeProgram(node->son[0], fileOut);
			break;
		case AST_VARIABLEDECLARATION:
			astMakeProgram(node->son[0], fileOut);
        	        fprintf(fileOut, "%s = ",node->symbol->value);
			astMakeProgram(node->son[1], fileOut);
            fprintf(fileOut, ";\n");
			break;
		case AST_VECTORDECLARATION:
			astMakeProgram(node->son[0], fileOut);
            fprintf(fileOut, "%s [ ",node->symbol->value);
			astMakeProgram(node->son[1], fileOut);
            fprintf(fileOut, "]");
			astMakeProgram(node->son[2], fileOut);
	       fprintf(fileOut, ";\n");
		break;
        case AST_SYMBOL:
			fprintf(fileOut, " %s ",node->symbol->value);
			break;
        case AST_BOOL:
            fprintf(fileOut, "bool ");
            break;
        case AST_BYTE:
            fprintf(fileOut, "byte ");
            break;

        case AST_INT:
            fprintf(fileOut, "int ");
            break;

        case AST_LONG:
            fprintf(fileOut, "long ");
            break;

        case AST_FLOAT:
            fprintf(fileOut, "float ");
            break;

		case AST_VECTORVALUE:
            fprintf(fileOut, ":");
			astMakeProgram(node->son[0], fileOut);
			astMakeProgram(node->son[1], fileOut);
			break;
			
		case AST_VECTORREMAINDER:
			astMakeProgram(node->son[0], fileOut);
			astMakeProgram(node->son[1], fileOut);
			break;

		case AST_FUNCTIONDECLARATION:
			astMakeProgram(node->son[0], fileOut);
            fprintf(fileOut, "%s ( ",node->symbol->value);
			astMakeProgram(node->son[1], fileOut);
            fprintf(fileOut, ")");
			astMakeProgram(node->son[2], fileOut);
            fprintf(fileOut, "\n");
			break;

		case AST_PARAMLIST:
			astMakeProgram(node->son[0], fileOut);
			astMakeProgram(node->son[1], fileOut);
			break;
			
		case AST_REMAINDER:
            fprintf(fileOut, ",");
			astMakeProgram(node->son[0], fileOut);
			astMakeProgram(node->son[1], fileOut);
			break;
			
		case AST_PARAM:
			astMakeProgram(node->son[0], fileOut);
            fprintf(fileOut, "%s",node->symbol->value);
			break;

		case AST_COMMANDBLOCK:
            fprintf(fileOut, "{ \n");
			astMakeProgram(node->son[0], fileOut);
			astMakeProgram(node->son[1], fileOut);
            fprintf(fileOut, " }\n");
			break;

		case AST_COMMANDREMAINDER:
			fprintf(fileOut, " ;\n");
			astMakeProgram(node->son[0], fileOut);
			astMakeProgram(node->son[1], fileOut);
			break;

		case AST_ATTR:
            fprintf(fileOut, "%s",node->symbol->value);
            fprintf(fileOut, " = ");
			astMakeProgram(node->son[0], fileOut);
			break;

		case AST_VECTORATTR:
            fprintf(fileOut, "%s",node->symbol->value);
            fprintf(fileOut, " [");
			astMakeProgram(node->son[0], fileOut);
            fprintf(fileOut, "] = ");
			astMakeProgram(node->son[1], fileOut);
			break;

        case AST_READ:
            fprintf(fileOut, "read  %s",node->symbol->value);
            break;

        case AST_PRINT:
            fprintf(fileOut, "print");
			astMakeProgram(node->son[0], fileOut);
            break;

        case AST_RETURN:
            fprintf(fileOut, "return");
			astMakeProgram(node->son[0], fileOut);
            break;

        case AST_FLUXCONTROL:
			astMakeProgram(node->son[0], fileOut);
            break;

		case AST_IFTHEN:
            fprintf(fileOut, "if ( ");
			astMakeProgram(node->son[0], fileOut);
            fprintf(fileOut, " ) then ");
			astMakeProgram(node->son[1], fileOut);
			break;

		case AST_IFTHENELSE:
            fprintf(fileOut, "if ( ");
			astMakeProgram(node->son[0], fileOut);
            fprintf(fileOut, " ) then ");
			astMakeProgram(node->son[1], fileOut);
            fprintf(fileOut, " else ");
			astMakeProgram(node->son[2], fileOut);
			break;

		case AST_WHILE:
            fprintf(fileOut, "while ( ");
			astMakeProgram(node->son[0], fileOut);
            fprintf(fileOut, " ) ");
			astMakeProgram(node->son[1], fileOut);
			break;

		case AST_FOR:
            fprintf(fileOut, "for ( %s : ", node->symbol->value);
			astMakeProgram(node->son[0], fileOut);
            fprintf(fileOut, " , ");
			astMakeProgram(node->son[1], fileOut);
            fprintf(fileOut, " , ");
			astMakeProgram(node->son[2], fileOut);
            fprintf(fileOut, " ) ");
			astMakeProgram(node->son[3], fileOut);
			break;

		case AST_BREAK:
            fprintf(fileOut, "break");
			break;

		case AST_PRINTVALUE:
			astMakeProgram(node->son[0], fileOut);
			astMakeProgram(node->son[1], fileOut);
			break;

		case AST_PRINTFINAL:
			astMakeProgram(node->son[0], fileOut);
			break;

		case AST_FUNCTIONCALL:
            fprintf(fileOut, " %s ( ", node->symbol->value);
			astMakeProgram(node->son[0], fileOut);
            fprintf(fileOut, " ) ");
			break;

		case AST_VECTORPOS:
            fprintf(fileOut, " %s [ ", node->symbol->value);
			astMakeProgram(node->son[0], fileOut);
            fprintf(fileOut, " ] ");
			break;

		case AST_PARENTHESIS:
            fprintf(fileOut, " ( ");
			astMakeProgram(node->son[0], fileOut);
            fprintf(fileOut, " ) ");
		
        case AST_ADD:
            astMakeProgram(node->son[0],fileOut);
            fprintf(fileOut, " + ");
            astMakeProgram(node->son[1],fileOut);
            break;
		
        case AST_SUB:
            astMakeProgram(node->son[0],fileOut);
            fprintf(fileOut, " - ");
            astMakeProgram(node->son[1],fileOut);
            break;
		
        case AST_MUL:
            astMakeProgram(node->son[0],fileOut);
            fprintf(fileOut, " * ");
            astMakeProgram(node->son[1],fileOut);
            break;
		
        case AST_DIV:
            astMakeProgram(node->son[0],fileOut);
            fprintf(fileOut, " / ");
            astMakeProgram(node->son[1],fileOut);
            break;
		
        case AST_LESS:
            astMakeProgram(node->son[0],fileOut);
            fprintf(fileOut, " < ");
            astMakeProgram(node->son[1],fileOut);
            break;
		
        case AST_GREATER:
            astMakeProgram(node->son[0],fileOut);
            fprintf(fileOut, " > ");
            astMakeProgram(node->son[1],fileOut);
            break;
		
        case AST_AND:
            astMakeProgram(node->son[0],fileOut);
            fprintf(fileOut, " . ");
            astMakeProgram(node->son[1],fileOut);
            break;
		
        case AST_OR:
            astMakeProgram(node->son[0],fileOut);
            fprintf(fileOut, " v ");
            astMakeProgram(node->son[1],fileOut);
            break;
		
        case AST_NOT:
            fprintf(fileOut, "~ ");
            astMakeProgram(node->son[1],fileOut);
            break;
		
        case AST_LE:
            astMakeProgram(node->son[0],fileOut);
            fprintf(fileOut, " <= ");
            astMakeProgram(node->son[1],fileOut);
            break;
		
        case AST_GE:
            astMakeProgram(node->son[0],fileOut);
            fprintf(fileOut, " >= ");
            astMakeProgram(node->son[1],fileOut);
            break;
		
        case AST_EQ:
            astMakeProgram(node->son[0],fileOut);
            fprintf(fileOut, " == ");
            astMakeProgram(node->son[1],fileOut);
            break;
		
        case AST_DIFF:
            astMakeProgram(node->son[0],fileOut);
            fprintf(fileOut, " != ");
            astMakeProgram(node->son[1],fileOut);
            break;
		
        case AST_ARGLIST:
            astMakeProgram(node->son[0],fileOut);
            astMakeProgram(node->son[1],fileOut);
            break;
		
        case AST_ARGREMAINDER:
            fprintf(fileOut, " , ");
            astMakeProgram(node->son[0],fileOut);
            astMakeProgram(node->son[1],fileOut);
            break;
	default: printf("DEFAULT");break;
            
    }
}
