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
%token TK_IDENTIFIER
%token LIT_INTEGER
%token LIT_FLOAT
%token LIT_TRUE
%token LIT_FALSE
%token LIT_CHAR
%token LIT_STRING


%%
program: 	program declaration
	|
	;

declaration: 	variableDeclaration 
	| 	functionDeclaration
        ;




variableDeclaration: 	variableType				TK_IDENTIFIER '=' variableValue 	';'
	|		variableType	'[' LIT_INTEGER ']' 	TK_IDENTIFIER ':' vectorValue	 	';'
        ;

variableType:		KW_BOOL
	|		KW_BYTE
	|		KW_INT
	|		KW_LONG
	|		KW_FLOAT
	;

vectorValue:	vectorValue variableValue
	|
	;

variableValue:		LIT_INTEGER
	|		LIT_FLOAT
	|		LIT_CHAR
	;





functionDeclaration: KW_INT TK_IDENTIFIER '(' paramList ')' command
        ;

paramList:	param remainder
	|
	;

remainder:	',' param remainder
	|
	;

param:		variableType TK_IDENTIFIER
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
