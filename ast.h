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

typedef struct ast_node {
    int type;
    HASHTABLE_NODE* symbol;
    struct ast_node *son[MAX_SONS];
} AST;

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

#endif
