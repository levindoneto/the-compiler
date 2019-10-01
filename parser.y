%{	
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>

	#include "ast.h"
	#include "hash.h"	

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
program:		program declaration {$$ = $1}
	|
	;

declaration:		variableDeclaration {$$ = }
	|		functionDeclaration
	|		vectorDeclaration
        ;

//		VARIABLE DECLARATION
variableDeclaration: 	variableType TK_IDENTIFIER	'=' variableValue ';'
        ;

vectorDeclaration:	variableType TK_IDENTIFIER '[' LIT_INTEGER ']' vectorValue ';'
	;

variableType:		KW_BOOL
	|		KW_BYTE
	|		KW_INT
	|		KW_LONG
	|		KW_FLOAT
	;

vectorValue:		':' variableValue vectorRemainder
	|		
	;

vectorRemainder:	variableValue vectorRemainder
	|
	;

variableValue:		LIT_INTEGER {fprintf(stderr, "INT_Id=%d\n", $1);}
	|		LIT_FLOAT {fprintf(stderr, "FLOAT_Id=%d\n", $1);}
	|		LIT_CHAR
	|		LIT_TRUE
	|		LIT_FALSE
	;

//		FUNCTION DECLARATION
functionDeclaration: 	variableType TK_IDENTIFIER '(' paramList ')' commandBlock
        ;

paramList:		param remainder
	|
	;

remainder:		',' param remainder
	|
	;

param:			variableType TK_IDENTIFIER
	;

// COMMAND BLOCK
commandBlock:		'{' command commandRemainder '}'
	;

commandRemainder:	command	';' commandRemainder
	|
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
	|		expression 		'+' 			expression
	|		expression 		'-' 			expression
	|		expression 		'*' 			expression
	|		expression 		'/' 			expression
	|		expression 		'<' 			expression
	|		expression 		'>' 			expression
	|		expression 		'.' 			expression
	|		expression 		'v' 			expression
	|		expression 		'~' 			expression
	|		expression 		OPERATOR_LE 		expression
	|		expression 		OPERATOR_GE 		expression
	|		expression 		OPERATOR_EQ 		expression
	|		expression 		OPERATOR_DIF	 	expression
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
