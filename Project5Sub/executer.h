#ifndef EXECUTER_H
#define EXECUTER_H

#include "nodes.h"

/*
*
* Put headers for execute functions here
*
*/

void executeProcedure(struct nodeProcedure *p, char *filename);

void executeFuncSeq(struct nodeFuncSeq *p);

void executeFunc(struct nodeFunc *p, struct nodeParams *c);

void executeParams(struct nodeParams *p);

void executeDeclSeq(struct nodeDeclSeq *p);

void executeDecl(struct nodeDecl *p);

void executeDeclInt(struct nodeDeclInt *p);

void executeDeclRec(struct nodeDeclRec *p);

void executeStmtSeq(struct nodeStmtSeq *p);

void executeStmt(struct nodeStmt *p);

void executeAssign(struct nodeAssign *p);

int executeIndex(struct nodeIndex *p);

void executeOut(struct nodeOut *p);

void executeCall(struct nodeCall *p);

void executeIf(struct nodeIf *p);

void executeLoop(struct nodeLoop *p);

int executeCond(struct nodeCond *p);

int executeCmpr(struct nodeCmpr *p);

int executeExpr(struct nodeExpr *p);

int executeTerm(struct nodeTerm *p);

int executeFactor(struct nodeFactor *p);


#endif