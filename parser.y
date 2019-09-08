%{
	#include <stdio.h>
	#include <stdlib.h>
	int getLineNumber(void);
	int yylex();
	int yyerror();
%}

%token KW_BYTE
%token KW_INT
%token KW_LONG
%token KW_FLOAT
%token KW_BOOL
%token KW_IF
%token KW_THEN
%token KW_ELSE
%token KW_WHILE
%token KW_FOR
%token KW_READ
%token KW_PRINT
%token KW_RETURN
%token KW_BREAK
%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_DIF
%token TOKEN_ERROR
%token<symbol> TK_IDENTIFIER
%token<symbol> LIT_INTEGER
%token<symbol> LIT_FLOAT
%token<symbol> LIT_TRUE
%token<symbol> LIT_FALSE
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING


%%
program: program declaration
	|
	;

declaration: variableDeclaration | functionDeclaration
        ;

variableDeclaration: KW_INT TK_IDENTIFIER '=' LIT_INTEGER ';'
        ;

initializationLiterals: LIT_INTEGER
        ;

functionDeclaration: KW_INT TK_IDENTIFIER '(' ')' command
        ;

command: TK_IDENTIFIER '=' LIT_FLOAT
        | block
        ;

block: '{' commandList '}'
        ;

commandList: commandList command ';'
        |
        ;

%%



int yyerror(char *msg){
	fprintf(stderr, "Syntax Error on the line [%d]\n", getLineNumber());
	exit(3); // exiting status when a syntax error occurs
}
