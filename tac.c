#include "tac.h"
#include <stdio.h>
#include <stdlib.h>


TAC* functionDec(AST *node, TAC *params, TAC *block)
{
  HASHTABLE_NODE *fBegin = makeLabel();
  HASHTABLE_NODE *fEnd = makeLabel();
  TAC *funBegin = tacCreate(TAC_BEGINFUN, node->symbol, fBegin , 0, (int)node->type, node);
  TAC *funEnd = tacCreate(TAC_ENDFUN, node->symbol, fEnd,0, 0, node);
  return tacJoin(tacJoin( tacJoin(funBegin,params), block), funEnd);
}


TAC* tacCreate(int type, HASHTABLE_NODE *res, HASHTABLE_NODE *op1, HASHTABLE_NODE *op2, int num, AST *node)
{
  TAC *newTac;
  newTac = (TAC*)calloc(1, sizeof(TAC));
  newTac->type = type;
  newTac->num = num;
  newTac->res = res;
  newTac->op1 = op1;
  newTac->op2 = op2;
  newTac->prev = 0;
  newTac->next = 0;
  newTac->node = node;
  return newTac;
}


void tacPrintBackwards(TAC *tac)
{
  if (!tac)
  {
    return;
  }
  tacPrintSingle(tac);
  tacPrintBackwards(tac->prev);
}


TAC* tacJoin(TAC *t1, TAC *t2)
{
  TAC *aux = t2;
  if (!t1)
  {
      return t2;
  }
  if (!t2)
  {
    return t1;
  }
  while(aux->prev)
  {
    aux = aux->prev;
  }
  aux->prev = t1;
  return t2;
}


void tacPrintForward(TAC *tac)
{
  if (!tac)
  {
    return;
  }
  tacPrintSingle(tac);
  tacPrintForward(tac->next);
}


TAC* makeCall(AST* node, TAC* listParam)
{
  HASHTABLE_NODE* labelBegin = makeLabel();
  HASHTABLE_NODE* labelEnd = makeLabel();
  TAC* callEnd = tacCreate(TAC_CALL_END, node->symbol, labelEnd, 0, 0, node);

  int numParams = (int)node->type;
  TAC *aux = listParam;
  TAC *p  = aux;
  int done = 0;

  if (numParams == 0)
  {
    return tacJoin(tacCreate(TAC_CALL_BEGIN, node->symbol, labelBegin,0,0, node), callEnd);
  }
  else if (numParams == 1)
  {
    return tacJoin(tacJoin(tacJoin(p, tacCreate(TAC_CALL_BEGIN, node->symbol, labelBegin, 0, 1, node)),tacCreate(TAC_CALL_PARAM, p->res, 0, 0, 0, node)), callEnd);
  }
  else if (numParams > 1)
  {
    for (int i = 0; i < numParams; i++)
    {
      p = tacJoin(p,tacCreate(TAC_CALL_PARAM, aux->res?aux->res : 0, 0, 0, numParams - i, node));
      aux = aux->prev;
    }
  }
  return tacJoin(tacJoin(tacCreate(TAC_CALL_BEGIN, node->symbol, labelBegin,0,(int)node->type, node), p), callEnd);
}


TAC* makeBinaryOperation(int op, TAC *result0, TAC *result1, AST* node)
{
    TAC* opRes = tacCreate(op, makeTemp() , result0?result0->res:0, result1?result1->res:0,0, node);
    return tacJoin(tacJoin(result0, result1), opRes);
}


TAC* makeIf(TAC *result0, TAC *result1, AST* node)
{
  TAC *newIfTac = 0;
  TAC *newLabelTac = 0;
  HASHTABLE_NODE *newLabel = 0;
  newLabel = makeLabel();
  newIfTac = tacCreate(TAC_IFZ, result0?result0->res:0, newLabel, 0,0, node);
  newLabelTac = tacCreate(TAC_LABEL, newLabel, 0, 0,0, node);
  return tacJoin(tacJoin( tacJoin(result0 , newIfTac), result1), newLabelTac);
}


TAC *makeIfElse(TAC *expr, TAC *cmdIf, TAC *cmdElse, AST* node)
{
  HASHTABLE_NODE *labelElse = makeLabel();
  HASHTABLE_NODE *labelContinue = makeLabel();
  TAC *newIfElseTac = 0;
  TAC *lbContinue = 0;
  TAC *lbElse = 0;
  TAC *jmp = 0;
  newIfElseTac = tacCreate(TAC_IFZ_ELSE, expr?expr->res:0, labelElse, 0,0, node);
  lbContinue = tacCreate(TAC_LABEL, labelContinue, 0,0,0, node);
  lbElse = tacCreate(TAC_LABEL, labelElse, 0, 0,0, node);
  jmp = tacCreate(TAC_JUMP, labelContinue, 0, 0,0, node);
  return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(expr, newIfElseTac), cmdIf), jmp), lbElse), cmdElse), lbContinue);
}

