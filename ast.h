#ifndef AST_H
#define AST_H

#include "hash.h"

#define SON_ZERO 0
#define SON_ONE 1
#define SON_TWO 2
#define SON_THREE 3

// Maximum number of leaves
#define MAX_SONS 4

// TODO: Define here the AST Symbols

typedef struct ast_node {
    int type;
    HASH_NODE *symbol;
    struct ast_node *son[MAX_SONS];
} AST_NODE;

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
AST_NODE* astCreate(int type, HASH_NODE* symbol, AST_NODE* son0, AST_NODE* son1, AST_NODE* son2, AST_NODE* son3);

/* Function for printing the tree.
 * @param {AST_NODE*} node.
 * @return {int} level to print to.
 * 
 */
void astPrint(AST_NODE* node, int level);