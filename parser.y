%{	
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>

	#include "ast.h"	

	int getLineNumber(void);
	int yylex();
	int yyerror(char*);
%}

%union {
	struct hash_node* symbol;
	struct ast_node* ast;
} 

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

%token <symbol> TK_IDENTIFIER
%token <symbol> LIT_INTEGER
%token <symbol> LIT_FLOAT
%token <symbol> LIT_TRUE
%token <symbol> LIT_FALSE
%token <symbol> LIT_CHAR
%token <symbol> LIT_STRING


%type <ast> program
%type <ast> declaration
%type <ast> variableDeclaration
%type <ast> functionDeclaration
%type <ast> vectorDeclaration
%type <ast> vectorSize
%type <ast> variableType
%type <ast> variableValue
%type <ast> vectorValue
%type <ast> paramList
%type <ast> remainder
%type <ast> param
%type <ast> commandBlock
%type <ast> commandRemainder
%type <ast> command
%type <ast> fluxControl
%type <ast> printValue
%type <ast> expression
%type <ast> argList
%type <ast> argRemainder
%type <ast> argument


%left '*' '/'
%left '-' '+'
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_NE
%left OPERATOR_OR OPERATOR_AND
%right '!'

%%
program:		program declaration			{$$ = $1;}
	|							{$$ = 0;}
	;

declaration:		variableDeclaration			{$$ = astCreate{AST_DECLARATION, 0, $1, 0, 0, 0};}
	|		functionDeclaration			{$$ = astCreate{AST_DECLARATION, 0, $1, 0, 0, 0};}
	|		vectorDeclaration			{$$ = astCreate{AST_DECLARATION, 0, $1, 0, 0, 0};}
        ;

//		VARIABLE DECLARATION
variableDeclaration:	variableType TK_IDENTIFIER	'=' variableValue ';'		{$$ = astCreate{AST_VARIABLEDECLARATION, $2, $1, $4, 0, 0};}
        ;

vectorDeclaration:	variableType TK_IDENTIFIER '[' vectorSize ']' vectorValue ';'	{$$ = astCreate{AST_VECTORDECLARATION, $2, $1, $4, $6, 0};}
	;

vectorSize:		LIT_INTEGER				{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
	;

variableType:		KW_BOOL					{$$ = astCreate{AST_BOOL, 0, 0, 0, 0, 0};}
	|		KW_BYTE					{$$ = astCreate{AST_BYTE, 0, 0, 0, 0, 0};}
	|		KW_INT					{$$ = astCreate{AST_INT, 0, 0, 0, 0, 0};}
	|		KW_LONG					{$$ = astCreate{AST_LONG, 0, 0, 0, 0, 0};}
	|		KW_FLOAT				{$$ = astCreate{AST_FLOAT, 0, 0, 0, 0, 0};}
	;

vectorValue:		':' variableValue vectorRemainder	{$$ = astCreate{AST_VECTORVALUE, 0, $2, $3, 0, 0};}
	|							{$$ = 0;}
	;

vectorRemainder:	variableValue vectorRemainder		{$$ = astCreate{AST_VECTORREMAINDER, 0, $1, $2, 0, 0};}
	|							{$$ = 0;}
	;

variableValue:		LIT_INTEGER				{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
	|		LIT_FLOAT				{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
	|		LIT_CHAR				{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
	|		LIT_TRUE				{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
	|		LIT_FALSE				{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
	;

//		FUNCTION DECLARATION
functionDeclaration:	variableType TK_IDENTIFIER '(' paramList ')' commandBlock {$$ = astCreate(AST_FUNCTIONDECLARATION, $2, $1, $4, $6, 0);}
        ;

paramList:		param remainder				{$$ = astCreate(AST_PARAMLIST, 0, $1, $2, 0, 0);}
	|							{$$=0;}
	;

remainder:		',' param remainder			{$$ = astCreate(AST_REMAINDER, 0, $2, $3, 0, 0);}
	|							{$$=0;}
	;

param:			variableType TK_IDENTIFIER		{$$ = astCreate(AST_PARAM, $2, $1, 0, 0, 0);}
	;

// COMMAND BLOCK
commandBlock:		'{' command commandRemainder '}'	{$$ = astCreate(AST_COMMANDBLOCK, 0, $2, $3, 0, 0);}
	;

commandRemainder:	command	';' commandRemainder		{$$ = astCreate(AST_COMMANDREMAINDER, 0, $1, $3, 0, 0);}
	|							{$$ = 0;}
	;


command:		TK_IDENTIFIER '=' expression
	|		TK_IDENTIFIER '[' expression ']' '=' expression 
	|		KW_READ	  TK_IDENTIFIER 
	|		KW_PRINT  printValue 
	|		KW_RETURN expression 
	|		fluxControl 
	|		commandBlock 
	|
        ;



//		COMMANDS
fluxControl:		KW_IF		'(' expression ')' KW_THEN command
	|		KW_IF		'(' expression ')' KW_THEN command KW_ELSE command
	|		KW_WHILE	'(' expression ')' command 
	|		KW_FOR		'(' TK_IDENTIFIER ':' expression ',' expression ',' expression ')' command
	|		KW_BREAK
	;

printValue:		expression printValue
	|		expression
	|		LIT_STRING
	;

//		EXPRESSIONS
expression:		argument
	|		TK_IDENTIFIER  '(' argList ')'
	|		TK_IDENTIFIER  '[' expression ']'
	|		'(' expression ')'
	|		expression		'+'			expression
	|		expression		'-'			expression
	|		expression		'*'			expression
	|		expression		'/'			expression
	|		expression		'<'			expression
	|		expression		'>'			expression
	|		expression		'.'			expression
	|		expression		'v'			expression
	|		expression		'~'			expression
	|		expression		OPERATOR_LE		expression
	|		expression		OPERATOR_GE		expression
	|		expression		OPERATOR_EQ		expression
	|		expression		OPERATOR_DIF		expression
	;

argList:		argument argRemainder
	|
	;

argRemainder:		',' argument argRemainder
	|
	;

argument:		TK_IDENTIFIER
	|		LIT_FLOAT
	|		LIT_INTEGER
	|		LIT_CHAR
	;

%%


int yyerror(char *msg){
	fprintf(stderr, "Syntax Error on the line [%d]\n", getLineNumber());
	exit(3); // exiting status when a syntax error occurs
}
