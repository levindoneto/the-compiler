%{
	#include <stdio.h>
	#include <stdlib.h>
	//#include "lex.yy.h"
	#include "ast.h"
	#include "hash.h"
	int getLineNumber(void);
	//int yylex();
	int yyerror(char*);
%}

%union
{
	int value;
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
%token TK_IDENTIFIER

%token<value> LIT_INTEGER
%token<value> LIT_FLOAT
%token LIT_TRUE
%token LIT_FALSE
%token LIT_CHAR
%token LIT_STRING

%nonassoc JUSTIF
%nonassoc KW_ELSE

%nonassoc noSemiColon
%nonassoc ';'



%%
program:		program declaration
	|
	;

declaration:		variableDeclaration 
	|		functionDeclaration
	|		vectorDeclaration
        ;

//		VARIABLE DECLARATION
variableDeclaration: variableType TK_IDENTIFIER	'=' variableValue ';'
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
	|		booleanValue
	;

//		FUNCTION DECLARATION
functionDeclaration: variableType TK_IDENTIFIER '(' paramList ')' '{' commandBlock '}'
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
commandBlock:		command commandRemainder
	;

commandFlux:		'{' command commandRemainderFlux
	|		command commandEnd command
	;

commandRemainderFlux:	';' command commandRemainderFlux
	|		'}' command
	;

commandEnd:		';' 
	|		%prec noSemiColon
	;

commandRemainder:	';' command commandRemainder
	|
	;


command:		assignement 
	|		readComm 
	|		printComm 
	|		returnComm 
	|		fluxControl 
	|
        ;



//		COMMANDS
assignement:		TK_IDENTIFIER '=' expression
	|		TK_IDENTIFIER '[' expression ']' '=' expression
	;

fluxControl:		KW_IF		'(' expression ')' KW_THEN commandFlux %prec JUSTIF
	|		KW_IF		'(' expression ')' KW_THEN commandFlux KW_ELSE commandFlux
	|		KW_WHILE	'(' expression ')' commandFlux 
	|		KW_FOR		'(' TK_IDENTIFIER ':' expression ',' expression ',' expression ')' commandFlux
	|		KW_BREAK
	;

readComm:		KW_READ	  TK_IDENTIFIER
	;

printComm:		KW_PRINT  printValue printElement
	;

printValue:		expression
	|		LIT_STRING
	;

printElement:		printValue printElement
	|
	;

returnComm:		KW_RETURN expression
	;

//		EXPRESSIONS
expression:		arithmetic
	|		booleanExp
	|		TK_IDENTIFIER '(' argList ')'
	;

argList:		argument argRemainder
	|
	;

argRemainder:		',' argument argRemainder
	|
	;

argument:		TK_IDENTIFIER
	|		LIT_FLOAT
	|		LIT_TRUE
	|		LIT_FALSE
	|		LIT_INTEGER
	|		LIT_CHAR
	;

//		ARITHMETIC PART
arithmetic:		arithmeticValue arithmeticRemainder
	;

arithmeticRemainder:	arithmeticOp arithmeticBegin '(' arithmetic ')' arithmeticEnd
	|		arithmeticEnd
	;

arithmeticEnd:		arithmeticOp arithmeticValue arithmeticEnd
	|
	;

arithmeticBegin:	arithmeticValue arithmeticOp arithmeticBegin
	|
	;

arithmeticValue:	identifiers
	|		LIT_INTEGER
	|		LIT_CHAR
	|		LIT_FLOAT
	;

identifiers:		TK_IDENTIFIER
	|		TK_IDENTIFIER '[' expression ']'
	;

arithmeticOp:		'*'
	|		'/'
	|		'+'
	|		'-'
	;

//		BOOLEAN PART
booleanExp:		booleanValue booleanRemainder
	;

booleanRemainder:	booleanOp booleanValue booleanRemainder
	|
	;

booleanValue:		LIT_FALSE
	|		LIT_TRUE
	|		arithmetic comparativeOp arithmetic
	;

booleanOp:		'.'
	|		'v'
	|		'~'
//	|		'#'
//	|		'$'
//	|		'&'
	;

comparativeOp:		OPERATOR_LE
	|		OPERATOR_GE
	|		OPERATOR_EQ
	|		OPERATOR_DIF
	|		'<'
	|		'>'
	;

%%



int yyerror(char *msg){
	fprintf(stderr, "Syntax Error on the line [%d]\n", getLineNumber());
	exit(3); // exiting status when a syntax error occurs
}
