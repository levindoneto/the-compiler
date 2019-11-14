#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 1
#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_MULT 4
#define TAC_DIV 5
#define TAC_ATTR 6
#define TAC_IFZ 7
#define TAC_LABEL 8

#define TAC_LESS 9
#define TAC_GREATER 10
#define TAC_AND 11
#define TAC_OR  12
#define TAC_NOT 13
#define TAC_LE 14
#define TAC_GE 15
#define TAC_EQ 16
#define TAC_DIFF 45
#define TAC_ARRAY 17
#define TAC_STRING 19
#define TAC_READ 20
#define TAC_PRINT 21

#define TAC_SYMBOL_LIT 22
#define TAC_RET 23
#define TAC_PARAM 24

#define TAC_BREAK 25
#define TAC_JUMP 26
#define TAC_SYMBOL_VEC 27
#define TAC_IFZ_ELSE 28
#define TAC_SYMBOL_KW 29
#define TAC_BEGINFUN 30
#define TAC_ENDFUN 31
#define TAC_VEC_ATTR 32
#define TAC_PRINT_PARAM 33
#define TAC_FUNC_PARAM 34
#define TAC_CALL_PARAM 35
#define TAC_WHILE 36
#define TAC_FOR 37

#define TAC_DEC_VAR 38
#define TAC_DEC_VEC 39
#define TAC_SYMBOL_STRING 40
#define TAC_CALL_BEGIN 41
#define TAC_CALL_END 42
#define TAC_VEC_ATTR_INIT 43
#define TAC_VEC_ATTR_END 44

typedef struct tac_struct {
  int type;
  int numOfArgs;
  HASHTABLE_NODE *res;
  HASHTABLE_NODE *op1;
  HASHTABLE_NODE *op2;
  int num;
  struct tac_struct *prev;
  struct tac_struct *next;
  AST *node;
} TAC;

TAC* tacGenerate(AST *node,HASHTABLE_NODE *jmpLoop);
TAC* tacCreate(int type, HASHTABLE_NODE *res, HASHTABLE_NODE *op1, HASHTABLE_NODE *op2, int num, AST *node);
void tacPrintSingle(TAC *tac);
void tacPrintBackwards(TAC *tac);
TAC* tacJoin(TAC *t1, TAC *t2);
TAC* tacReverse(TAC *tac);
void tacPrintForward(TAC *tac);
TAC* functionDec(AST *node, TAC* params, TAC *block);
TAC* makeCall(AST* node, TAC* listParam);
TAC *makeBinaryOperation(int op, TAC *result0, TAC *result1, AST* node);
TAC* makeIf(TAC *result0, TAC *result1, AST* node);
TAC *makeIfElse(TAC *expr, TAC *cmdIf, TAC *cmdElse, AST* node);
TAC* makeWhile(TAC *expr, TAC *cmd, HASHTABLE_NODE *jmpLeapLoop, AST* node);
TAC* makeFor(TAC *expr, TAC *expr1, TAC *expr2, TAC *cmd, HASHTABLE_NODE *jmpLeapLoop, AST* node);
TAC* tacReverse(TAC *tac);
