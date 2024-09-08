#ifndef PRINTER_H
#define PRINTER_H

#include "nodes.h"

/*
*
* Put headers for print functions here
*
*/

int printProcedure(struct nodeProcedure *p);

int printFuncSeq(struct nodeFuncSeq *p);

int printFunc(struct nodeFunc *p);

int printParams(struct nodeParams *p);

int printDeclSeq(struct nodeDeclSeq *p);

int printDecl(struct nodeDecl *p);

int printDeclInt(struct nodeDeclInt *p);

int printDeclRec(struct nodeDeclRec *p);

int printStmtSeq(struct nodeStmtSeq *p);

int printStmt(struct nodeStmt *p);

int printAssign(struct nodeAssign *p);

int printIndex(struct nodeIndex *p);

int printOut(struct nodeOut *p);

int printCall(struct nodeCall *p);

int printIf(struct nodeIf *p);

int printLoop(struct nodeLoop *p);

int printCond(struct nodeCond *p);

int printCmpr(struct nodeCmpr *p);

int printExpr(struct nodeExpr *p);

int printTerm(struct nodeTerm *p);

int printFactor(struct nodeFactor *p);


#endif