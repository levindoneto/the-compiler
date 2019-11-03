#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

#define INIT 0

void checkUndeclared(void);
int getSemanticErrors(void);
void checkAndSetTypes(AST*node);
void checkOperands(AST* node);
int isBool(AST* node);
int isNotBool(AST* node);
int confirmType(AST* node, int datatype);
int checkReturn(AST* node, int datatype);
int verifyReturn(AST* node);

#endif
