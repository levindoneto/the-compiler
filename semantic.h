#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

#define INIT 0
#define SON_LEFT 0
#define SON_RIGHT 1
#define MAX_COMPARE 2

void checkUndeclared(void);
int getSemanticErrors(void);
void checkAndSetTypes(AST*node);
void checkOperands(AST* node);
int isBool(AST* node);
int isNotBool(AST* node);
int confirmType(AST* node, int datatype);
int checkReturn(AST* node, int datatype);
int verifyReturn(AST* node);
int isArithmetic(AST* node);
int isNotArithmetic(AST* node);
int checkFunctionParams(AST* node);


#endif
