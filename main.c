#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"

extern int getLineNumber();
extern int isRunning();
extern void initMe();
extern int yylex();
extern void printHash();
extern FILE *yyin;
extern char *yytext;
extern int running;
extern int lineNumber;

void showToken(int tok) {
	switch (tok) {
		case KW_BYTE: printf("KW_BYTE = linha %d \n", getLineNumber()); break;
		case KW_INT: printf("KW_INT = linha %d \n", getLineNumber()); break;
		case KW_LONG: printf("KW_LONG = linha %d \n", getLineNumber()); break;
		case KW_FLOAT: printf("KW_FLOAT = linha %d \n", getLineNumber()); break;
		case KW_BOOL: printf("KW_BOOL = linha %d \n", getLineNumber()); break;
		case KW_IF: printf("KW_IF = linha %d \n", getLineNumber()); break;
		case KW_THEN: printf("KW_THEN = linha %d \n", getLineNumber()); break;
		case KW_ELSE: printf("KW_ELSE = linha %d \n", getLineNumber()); break;
		case KW_WHILE: printf("KW_WHILE = linha %d \n", getLineNumber()); break;
		case KW_FOR: printf("KW_FOR = linha %d \n", getLineNumber()); break;
		case KW_READ: printf("KW_READ = linha %d \n", getLineNumber()); break;
		case KW_PRINT: printf("KW_PRINT = linha %d \n", getLineNumber()); break;
		case KW_RETURN: printf("KW_RETURN = linha %d \n", getLineNumber()); break;
		case KW_BREAK: printf("KW_BREAK = linha %d \n", getLineNumber()); break;
		case OPERATOR_LE: printf("OPERATOR_LE = linha %d \n", getLineNumber()); break;
		case OPERATOR_GE: printf("OPERATOR_GE = linha %d \n", getLineNumber()); break;
		case OPERATOR_EQ: printf("OPERATOR_EQ = linha %d \n", getLineNumber()); break;
		case OPERATOR_DIF: printf("OPERATOR_DIF = linha %d \n", getLineNumber()); break;
		case TK_IDENTIFIER: printf("TK_IDENTIFIER = linha %d \n", getLineNumber()); break;
		case LIT_INTEGER: printf("LIT_INTEGER = linha %d \n", getLineNumber()); break;
		case LIT_FLOAT: printf("LIT_FLOAT = linha %d \n", getLineNumber()); break;
		case LIT_CHAR: printf("LIT_CHAR = linha %d \n", getLineNumber()); break;
		case LIT_STRING: printf("LIT_STRING = linha %d \n", getLineNumber()); break;
		case LIT_FALSE: printf("LIT_FALSE = linha %d \n", getLineNumber()); break;
		case LIT_TRUE: printf("LIT_TRUE = linha %d \n", getLineNumber()); break;
		case TOKEN_ERROR: printf("TOKEN_ERROR |%s| = linha %d \n", yytext, getLineNumber()); break;
		default: if(yytext[0])printf("TOKEN_ESPECIAL |%s| = linha %d \n", yytext, getLineNumber()); break;
	}
}

int main(int argc, char ** argv) {
	int tok;
	int parsingResult;
	if (argc < 2) {
		fprintf(stderr, "Call: a.out file_name\n");
		exit(1);
	}
	if ((yyin = fopen(argv[1], "r")) == 0) {
		fprintf(stderr, "Cannot open file \%s\"\n", argv[1]);
		exit(2);
	}
	initMe(); // init hashtable structure
	parsingResult = yyparse(); // Check if the content belongs to break192

	if(parsingResult == 0) {
        	fprintf(stderr, "Code accepted regarding the grammar\n");
	        printHash(); // print the whole structuring after parsing it
	        exit(0); // 0: for file alright
    	}  else {
       		fprintf(stderr, "Error parsing the code on line %d\n", getLineNumber());
	        exit(3); // 3: error on reading source code
	}
}
