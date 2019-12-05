%{
	#include <stdlib.h>
	#include <stdio.h>
	#include "semantic.h"

	int getLineNumber(void);
	int yylex();
	int yyerror(char *msg);



%}

%union {
	struct hashtable_node *symbol;
	struct ast_node *ast;
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

%token<symbol> TK_IDENTIFIER
%token<symbol> LIT_INTEGER
%token<symbol> LIT_FLOAT
%token<symbol> LIT_TRUE
%token<symbol> LIT_FALSE
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING


%type<ast> program
%type<ast> declarationList
%type<ast> declaration
%type<ast> vectorSize
%type<ast> variableType
%type<ast> variableValue
%type<ast> vectorValue
%type<ast> vectorRemainder
%type<ast> paramList
%type<ast> remainder
%type<ast> param
%type<ast> commandBlock
%type<ast> commandRemainder
%type<ast> command
%type<ast> fluxControl
%type<ast> printValue
%type<ast> expression
%type<ast> argList
%type<ast> argRemainder
%type<ast> argument


%right '='
%left '+' '-'
%left '*' '/'
%left '<' '>' OPERATOR_EQ OPERATOR_DIF OPERATOR_GE OPERATOR_LE
%left '.' 'v'

%%
program:		declarationList 			{$$ = $1; ast = $$; astPrint($$, 0);checkAndSetTypes($1); checkUndeclared();checkOperands($1);}
	;

declarationList:	declaration declarationList		{$$ = astCreate(AST_DECLARATIONLIST, 0, $1, $2, 0, 0, getLineNumber());}
	|							{$$ = 0;}
	;

declaration:		variableType TK_IDENTIFIER	'=' variableValue ';'		{$$ = astCreate(AST_VARIABLEDECLARATION, $2, $1, $4, 0, 0, getLineNumber());}
	|		variableType TK_IDENTIFIER '[' vectorSize ']' vectorValue ';' 	{$$ = astCreate(AST_VECTORDECLARATION, $2, $1, $4, $6, 0, getLineNumber());}
	|		variableType TK_IDENTIFIER '(' paramList ')' commandBlock {$$ = astCreate(AST_FUNCTIONDECLARATION, $2, $1, $4, $6, 0, getLineNumber());}
        ;

//		VARIABLE DECLARATION
vectorSize:		LIT_INTEGER 				{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber());}
	;

variableType:		KW_BOOL					{$$ = astCreate(AST_BOOL, 0, 0, 0, 0, 0, getLineNumber());}
	|		KW_BYTE					{$$ = astCreate(AST_BYTE, 0, 0, 0, 0, 0, getLineNumber());}
	|		KW_INT					{$$ = astCreate(AST_INT, 0, 0, 0, 0, 0, getLineNumber());}
	|		KW_LONG					{$$ = astCreate(AST_LONG, 0, 0, 0, 0, 0, getLineNumber());}
	|		KW_FLOAT				{$$ = astCreate(AST_FLOAT, 0, 0, 0, 0, 0, getLineNumber());}
	;

vectorValue:		':' variableValue vectorRemainder 	{$$ = astCreate(AST_VECTORVALUE, 0, $2, $3, 0, 0, getLineNumber());}
	|							{$$ = 0;}
	;

vectorRemainder:	variableValue vectorRemainder 		{$$ = astCreate(AST_VECTORREMAINDER, 0, $1, $2, 0, 0, getLineNumber());}
	|							{$$ = 0;}
	;

variableValue:		LIT_INTEGER 				{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber());}
	|		LIT_FLOAT 				{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber());}
	|		LIT_CHAR				{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber());}
	|		LIT_TRUE				{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber());}
	|		LIT_FALSE				{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber());}
	;

//		FUNCTION DECLARATION
paramList:		param remainder				{$$ = astCreate(AST_PARAMLIST, 0, $1, $2, 0, 0, getLineNumber());}
	|							{$$=0;}
	;

remainder:		',' param remainder			{$$ = astCreate(AST_REMAINDER, 0, $2, $3, 0, 0, getLineNumber());}
	|							{$$=0;}
	;

param:			variableType TK_IDENTIFIER		{$$ = astCreate(AST_PARAM, $2, $1, 0, 0, 0, getLineNumber());}
	;

// COMMAND BLOCK
commandBlock:		'{' command commandRemainder '}'	{$$ = astCreate(AST_COMMANDBLOCK, 0, $2, $3, 0, 0, getLineNumber());}
	;

commandRemainder:	';' command commandRemainder		{$$ = astCreate(AST_COMMANDREMAINDER, 0, $2, $3, 0, 0, getLineNumber());}
	|							{$$ = 0;}
	;


