#include "tac.h"
#include <stdio.h>
#include <stdlib.h>


TAC* functionDec(AST *node, TAC *params, TAC *block)
{
  HASH_NODE *fBegin = makeLabel();
  HASH_NODE *fEnd = makeLabel();
  TAC *funBegin = tacCreate(TAC_BEGINFUN, node->symbol, fBegin , 0, (int)node->value);
  TAC *funEnd = tacCreate(TAC_ENDFUN, node->symbol, fEnd,0, 0);
  return tacJoin(tacJoin( tacJoin(funBegin,params), block), funEnd);
}


TAC* tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2, int num)
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
  HASH_NODE* labelBegin = makeLabel();
  HASH_NODE* labelEnd = makeLabel();
  TAC* callEnd = tacCreate(TAC_CALL_END, node->symbol, labelEnd, 0, 0);

  int numParams = (int)node->value;
  TAC *aux = listParam;
  TAC *p  = aux;
  int done = 0;

  if (numParams == 0)
  {
    return tacJoin(tacCreate(TAC_CALL_BEGIN, node->symbol, labelBegin,0,0), callEnd);
  }
  else if (numParams == 1)
  {
    return tacJoin(tacJoin(tacJoin(p, tacCreate(TAC_CALL_BEGIN, node->symbol, labelBegin,0,1)),tacCreate(TAC_CALL_PARAM, p->res, 0, 0, 0)), callEnd);
  }
  else if (numParams > 1)
  {
    for (int i = 0; i < numParams; i++)
    {
      p = tacJoin(p,tacCreate(TAC_CALL_PARAM, aux->res?aux->res : 0, 0, 0, numParams - i));
      aux = aux->prev;
    }   
  }
  return tacJoin(tacJoin(tacCreate(TAC_CALL_BEGIN, node->symbol, labelBegin,0,(int)node->value), p), callEnd);
}


TAC* makeBinaryOperation(int op, TAC *result0, TAC *result1)
{
    TAC* opRes = tacCreate(op, makeTemp() , result0?result0->res:0, result1?result1->res:0,0);
    return tacJoin( tacJoin(result0, result1), opRes);

}


TAC* makeIf(TAC *result0, TAC *result1)
{
  TAC *newIfTac = 0;
  TAC *newLabelTac = 0;
  HASH_NODE *newLabel = 0;
  newLabel = makeLabel();
  newIfTac = tacCreate(TAC_IFZ, result0?result0->res:0, newLabel , 0,0);
  newLabelTac = tacCreate(TAC_LABEL, newLabel, 0, 0,0);
  return tacJoin(tacJoin( tacJoin(result0 , newIfTac), result1), newLabelTac);
}


TAC *makeIfElse(TAC *expr, TAC *cmdIf, TAC *cmdElse)
{
  HASH_NODE *labelElse = makeLabel();
  HASH_NODE *labelContinue = makeLabel();
  TAC *newIfElseTac = 0;
  TAC *lbContinue = 0;
  TAC *lbElse = 0;
  TAC *jmp = 0;
  newIfElseTac = tacCreate(TAC_IFZ_ELSE, expr?expr->res:0, labelElse, 0,0);
  lbContinue = tacCreate(TAC_LABEL, labelContinue, 0,0,0);
  lbElse = tacCreate(TAC_LABEL, labelElse, 0, 0,0);
  jmp = tacCreate(TAC_JUMP, labelContinue, 0, 0,0);
  return tacJoin(tacJoin( tacJoin(tacJoin(tacJoin(tacJoin(expr, newIfElseTac), cmdIf), jmp), lbElse), cmdElse ),lbContinue);
}

