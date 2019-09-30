#include "ast.h"

AST_NODE* astCreate(int type, HASHTABLE_NODE* symbol, AST_NODE* son0, AST_NODE* son1, AST_NODE* son2, AST_NODE* son3) {
    AST_NODE* newNode = 0;
    newNode = (AST*) calloc(DEFAULT_N_OBJECTS, sizeof(AST)); // DEFAULT_N_OBJECTS from hash.h
    newNode->type = type;
    newNode->symbol = symbol;
    newNode->son[SON_ZERO] = son0;
    newNode->son[SON_ONE] = son1;
    newNode->son[SON_TWO] = son2;
    newNode->son[SON_THREE] = son3;

    return newnode;
}

void astPrint(AST_NODE *node, int level) {
    if(node == 0) {
        return;
    }
    fprintf(stderr, "AST(");
    switch (node->type) {
        case '0': #TODO: ADD AST_LIT_INTEGER
            fprintf(stderr, "AST_LIT_INTEGER,");
            break
        default:
            break;
    }
    for (i = 0; i < MAX_SONS; ++i) {
        astPrint(node->son[i], level+1);
    }
}