command:		TK_IDENTIFIER '=' expression			{$$ = astCreate(AST_ATTR, $1, $3, 0, 0, 0, getLineNumber());}
	|		TK_IDENTIFIER '[' expression ']' '=' expression {$$ = astCreate(AST_VECTORATTR, $1, $3, $6, 0, 0, getLineNumber());}
	|		KW_READ	  TK_IDENTIFIER 			{$$ = astCreate(AST_READ, $2, 0, 0, 0, 0, getLineNumber());}
	|		KW_PRINT  printValue 				{$$ = astCreate(AST_PRINT, 0, $2, 0, 0, 0, getLineNumber());}
	|		KW_RETURN expression 				{$$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0, getLineNumber());}
	|		fluxControl 					{$$ = astCreate(AST_FLUXCONTROL, 0, $1, 0, 0, 0, getLineNumber());}
	|		commandBlock 					{$$ = $1;}
	|								{$$ = 0;}
        ;



//		COMMANDS
fluxControl:		KW_IF		'(' expression ')' KW_THEN command			{$$ = astCreate(AST_IFTHEN, 0, $3, $6, 0, 0, getLineNumber());}
	|		KW_IF		'(' expression ')' KW_THEN command KW_ELSE command 	{$$ = astCreate(AST_IFTHENELSE, 0, $3, $6, $8, 0, getLineNumber());}
	|		KW_WHILE	'(' expression ')' command 				{$$ = astCreate(AST_WHILE, 0, $3, $5, 0, 0, getLineNumber());}
	|		KW_FOR		'(' TK_IDENTIFIER ':' expression ',' expression ',' expression ')' command
												{$$ = astCreate(AST_FOR, $3, $5, $7, $9, $11, getLineNumber());}
	|		KW_BREAK								{$$ = astCreate(AST_BREAK, 0, 0, 0, 0, 0, getLineNumber());}
	;

printValue:		expression printValue							{$$ = astCreate(AST_PRINTVALUE, 0, $1, $2, 0, 0, getLineNumber());}
	|		expression								{$$ = astCreate(AST_PRINTFINAL, 0, $1, 0, 0, 0, getLineNumber());}
	;

//		EXPRESSIONS
expression:		argument								{$$ = $1;}
	|		LIT_STRING						  		{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber());}
	|		TK_IDENTIFIER  '(' argList ')'						{$$ = astCreate(AST_FUNCTIONCALL, $1, $3, 0, 0, 0, getLineNumber());}
	|		TK_IDENTIFIER  '[' expression ']'					{$$ = astCreate(AST_VECTORPOS, $1, $3, 0, 0, 0, getLineNumber());}
	|		'(' expression ')'							{$$ = astCreate(AST_PARENTHESIS, 0, $2, 0, 0, 0, getLineNumber());}
	|		expression 		'+' 			expression		{$$ = astCreate(AST_ADD, 0, $1, $3, 0, 0, getLineNumber());}
	|		expression 		'-' 			expression		{$$ = astCreate(AST_SUB, 0, $1, $3, 0, 0, getLineNumber());}
	|		expression 		'*' 			expression		{$$ = astCreate(AST_MUL, 0, $1, $3, 0, 0, getLineNumber());}
	|		expression 		'/' 			expression		{$$ = astCreate(AST_DIV, 0, $1, $3, 0, 0, getLineNumber());}
	|		expression 		'<' 			expression		{$$ = astCreate(AST_LESS, 0, $1, $3, 0, 0, getLineNumber());}
	|		expression 		'>' 			expression		{$$ = astCreate(AST_GREATER, 0, $1, $3, 0, 0, getLineNumber());}
	|		expression 		'.' 			expression		{$$ = astCreate(AST_AND, 0, $1, $3, 0, 0, getLineNumber());}
	|		expression 		'v' 			expression		{$$ = astCreate(AST_OR, 0, $1, $3, 0, 0, getLineNumber());}
	|					'~' 			expression		{$$ = astCreate(AST_NOT, 0, $2, 0, 0, 0, getLineNumber());}
	|		expression 		OPERATOR_LE 		expression		{$$ = astCreate(AST_LE, 0, $1, $3, 0, 0, getLineNumber());}
	|		expression 		OPERATOR_GE 		expression		{$$ = astCreate(AST_GE, 0, $1, $3, 0, 0, getLineNumber());}
	|		expression 		OPERATOR_EQ 		expression		{$$ = astCreate(AST_EQ, 0, $1, $3, 0, 0, getLineNumber());}
	|		expression 		OPERATOR_DIF	 	expression		{$$ = astCreate(AST_DIFF, 0, $1, $3, 0, 0, getLineNumber());}
	;

argList:		argument argRemainder							{$$ = astCreate(AST_ARGLIST, 0, $1, $2, 0, 0, getLineNumber());}
	|											{$$ = 0;}
	;

argRemainder:		',' argument argRemainder						{$$ = astCreate(AST_ARGREMAINDER, 0, $2, $3, 0, 0, getLineNumber());}
	|											{$$ = 0;}
	;

argument:		TK_IDENTIFIER								{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber());}
	|		variableValue								{$$ = $1;}
	;

%%


int yyerror(char *msg){
	fprintf(stderr, "Syntax Error on the line [%d]\n", getLineNumber());
	exit(3); // exiting status when a syntax error occurs
}
