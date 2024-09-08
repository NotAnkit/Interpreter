#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "core.h"
#include "scanner.h"
#include "nodes.h"
#include "parser.h"
#include "executer.h"
#include "memory.h"

/*
*
* Execute functions go here
*
*/

/*
*Each function is just executing its corresponding node
*Similar structure where it goes down the tree
*and prints the terminals
*Based on the nodes some terminals can be executed specific to it
*/

void executeProcedure(struct nodeProcedure *p, char *filename){
	memory_init();
	scanner_open(filename);

	executeDeclSeq(p->declSeq);
	
	executeStmtSeq(p->stmtSeq);
	popStack();
}

void executeFunc(struct nodeFunc *p, struct nodeParams *c){
	memory_init();
	executeParams(p->params);
	setParams(p->params, c);

	executeDeclSeq(p->declSeq);
	
	executeStmtSeq(p->stmtSeq);
}

void executeParams(struct nodeParams *p){
	declare(p->param1, 0);
	if(p->param2 != NULL){
		declare(p->param2, 0);
	}
}

void executeDeclSeq(struct nodeDeclSeq *p){
    executeDecl(p->decl);
	if(p->has == 1){
		executeDeclSeq(p->declSeq);
	}
}

void executeDecl(struct nodeDecl *p){

	if(p->rec == 0){
		executeDeclInt(p->declInt);
	}
	else{
		executeDeclRec(p->declRec);
	}
}

void executeDeclInt(struct nodeDeclInt *p){
	declare(p->id, 1);
}

void executeDeclRec(struct nodeDeclRec *p){
	declare(p->id, 0);
}

void executeStmtSeq(struct nodeStmtSeq *p){
    executeStmt(p->stmt);
	if(p->has == 1){
		executeStmtSeq(p->stmtSeq);
	}
}

void executeStmt(struct nodeStmt *p){

	if(p->assign != NULL){
		executeAssign(p->assign);
	}
	else if(p->ifStmt != NULL){
		executeIf(p->ifStmt);
	}
	else if(p->loop != NULL){
		executeLoop(p->loop);
	}
	else if(p->out != NULL){
		executeOut(p->out);
	}
	else if(p->call != NULL){
		executeCall(p->call);
	}
}

//Based on the case picks which of the assign functions it should use.
void executeAssign(struct nodeAssign *p){

	if(p->type == 1){
		int index = executeIndex(p->index);
		int value = executeExpr(p->expr);
		if(index == -1){
			store(p->id, value);
		}
		else{
			storeRec(p->id, index, value);
		}

	}
	else{
		if(p->type == 2){
			allocateRecord(p->id, executeExpr(p->expr));
		}
		else{
			record(p->id, p->id2);
		}

	}
}

//Cond returns 1 if true and only runs else if there is another stmt
void executeIf(struct nodeIf *p){
	
	if(executeCond(p->cond) == 1){
		executeStmtSeq(p->stmtSeq1);
	}
	else if(p->stmtSeq2 != NULL){
		executeStmtSeq(p->stmtSeq2);
	}
}

//While loop while the cond is true
void executeLoop(struct nodeLoop *p){

	while(executeCond(p->cond) == 1){
		executeStmtSeq(p->stmtSeq);
	}
}

//This the languages print function
void executeOut(struct nodeOut *p){
	
	int value = executeExpr(p->expr);
	printf("%d\n", value);
}

void executeCall(struct nodeCall *p){
	struct nodeFunc* function = getFunc(p->fid);
	executeFunc(function, p->params);
	popStack();
}

//For index of a record returns -1 if empty to make sure to pick the right case
int executeIndex(struct nodeIndex *p){
	
	if(p->empty == 1){
		return executeExpr(p->expr);
	}
	return -1;
}

//Return 1 if true and 0 if false
int executeCond(struct nodeCond *p){
	int value;
	if(p->op == NOT){
		value = executeCond(p->cond);
		if(value == 1){
			value = 0;
		}
		else{
			value = 1;
		}
	}
	else{
		value = executeCmpr(p->cmpr);
		if(p->op == OR){
			if(value == 1 || executeCond(p->cond) == 1){
				value = 1;
			}
			else{
				value = 0;
			}
		}
		else if (p->op == AND){
			if(value == 1 && executeCond(p->cond) == 1){
				value = 1;
			}
			else{
				value = 0;
			}
		}
	}
	return value;
}

//Return 1 if true and 0 if false
int executeCmpr(struct nodeCmpr *p){

	int value = executeExpr(p->expr1);

	if(p->op == EQUAL){
		if(value == executeExpr(p->expr2)){
			value = 1;
		}
		else{
			value = 0;
		}
		
	}
	else{
		if(value < executeExpr(p->expr2)){
			value = 1;
		}
		else{
			value = 0;
		}
	}
	return value;
}

//Does the expr calc
int executeExpr(struct nodeExpr *p){

	int value = executeTerm(p->term);
	if(p->op == ADD){
		value += executeExpr(p->expr);
	}
	else if(p->op == SUBTRACT){
		value -= executeExpr(p->expr);
	}
	return value;
}

//Does the term calc and checks for divide by 0
int executeTerm(struct nodeTerm *p){

	int value = executeFactor(p->factor);
	if(p->op == MULTIPLY){
		value *= executeTerm(p->term);
	}
	else if(p->op == DIVIDE){
		int value2 = executeTerm(p->term);
		if(value2 == 0){
			printf("ERROR: Divide by zero\n");
			exit(0);
		}
		value /= value2;
	}
	return value;
}

//All the calls related to factor
int executeFactor(struct nodeFactor *p){

	if(p->id != NULL){
		if(p->expr != NULL){
			return recallRec(p->id, executeExpr(p->expr));
		}
		return recall(p->id);
	}
	else if(p->constNum != 1100){
		return p->constNum;
	}
	else if(p->expr != NULL){
		return executeExpr(p->expr);
	}
	else if(p->in != 0){
		if(currentToken() == EOS){
			printf("ERROR: no more data in the file \n");
			exit(0);
		}
		int value = getConst();
		nextToken();
		return value;
	}
}
