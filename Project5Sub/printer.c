#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "core.h"
#include "nodes.h"
#include "printer.h"
#include "parser.h"

/*
*
* Helper functions
*
*/


static void indents(int indent) {
	int i = 0;
	for (i; i<indent; i++) {
		printf("  ");
	}
}

/*
*
* Print functions go here
*
*/

//Trying to do the right indents
int counter = 2;

/*
*Each function is just printing its corresponding node
*Similar structure where it goes down the tree
*and prints the terminals
*Based on the nodes some terminals can be printed specific to it
*/

int printProcedure(struct nodeProcedure *p){

	printf("procedure %s ", p->pid);
	free(p->pid);

	if(p->funcSeq != NULL){
		printf("and ");
		printFuncSeq(p->funcSeq);
		free(p->funcSeq);
	}

	printf("is ");
	
	printDeclSeq(p->declSeq);
	free(p->declSeq);
	printf("begin \n");
	
	printStmtSeq(p->stmtSeq);
	free(p->stmtSeq);
	printf("end \n");
	
    return 1;
}

int printFuncSeq(struct nodeFuncSeq *p){
	printFunc(p->func);
	free(p->func);
	if(p->has == 1){
		printFuncSeq(p->funcSeq);
		free(p->funcSeq);
	}
}

int printFunc(struct nodeFunc *p){
	printf("procedure %s (", p->pid);
	free(p->pid);
	printParams(p->params);
	free(p->params);
	printf(") is ");
	
	printDeclSeq(p->declSeq);
	free(p->declSeq);
	printf("begin \n");
	
	printStmtSeq(p->stmtSeq);
	free(p->stmtSeq);
	printf("end \n");
	
    return 1;
}

int printParams(struct nodeParams *p){
	printf("%s", p->param1);
	if(p->param2 != NULL){
		printf(" ,%s", p->param2);
		free(p->param2);
	}
	free(p->param1);
}

int printDeclSeq(struct nodeDeclSeq *p){
	indents(counter);
    printDecl(p->decl);
	free(p->decl);
	if(p->has == 1){
		printDeclSeq(p->declSeq);
		free(p->declSeq);
	}
    return 1;
}

int printDecl(struct nodeDecl *p){

	if(p->rec == 0){
		printDeclInt(p->declInt);
		free(p->declInt);
	}
	else{
		printDeclRec(p->declRec);
		free(p->declRec);
	}
	return 1;
}

int printDeclInt(struct nodeDeclInt *p){

	printf("integer %s; \n", p->id);
	free(p->id);
	return 1;
}

int printDeclRec(struct nodeDeclRec *p){

	printf("record %s; \n", p->id);
	free(p->id);
	return 1;
}

int printStmtSeq(struct nodeStmtSeq *p){

	indents(counter);
    printStmt(p->stmt);
	free(p->stmt);
	if(p->stmtSeq != NULL){
		printStmtSeq(p->stmtSeq);
		free(p->stmtSeq);
	}
    return 1;
}

int printStmt(struct nodeStmt *p){

	if(p->assign != NULL){
		printAssign(p->assign);
		free(p->assign);
	}
	else if(p->ifStmt != NULL){
		printIf(p->ifStmt);
		free(p->ifStmt);
	}
	else if(p->loop != NULL){
		printLoop(p->loop);
		free(p->loop);
	}
	else if(p->out != NULL){
		printOut(p->out);
		free(p->out);
	}
	else if(p->call != NULL){
		printCall(p->call);
		free(p->call);
	}
	return 1;
}

int printAssign(struct nodeAssign *p){

	printf("%s", p->id);
	free(p->id);
	if(p->type == 1){
		printIndex(p->index);
		free(p->index);
		printf(":=");
		printExpr(p->expr);
		free(p->expr);
		printf("; \n");
	}
	else{
		printf(":= ");
		if(p->type == 2){
			printf("new record [");
			printExpr(p->expr);
			free(p->expr);
			printf("];\n");
		}
		else{
			printf("record %s ;\n", p->id2);
			free(p->id2);
		}

	}
	return 1;
}

int printIf(struct nodeIf *p){
	
	printf("if ");
	printCond(p->cond);
	free(p->cond);
	printf(" then \n");
	counter++;
	indents(counter);
	printStmtSeq(p->stmtSeq1);
	free(p->stmtSeq1);
	if(p->stmtSeq2 != NULL){
		counter--;
		indents(counter);
		printf("else \n");
		counter++;
		indents(counter);
		printStmtSeq(p->stmtSeq2);
		free(p->stmtSeq2);
	}
	counter--;
	indents(counter);
	printf("end \n");
	return 1;
}

int printLoop(struct nodeLoop *p){

	printf("while ");
	printCond(p->cond);
	free(p->cond);
	printf(" do \n");
	counter++;
	indents(counter);
	printStmtSeq(p->stmtSeq);
	free(p->stmtSeq);
	counter--;
	indents(counter);
	printf("end \n");
	return 1;

}

int printOut(struct nodeOut *p){
	
	printf("out(");
	printExpr(p->expr);
	free(p->expr);
	printf(");\n");
	return 1;
}

int printCall(struct nodeCall *p){
	printf("begin ");
	printf("%s ", p->fid);
	free(p->fid);
	printf("(");
	printParams(p->params);
	free(p->params);
	printf(");\n");

}

int printIndex(struct nodeIndex *p){
	
	if(p->empty == 1){
		printf("[");
		printExpr(p->expr);
		free(p->expr);
		printf("] ");
	}
	else{
		
		printf("");
	}
	return 1;
}

int printCond(struct nodeCond *p){

	if(p->op == NOT){
		printf("not ");
		printCond(p->cond);
		free(p->cond);
	}
	else{
		printCmpr(p->cmpr);
		free(p->cmpr);
		if(p->op == OR){
			printf("or ");
			printCond(p->cond);
			free(p->cond);
		}
		else if (p->op == AND){
			printf("and ");
			printCond(p->cond);
			free(p->cond);
		}
	}
	return 1;
}

int printCmpr(struct nodeCmpr *p){

	printExpr(p->expr1);
	free(p->expr1);
	if(p->op == EQUAL){
		printf("=");
		printExpr(p->expr2);
		free(p->expr2);
	}
	else{
		printf("<");
		printExpr(p->expr2);
		free(p->expr2);
	}
	return 1;
}

int printExpr(struct nodeExpr *p){

	printTerm(p->term);
	free(p->term);
	if(p->op == ADD){
		printf("+");
		printExpr(p->expr);
		free(p->expr);
	}
	else if(p->op == SUBTRACT){
		printf("-");
		printExpr(p->expr);
		free(p->expr);
	}
	return 1;
}

int printTerm(struct nodeTerm *p){

	printFactor(p->factor);
	free(p->factor);
	if(p->op == MULTIPLY){
		printf("*");
		printTerm(p->term);
		free(p->term);
	}
	else if(p->op == DIVIDE){
		printf("/");
		printTerm(p->term);
		free(p->term);
	}
	return 1;
}

int printFactor(struct nodeFactor *p){

	if(p->id != NULL){
		printf("%s", p->id);
		free(p->id);
		if(p->expr != NULL){
			printf("[");
			printExpr(p->expr);
			free(p->expr);
			printf("]");
		}
	}
	else if(p->constNum != 1100){
		printf("%d", p->constNum);
	}
	else if(p->expr != NULL){
		printf("(");
		printExpr(p->expr);
		free(p->expr);
		printf(")");
	}
	else if(p->in != 0){
		printf("in () ");
	}
	return 1;
	
}
