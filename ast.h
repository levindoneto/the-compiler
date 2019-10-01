#ifndef AST_HEADER
#define AST_HEADER

#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SON_ZERO 0
#define SON_ONE 1
#define SON_TWO 2
#define SON_THREE 3

// Maximum number of leaves
#define MAX_SONS 4

#define AST_SYMBOL 0
#define AST_DECLARATION 1
#define AST_VARIABLEDECLARATION 2
#define AST_VECTORDECLARATION 3

// variableType
#define AST_BOOL 4
#define AST_BYTE 5
#define AST_INT 6
#define AST_LONG 7
#define AST_FLOAT 8

#define AST_VECTORVALUE 9
#define AST_VECTORREMAINDER 10
#define AST_FUNCTIONDECLARATION 11
#define AST_PARAMLIST 12
#define AST_REMAINDER 13
#define AST_PARAM 14
#define AST_COMMANDBLOCK 15
#define AST_COMMANDREMAINDER 16

// command
#define AST_ATTR 17
#define AST_VECTORATTR 18
#define AST_READ 19
#define AST_PRINT 20
#define AST_RETURN 21
#define AST_FLUXCONTROL 22

// fluxControl
#define AST_IFTHEN 23
#define AST_IFTHENELSE 24
#define AST_WHILE 25
#define AST_FOR 26
#define AST_BREAK 27

// printValue
#define AST_PRINTVALUE 28
#define AST_PRINTFINAL 29

// expression
#define AST_FUNCTIONCALL 30
#define AST_VECTORPOS 31
#define AST_PARENTHESIS 32
#define AST_ADD 33
#define AST_SUB 34
#define AST_MUL 35
#define AST_DIV 36
#define AST_LESS 37
#define AST_GREATER 38
#define AST_AND 39
#define AST_OR 40
#define AST_NOT 41
#define AST_LE 42
#define AST_GE 43
#define AST_EQ 44
#define AST_DIFF 45

#define AST_ARGLIST 46
#define AST_ARGREMAINDER 47
#define AST_DECLARATIONLIST 48

typedef struct ast_node {
    int type;
    HASHTABLE_NODE* symbol;
    struct ast_node *son[MAX_SONS];
} AST;

AST* ast; // used for making program

/* Function for creating a node in the tree.
 * @param {int} type.
 * @param {HASH_NODE*} type.
 * @param {AST*} son0.
 * @param {AST*} son1.
 * @param {AST*} son2.
 * @param {AST*} son3.
 * @return {AST_NODE*} tree
 *
 */
AST* astCreate(int type, HASHTABLE_NODE* symbol, AST* son0, AST* son1, AST* son2, AST* son3);

/* Function for printing the tree.
 * @param {AST_NODE*} node.
 * @return {int} level to print to.
 *
 */
void astPrint(AST* node, int level);


/* Function for making simular program.
 * @param {AST_NODE*} node.
 * @return {FILE*} output.
 *
 */
void astreeProgram(AST* node, FILE* output);

#endif
