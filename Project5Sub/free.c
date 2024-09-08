#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "core.h"
#include "nodes.h"
#include "free.h"
#include "parser.h"


/*
*
* Free functions go here just does a recursive decsent and frees all the nodes
* Almost identincal to the print functions, just no printing
*
*/

void freeProcedure(struct nodeProcedure *p){

	free(p->pid);

	if(p->funcSeq != NULL){
		freeFuncSeq(p->funcSeq);
		free(p->funcSeq);
	}

	freeDeclSeq(p->declSeq);
	free(p->declSeq);
	
	freeStmtSeq(p->stmtSeq);
	free(p->stmtSeq);
}

void freeFuncSeq(struct nodeFuncSeq *p){
    freeFunc(p->func);
	free(p->func);
	if(p->has == 1){
		freeFuncSeq(p->funcSeq);
		free(p->funcSeq);
	}
}

void freeFunc(struct nodeFunc *p){
    freeParams(p->params);
	free(p->params);

	free(p->pid);

	freeDeclSeq(p->declSeq);
	free(p->declSeq);
	
	freeStmtSeq(p->stmtSeq);
	free(p->stmtSeq);
}

void freeParams(struct nodeParams *p){
	free(p->param1);
	if(p->param2 != NULL){
		free(p->param2);
	}
}

void freeDeclSeq(struct nodeDeclSeq *p){
    freeDecl(p->decl);
	free(p->decl);
	if(p->has == 1){
		freeDeclSeq(p->declSeq);
		free(p->declSeq);
	}
}

void freeDecl(struct nodeDecl *p){
	if(p->rec == 0){
		freeDeclInt(p->declInt);
		free(p->declInt);
	}
	else{
		freeDeclRec(p->declRec);
		free(p->declRec);
	}
}

void freeDeclInt(struct nodeDeclInt *p){
	free(p->id);
}

void freeDeclRec(struct nodeDeclRec *p){
	free(p->id);
}

void freeStmtSeq(struct nodeStmtSeq *p){
    freeStmt(p->stmt);
	free(p->stmt);
	if(p->stmtSeq != NULL){
		freeStmtSeq(p->stmtSeq);
		free(p->stmtSeq);
	}
}

void freeStmt(struct nodeStmt *p){
	if(p->assign != NULL){
		freeAssign(p->assign);
		free(p->assign);
	}
	else if(p->ifStmt != NULL){
		freeIf(p->ifStmt);
		free(p->ifStmt);
	}
	else if(p->loop != NULL){
		freeLoop(p->loop);
		free(p->loop);
	}
	else if(p->out != NULL){
		freeOut(p->out);
		free(p->out);
	}
}

void freeAssign(struct nodeAssign *p){

	free(p->id);
	if(p->type == 1){
		freeIndex(p->index);
		free(p->index);
		freeExpr(p->expr);
		free(p->expr);
	}
	else{
		if(p->type == 2){
			freeExpr(p->expr);
			free(p->expr);
		}
		else{
			free(p->id2);
		}

	}
}

void freeIf(struct nodeIf *p){
	freeCond(p->cond);
	free(p->cond);
	freeStmtSeq(p->stmtSeq1);
	free(p->stmtSeq1);
	if(p->stmtSeq2 != NULL){
		freeStmtSeq(p->stmtSeq2);
		free(p->stmtSeq2);
	}
}

void freeLoop(struct nodeLoop *p){
	freeCond(p->cond);
	free(p->cond);
	freeStmtSeq(p->stmtSeq);
	free(p->stmtSeq);
}

void freeOut(struct nodeOut *p){
	freeExpr(p->expr);
	free(p->expr);
}

void freeCall(struct nodeCall *p){
	free(p->fid);
	free(p->params);
}

void freeIndex(struct nodeIndex *p){
	if(p->empty == 1){
		freeExpr(p->expr);
		free(p->expr);
	}
}

void freeCond(struct nodeCond *p){
	if(p->op == NOT){
		freeCond(p->cond);
		free(p->cond);
	}
	else{
		freeCmpr(p->cmpr);
		free(p->cmpr);
		if(p->op == OR){
			freeCond(p->cond);
			free(p->cond);
		}
		else if (p->op == AND){
			freeCond(p->cond);
			free(p->cond);
		}
	}
}

void freeCmpr(struct nodeCmpr *p){
	freeExpr(p->expr1);
	free(p->expr1);
	if(p->op == EQUAL){
		freeExpr(p->expr2);
		free(p->expr2);
	}
	else{
		freeExpr(p->expr2);
		free(p->expr2);
	}
}

void freeExpr(struct nodeExpr *p){
	freeTerm(p->term);
	free(p->term);
	if(p->op == ADD){
		freeExpr(p->expr);
		free(p->expr);
	}
	else if(p->op == SUBTRACT){
		freeExpr(p->expr);
		free(p->expr);
	}
}

void freeTerm(struct nodeTerm *p){
	freeFactor(p->factor);
	free(p->factor);
	if(p->op == MULTIPLY){
		freeTerm(p->term);
		free(p->term);
	}
	else if(p->op == DIVIDE){
		freeTerm(p->term);
		free(p->term);
	}
}

void freeFactor(struct nodeFactor *p){
	if(p->id != NULL){
		free(p->id);
		if(p->expr != NULL){
			freeExpr(p->expr);
			free(p->expr);
		}
	}
	else if(p->expr != NULL){
		freeExpr(p->expr);
		free(p->expr);
	}
}
