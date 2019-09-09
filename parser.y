%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "hash.h"
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
program: program declaration
	|
	;

declaration: variableDeclaration 
	| functionDeclaration
        ;

variableDeclaration: 	KW_INT 					TK_IDENTIFIER '=' LIT_INTEGER 	';'
	|		KW_BOOL 				TK_IDENTIFIER '=' boolValue	';'
	|		KW_LONG 				TK_IDENTIFIER '=' LIT_INTEGER 	';'
	|		KW_FLOAT 				TK_IDENTIFIER '=' LIT_FLOAT 	';'
	|		KW_BYTE					TK_IDENTIFIER '=' LIT_CHAR 	';'
	|		KW_INT 		'[' LIT_INTEGER ']' 	TK_IDENTIFIER ':' LIT_INTEGER 	';'
	|		KW_BOOL 	'[' LIT_INTEGER ']' 	TK_IDENTIFIER ':' boolValue	';'
	|		KW_LONG 	'[' LIT_INTEGER ']' 	TK_IDENTIFIER ':' LIT_INTEGER 	';'
	|		KW_FLOAT 	'[' LIT_INTEGER ']' 	TK_IDENTIFIER ':' LIT_FLOAT 	';'
	|		KW_BYTE 	'[' LIT_INTEGER ']' 	TK_IDENTIFIER ':' LIT_CHAR 	';'
        ;

boolValue:		LIT_TRUE
	|		LIT_FALSE
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
