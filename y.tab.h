/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    KW_BYTE = 258,
    KW_INT = 259,
    KW_LONG = 260,
    KW_FLOAT = 261,
    KW_BOOL = 262,
    KW_IF = 263,
    KW_THEN = 264,
    KW_ELSE = 265,
    KW_WHILE = 266,
    KW_FOR = 267,
    KW_READ = 268,
    KW_PRINT = 269,
    KW_RETURN = 270,
    KW_BREAK = 271,
    OPERATOR_LE = 272,
    OPERATOR_GE = 273,
    OPERATOR_EQ = 274,
    OPERATOR_DIF = 275,
    TOKEN_ERROR = 276,
    TK_IDENTIFIER = 277,
    LIT_INTEGER = 278,
    LIT_FLOAT = 279,
    LIT_TRUE = 280,
    LIT_FALSE = 281,
    LIT_CHAR = 282,
    LIT_STRING = 283,
    JUSTIF = 284,
    noSemiColon = 285,
    OPERATOR_NE = 286,
    OPERATOR_OR = 287,
    OPERATOR_AND = 288
  };
#endif
/* Tokens.  */
#define KW_BYTE 258
#define KW_INT 259
#define KW_LONG 260
#define KW_FLOAT 261
#define KW_BOOL 262
#define KW_IF 263
#define KW_THEN 264
#define KW_ELSE 265
#define KW_WHILE 266
#define KW_FOR 267
#define KW_READ 268
#define KW_PRINT 269
#define KW_RETURN 270
#define KW_BREAK 271
#define OPERATOR_LE 272
#define OPERATOR_GE 273
#define OPERATOR_EQ 274
#define OPERATOR_DIF 275
#define TOKEN_ERROR 276
#define TK_IDENTIFIER 277
#define LIT_INTEGER 278
#define LIT_FLOAT 279
#define LIT_TRUE 280
#define LIT_FALSE 281
#define LIT_CHAR 282
#define LIT_STRING 283
#define JUSTIF 284
#define noSemiColon 285
#define OPERATOR_NE 286
#define OPERATOR_OR 287
#define OPERATOR_AND 288

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 14 "parser.y" /* yacc.c:1909  */

	struct hash_node* symbol;
	struct ast_node* ast;

#line 125 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
