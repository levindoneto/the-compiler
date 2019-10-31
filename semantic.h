#ifndef SEMANTIC_H
#define SEMANTIC_H

#define INIT 0

void checkUndeclared(void);
int getSemanticErrors(void);
void checkAndSetTypes(AST*node);
void checkOperands(AST* node);

#endif
