#include "tac.h"
#include <stdio.h>
#include <stdlib.h>

TAC* tacGenerate(AST *node, HASHTABLE_NODE *jmpLoop) {
	int i;
	TAC *result[MAX_SONS];
	if (!node) {
		return 0;
	}
	
	if(node->type == AST_WHILE || node->type == AST_FOR) {
		jmpLoop = makeLabel();
	}

	for (i = 0; i < MAX_SONS; i++) {
		result[i] = tacGenerate(node->son[i],jmpLoop);
	}

	switch (node->type) {
		case AST_DECLARATIONLIST:
			return tacJoin(result[0], result[1]);
		break;
		
		case AST_VARIABLEDECLARATION:
			return tacJoin(tacCreate(TAC_ATTR, node->symbol, result[1]?result[1]->res:0, 0, 0, node),tacJoin(tacCreate(TAC_SYMBOL, node->symbol, 0, 0, 0, node), result[1]));
		break;
		
		case AST_VECTORDECLARATION:
			if (node->son[2]) {
				return tacJoin(tacJoin(tacJoin(tacCreate(TAC_VEC_ATTR_INIT, node->symbol, 0, 0, 0, node), result[1]), result[2]), tacCreate(TAC_VEC_ATTR_END, 0, 0, 0, 0, node));
			} else {
				return tacCreate(TAC_SYMBOL_VEC, node->symbol, result[1]?result[1]->res : 0 , 0, 0, node);
			}
		break;
		
		case AST_FUNCTIONDECLARATION:
			return functionDec(node, result[1], result[2]);
		break;
		
		case AST_PARAMLIST:
		case AST_REMAINDER:
		case AST_VECTORVALUE:
		case AST_VECTORREMAINDER:
		case AST_COMMANDBLOCK:
		case AST_COMMANDREMAINDER:
		case AST_ARGLIST:
		case AST_ARGREMAINDER:
			return tacJoin(result[0], result[1]);
		break;
		
		case AST_PARAM:
			return tacCreate(TAC_PARAM, node->symbol, 0, 0, 0, node);
		break;
		
		case AST_ATTR:
			return tacJoin(result[0],tacCreate(TAC_ATTR, node->symbol, result[0]?result[0]->res : 0, 0, 0, node));
		break;
		
		case AST_VECTORATTR:
			return tacJoin(tacJoin(result[0], result[1]), tacCreate(TAC_VEC_ATTR, node->symbol, result[1]?result[1]->res:0, result[0]?result[0]->res:0, 0, node));
		break;
				
		case AST_READ:
			return tacCreate(TAC_READ, node->symbol, 0, 0, 0, node);
		break;
		
		case AST_PRINT:
			return tacJoin(tacCreate(TAC_PRINT, 0, 0, 0, 0, node), result[0]);
		break;
		
		case AST_PRINTVALUE:
			return tacJoin(tacCreate(TAC_PRINT_PARAM, result[0]?result[0]->res:0, 0, 0, 0, node),result[1]);
		break;
		
		case AST_PRINTFINAL:
			return tacCreate(TAC_PRINT_PARAM, result[0]?result[0]->res:0, 0, 0, 0, node);
		break;
		
		case AST_RETURN:
			return tacJoin(result[0], tacCreate(TAC_RET, result[0]?result[0]->res:0, 0, 0, 0, node) );
		break;
		
		case AST_FLUXCONTROL:
		case AST_PARENTHESIS:
			return result[0];
		break;
		
		case AST_IFTHEN:
			return makeIf(result[0], result[1], node);
		break;
		
		case AST_IFTHENELSE:
			return makeIfElse(result[0], result[1], result[2], node);
		break;
		
		case AST_WHILE:
			return makeWhile(result[0], result[1], jmpLoop, node);
		break;
		
		case AST_FOR:
			return makeFor(result[0], result[1], result[2], result[3], jmpLoop, node);
		break;
		
		case AST_BREAK:
			return tacCreate(TAC_BREAK, 0, 0, 0, 0, node);
		break;
		
		case AST_FUNCTIONCALL:
			return makeCall(node, result[0]);
		break;
		
		case AST_VECTORPOS:
			return tacJoin(result[0], tacCreate(TAC_ARRAY, makeTemp(), node->symbol, result[0]?result[0]->res : 0, 0, node));
		break;
		
		case AST_SYMBOL:
			return tacCreate(TAC_SYMBOL, node->symbol, 0, 0, 0, node);
		break;
		
		case AST_ADD:
			return makeBinaryOperation(TAC_ADD, result[0], result[1], node);
		break;
		
		case AST_SUB:
			return makeBinaryOperation(TAC_SUB, result[0], result[1], node);
		break;
		
		case AST_MUL:
			return makeBinaryOperation(TAC_MULT, result[0], result[1], node);
		break;
		
		case AST_DIV:
			return makeBinaryOperation(TAC_DIV, result[0], result[1], node);
		break;
		
		case AST_LESS:
			return makeBinaryOperation(TAC_LESS, result[1], result[0], node);
		break;
		
		case AST_GREATER:
			return makeBinaryOperation(TAC_GREATER, result[0], result[1], node);
		break;
		
		case AST_AND:
			return makeBinaryOperation(TAC_AND, result[0], result[1], node);
		break;
		
		case AST_OR:
			return makeBinaryOperation(TAC_OR, result[0], result[1], node);
		break;
		
		case AST_NOT:
			return makeBinaryOperation(TAC_NOT, result[0], result[1], node);
		break;
		
		case AST_LE:
			return makeBinaryOperation(TAC_LE, result[0], result[1], node);
		break;
		
		case AST_GE:
			return makeBinaryOperation(TAC_GE, result[0], result[1], node);
		break;
		
		case AST_EQ:
			return makeBinaryOperation(TAC_EQ, result[0], result[1], node);
		break;
		
		case AST_DIFF:
			return makeBinaryOperation(TAC_DIFF, result[0], result[1], node);
		break;
		
	}
	return 0;
}

TAC* functionDec(AST *node, TAC *params, TAC *block)
{
  HASHTABLE_NODE *fBegin = makeLabel();
  HASHTABLE_NODE *fEnd = makeLabel();
  TAC *funBegin = tacCreate(TAC_BEGINFUN, node->symbol, fBegin , 0, 0, node);
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

  int numParams = 0;
  TAC *count = listParam;
  while(count){
  	numParams++;
	count = count->next;
  }
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
        case TAC_BREAK:
            fprintf(stderr, "TAC_BREAK");
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
        case TAC_WHILE:
            fprintf(stderr, "TAC_WHILE");
        break;
        case TAC_FOR:
            fprintf(stderr, "TAC_FOR");
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

TAC* makeWhile(TAC *expr, TAC *cmd, HASHTABLE_NODE *jmpLoop, AST* node)
{
	HASHTABLE_NODE *jmpFalse = makeLabel();
	HASHTABLE_NODE *jmpBegin = jmpLoop;
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


TAC* makeFor(TAC *expr, TAC *expr1, TAC *expr2, TAC *cmd, HASHTABLE_NODE *jmpLoop, AST* node){
	HASHTABLE_NODE *jmpFalse = makeLabel();
	HASHTABLE_NODE *jmpBegin = jmpLoop;
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


TAC* tacReverse(TAC *tac)
{
  if(!tac)
  {
    return 0;
  }
  for(; tac->prev; tac = tac->prev)
  {
    tac->prev->next = tac;
  }
  return tac;
}

