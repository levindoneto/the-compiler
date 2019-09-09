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

%nonassoc JUSTIF
%nonassoc KW_ELSE


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


functionDeclaration: variableType TK_IDENTIFIER '(' paramList ')' commandBlock
        ;

paramList:		param remainder
	|
	;

remainder:		',' param remainder
	|
	;

param:			variableType TK_IDENTIFIER
	;


commandBlock:		'{' command commandRemainder '}'
	;

commandRemainder:	';' command commandRemainder
	|
	;

command: 		assignement
	| 		fluxControl
	|		readComm
	|		printComm
	|		returnComm
	|
        ;

assignement:		TK_IDENTIFIER '=' expression
	|		TK_IDENTIFIER '[' expression ']' '=' expression
	;

fluxControl:		KW_IF 		'(' expression ')' KW_THEN command %prec JUSTIF
	|		KW_IF 		'(' expression ')' KW_THEN command KW_ELSE command
	|		KW_WHILE	'(' expression ')' 	   command KW_BREAK
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

expression:		arithmetic
	|		booleanExp
	|		TK_IDENTIFIER '(' paramList ')'
	;

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
	;

identifiers:		TK_IDENTIFIER
	|		TK_IDENTIFIER '[' expression ']'
	;

arithmeticOp:		'*'
	|		'/'
	|		'+'
	|		'-'
	;

booleanExp:		booleanValues booleanRemainder
	|		arithmetic comparativeOp arithmetic
	;

booleanRemainder:	booleanOp booleanValues booleanRemainder
	|		
	;

booleanValues:		LIT_FALSE
	|		LIT_TRUE
	;

booleanOp:		'.'
	|		'v'
	|		'~'
	;

comparativeOp:		OPERATOR_LE
	|		OPERATOR_GE
	|		OPERATOR_EQ
	|		'<'
	|		'>'
	;

%%



int yyerror(char *msg){
	fprintf(stderr, "Syntax Error on the line [%d]\n", getLineNumber());
	exit(3); // exiting status when a syntax error occurs
}