void tacPrintSingle(TAC* tac)
{
    if(!tac || tac->type == TAC_SYMBOL) {
	return;
    }
    fprintf(stderr, "TAC (");
    switch(tac->type) {
        case TAC_ADD:
            fprintf(stderr, "TAC_ADD");
        break;
        case TAC_SUB:
            fprintf(stderr, "TAC_SUB");
        break;
        case TAC_MULT:
            fprintf(stderr, "TAC_MULT");
        break;
        case TAC_DIV:
            fprintf(stderr, "TAC_DIV");
        break;
        case TAC_ATTR:
            fprintf(stderr, "TAC_ATTR");
        break;
        case TAC_IFZ:
            fprintf(stderr, "TAC_IFZ");
        break;
        case TAC_LABEL:
            fprintf(stderr, "TAC_LABEL");
        break;
        case TAC_LESS:
            fprintf(stderr, "TAC_LESS");
        break;
        case TAC_GREATER:
            fprintf(stderr, "TAC_GREATER");
        break;
        case TAC_AND:
            fprintf(stderr, "TAC_AND");
        break;
        case TAC_OR:
            fprintf(stderr, "TAC_OR");
        break;
        case TAC_NOT:
            fprintf(stderr, "TAC_NOT");
        break;
        case TAC_LE:
            fprintf(stderr, "TAC_LE");
        break;
        case TAC_GE:
            fprintf(stderr, "TAC_GE");
        break;
        case TAC_EQ:
            fprintf(stderr, "TAC_EQ");
        break;
        case TAC_ARRAY:
            fprintf(stderr, "TAC_ARRAY");
        break;
        case TAC_STRING:
            fprintf(stderr, "TAC_STRING");
        break;
        case TAC_READ:
            fprintf(stderr, "TAC_READ");
        break;
        case TAC_PRINT:
            fprintf(stderr, "TAC_PRINT");
        break;
        case TAC_SYMBOL_LIT:
            fprintf(stderr, "TAC_SYMBOL_LIT");
        break;
        case TAC_RET:
            fprintf(stderr, "TAC_RET");
        break;
        case TAC_PARAM:
            fprintf(stderr, "TAC_PARAM");
        break;
        case TAC_LEAP:
            fprintf(stderr, "TAC_LEAP");
        break;
        case TAC_JUMP:
            fprintf(stderr, "TAC_JUMP");
        break;
        case TAC_SYMBOL_VEC:
            fprintf(stderr, "TAC_SYMBOL_VEC");
        break;
        case TAC_IFZ_ELSE:
            fprintf(stderr, "TAC_IFZ_ELSE");
        break;
        case TAC_SYMBOL_KW:
            fprintf(stderr, "TAC_SYMBOL_KW");
        break;
        case TAC_BEGINFUN:
            fprintf(stderr, "TAC_BEGINFUN");
        break;
        case TAC_ENDFUN:
            fprintf(stderr, "TAC_ENDFUN");
        break;
        case TAC_VEC_ATTR:
            fprintf(stderr, "TAC_VEC_ATTR");
        break;
        case TAC_PRINT_PARAM:
            fprintf(stderr, "TAC_PRINT_PARAM");
        break;
        case TAC_FUNC_PARAM:
            fprintf(stderr, "TAC_FUNC_PARAM");
        break;
        case TAC_CALL_PARAM:
            fprintf(stderr, "TAC_CALL_PARAM");
        break;
        case TAC_LOOP:
            fprintf(stderr, "TAC_LOOP");
        break;
        case TAC_LABEL_LEAP:
            fprintf(stderr, "TAC_LABEL_LEAP");
        break;
        case TAC_DEC_VAR:
            fprintf(stderr, "TAC_DEC_VAR");
        break;
        case TAC_DEC_VEC:
            fprintf(stderr, "TAC_DEC_VEC");
        break;
        case TAC_SYMBOL_STRING:
            fprintf(stderr, "TAC_SYMBOL_STRING");
        break;
        case TAC_CALL_BEGIN:
            fprintf(stderr, "TAC_CALL_BEGIN");
        break;
        case TAC_CALL_END:
            fprintf(stderr, "TAC_CALL_END");
        break;
        case TAC_VEC_ATTR_INIT:
            fprintf(stderr, "TAC_VEC_ATTR_INIT");
        break;
        case TAC_VEC_ATTR_END:
            fprintf(stderr, "TAC_VEC_ATTR_END");
        break;
        default:
            fprintf(stderr, "TAC_UNKNOWN");
    }
    if(tac->res) {
        fprintf(stderr, ", %s", tac->res->value);
    } else  {
        fprintf(stderr, ", 0");
    }
    if(tac->op1) {
        fprintf(stderr, ", %s", tac->op1->value);
    } else {
        fprintf(stderr, ", 0");
    }
    if(tac->op2) {
        fprintf(stderr, ", %s", tac->op2->value);
    } else {
        fprintf(stderr, ", 0");
    }
    fprintf(stderr, ")\n");
}

TAC* makeWhile(TAC *expr, TAC *cmd, HASHTABLE_NODE *jmpLeapLoop, AST* node)
{
	HASHTABLE_NODE *jmpFalse = makeLabel();
	HASHTABLE_NODE *jmpBegin = jmpLeapLoop;
	TAC *newLoopTac = 0;
	TAC *labelFalse = 0;
	TAC *labelBegin = 0;
	TAC *jump = 0;
	newLoopTac = tacCreate(TAC_IFZ, jmpFalse, expr?expr->res:0, 0,0, node);
	labelFalse = tacCreate(TAC_LABEL, jmpFalse, 0, 0,0, node);
	labelBegin = tacCreate(TAC_LABEL, jmpBegin, 0, 0,0, node);
	jump = tacCreate(TAC_JUMP, jmpBegin, 0, 0,0, node);
	return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(labelBegin, expr), newLoopTac), cmd), jump), labelFalse);
}
