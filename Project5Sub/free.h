#ifndef FREE_H
#define FREE_H

#include "nodes.h"

/*
*
* Put headers for free functions here
*
*/

void freeProcedure(struct nodeProcedure *p);

void freeFuncSeq(struct nodeFuncSeq *p);

void freeFunc(struct nodeFunc *p);

void freeParams(struct nodeParams *p);

void freeDeclSeq(struct nodeDeclSeq *p);

void freeDecl(struct nodeDecl *p);

void freeDeclInt(struct nodeDeclInt *p);

void freeDeclRec(struct nodeDeclRec *p);

void freeStmtSeq(struct nodeStmtSeq *p);

void freeStmt(struct nodeStmt *p);

void freeAssign(struct nodeAssign *p);

void freeIndex(struct nodeIndex *p);

void freeOut(struct nodeOut *p);

void freeCall(struct nodeCall *p);

void freeIf(struct nodeIf *p);

void freeLoop(struct nodeLoop *p);

void freeCond(struct nodeCond *p);

void freeCmpr(struct nodeCmpr *p);

void freeExpr(struct nodeExpr *p);

void freeTerm(struct nodeTerm *p);

void freeFactor(struct nodeFactor *p);


#endif