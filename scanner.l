%{
#include "tokens.h"

int lineNumber = 1;

#include "hash.c"

%}

%x Comm

NMR [0-9]
BAS [a-zA-Z_]

%%

[ \t\r]                 		

"byte"					{return KW_BYTE;}
"int"					{return KW_INT;}
"long"					{return KW_LONG;}
"float"					{return KW_FLOAT;}
"bool"					{return KW_BOOL;}

"if"					{return KW_IF;}
"then"					{return KW_THEN;}
"else"					{return KW_ELSE;}
"while"					{return KW_WHILE;}
"for"					{return KW_FOR;}
"read"					{return KW_READ;}
"print"					{return KW_PRINT;}
"return"				{return KW_RETURN;}
"break"					{return KW_BREAK;}

{NMR}+					{hashInsert(yytext, LIT_INTEGER); return LIT_INTEGER;}
({NMR}+\.{NMR}*|{NMR}*\.{NMR}+)		{hashInsert(yytext, LIT_FLOAT); return LIT_FLOAT;}
['].[']					{hashInsert(yytext, LIT_CHAR); return LIT_CHAR;}
["](\\\"|[^\"\n])*["]			{hashInsert(yytext, LIT_STRING); return LIT_STRING;}
"TRUE"					{hashInsert(yytext, LIT_TRUE); return LIT_TRUE;}
"FALSE"					{hashInsert(yytext, LIT_FALSE); return LIT_FALSE;}

{BAS}[a-zA-Z_0-9]*			{hashInsert(yytext, TK_IDENTIFIER); return TK_IDENTIFIER;}

">="					{return OPERATOR_GE;}
"<="					{return OPERATOR_LE;}
"=="					{return OPERATOR_EQ;}
"!="					{return OPERATOR_DIF;}


"//".*
"###" 					BEGIN(Comm);
<Comm>"###"				BEGIN(INITIAL);
<Comm>"\n"				{lineNumber++;}
<Comm>.

[,;:\(\)\[\]\{\}\+\-*/<>=v\.~&$#]	{return yytext[0];}

"\n"					{lineNumber++;}
.					{return TOKEN_ERROR;}

%%

#include "main.c"

int running = 1;

int yywrap() {
	running = 0;
	return 1;
}

void initMe() {
	hashInitialize();
}

int getLineNumber() {
	return lineNumber;
}

int isRunning() {
	return running;
}

