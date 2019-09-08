%{
	#include <stdio.h>
	#include <stdlib.h>
	int getLineNumber(void);
	int yylex();
	int yyerror();
%}

%%
program



int yyerror(char *msg){
	fprintf(stderr, "Syntax Error at line %d\n", getLineNumber());
	exit(3);

}
