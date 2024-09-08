#ifndef PARSER_H
#define PARSER_H

#include "nodes.h"

/*
*
*
*All Parse function headers
*
*/
int parseProcedure(struct nodeProcedure *p);

int parseFuncSeq(struct nodeFuncSeq *p);

int parseFunc(struct nodeFunc *p);

int parseParams(struct nodeParams *p);

int parseDeclSeq(struct nodeDeclSeq *p);

int parseDecl(struct nodeDecl *p);

int parseDeclInt(struct nodeDeclInt *p);

int parseDeclRec(struct nodeDeclRec *p);

int parseStmtSeq(struct nodeStmtSeq *p);

int parseStmt(struct nodeStmt *p);

int parseCall(struct nodeCall *p);

int parseAssign(struct nodeAssign *p);

int parseIndex(struct nodeIndex *p);

int parseOut(struct nodeOut *p);

int parseIf(struct nodeIf *p);

int parseLoop(struct nodeLoop *p);

int parseCond(struct nodeCond *p);

int parseCmpr(struct nodeCmpr *p);

int parseExpr(struct nodeExpr *p);

int parseTerm(struct nodeTerm *p);

int parseFactor(struct nodeFactor *p);

#endif