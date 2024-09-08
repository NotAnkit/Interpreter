#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "core.h"
#include "nodes.h"
#include "parser.h"
#include "memory.h"

/*
*
* Helper functions
*
*/

// Converts token value to a string
static void tokenString(char* str, int current) {
	switch (current) {
	  case AND : strcpy(str, "AND"); break;
	  case BEGIN : strcpy(str, "BEGIN"); break;
	  case DO : strcpy(str, "DO"); break;
	  case ELSE : strcpy(str, "ELSE"); break;
	  case END : strcpy(str, "END"); break;
	  case IF : strcpy(str, "IF"); break;
	  case IN : strcpy(str, "IN"); break;
	  case INTEGER : strcpy(str, "INTEGER"); break;
	  case IS : strcpy(str, "IS"); break;
	  case NEW : strcpy(str, "NEW"); break;
	  case NOT : strcpy(str, "NOT"); break;
	  case OR : strcpy(str, "OR"); break;
	  case OUT : strcpy(str, "OUT"); break;
	  case PROCEDURE : strcpy(str, "PROCEDURE"); break;
	  case RECORD : strcpy(str, "RECORD"); break;
	  case THEN : strcpy(str, "THEN"); break;
	  case WHILE : strcpy(str, "WHILE"); break;
	  case ADD : strcpy(str, "ADD"); break;
	  case SUBTRACT : strcpy(str, "SUBTRACT"); break;
	  case MULTIPLY : strcpy(str, "MULTIPLY"); break;
	  case DIVIDE : strcpy(str, "DIVIDE"); break;
	  case ASSIGN : strcpy(str, "ASSIGN"); break;
	  case EQUAL : strcpy(str, "EQUAL"); break;
	  case LESS : strcpy(str, "LESS"); break;
	  case COLON : strcpy(str, "COLON"); break;
	  case SEMICOLON : strcpy(str, "SEMICOLON"); break;
	  case PERIOD : strcpy(str, "PERIOD"); break;
	  case COMMA : strcpy(str, "COMMA"); break;
	  case LPAREN : strcpy(str, "LPAREN"); break;
	  case RPAREN : strcpy(str, "RPAREN"); break;
	  case LBRACE : strcpy(str, "LBRACE"); break;
	  case RBRACE : strcpy(str, "RBRACE"); break;
	  case CONST : strcpy(str, "CONST"); break;
	  case ID : strcpy(str, "ID"); break;
	  case EOS : strcpy(str, "EOS"); break;
	}
}

// If the current token is something other than expected,
// prints a meaningful error message and halts the program
static void expectedToken(int expected) {
	int actual = currentToken();
	
    if (actual != expected) {
		char actualStr[20];
		char expectedStr[20];
		
		tokenString(actualStr, actual);
		tokenString(expectedStr, expected);
		
        printf("Error: expected %s but recieved %s", expectedStr, actualStr);
        exit(0);
    }
}

int parseProcedure(struct nodeProcedure *p){
	/*Intialize the arrays to hold the ids
	int i;
	for(i=0;i<=20;i++)
	{
    	ids[0][i] = (char*) calloc(1, 20);
		ids[1][i] = (char*) calloc(1, 20);
		usedVars[0][i] = (char*) calloc(1, 20);
		usedVars[1][i] = (char*) calloc(1, 20);
	}
	numids = 0;
	numVars = 0;
	*/

	expectedToken(PROCEDURE);
    nextToken();
	
	//Save the id for the procedure
	p->pid = (char *)calloc(20, sizeof(char));
    getId(p->pid);
    nextToken();

	//Check for function declarations
	if(currentToken() == AND){
		nextToken();
		p->funcSeq = calloc(1, sizeof(struct nodeFuncSeq));
		parseFuncSeq(p->funcSeq);
	}
	else{
		expectedToken(IS);
    	nextToken();
		p->funcSeq = NULL;
	}

	//Parse Decl Seq
	p->declSeq = calloc(1, sizeof(struct nodeDeclSeq));
    parseDeclSeq(p->declSeq);
	//Parse Stmt Seq
	p->stmtSeq = calloc(1, sizeof(struct nodeStmtSeq));
	parseStmtSeq(p->stmtSeq);

	expectedToken(END);
	nextToken();

	expectedToken(EOS);
	nextToken();
	
    return 1;
}

int parseFuncSeq(struct nodeFuncSeq *p){
	
	//Parse the first func
	p->func = calloc(1, sizeof(struct nodeFunc)); 
    parseFunc(p->func);
	
	addFunc(p->func);

	//Check if end of the func seq
	if(currentToken() == IS){
		p->has = 0;
		nextToken();
	}
	else{
		p->has = 1;
		p->funcSeq = calloc(1, sizeof(struct nodeFuncSeq));
		parseFuncSeq(p->funcSeq);
	}
    return 1;
}

int parseFunc(struct nodeFunc *p){
	expectedToken(PROCEDURE);
    nextToken();
	
	//Save the id for the procedure
	p->pid = (char *)calloc(20, sizeof(char));
    getId(p->pid);
    nextToken();

	expectedToken(LPAREN);
	nextToken();

	p->params = calloc(1, sizeof(struct nodeParams));
	parseParams(p->params);

	expectedToken(RPAREN);
	nextToken();
	
	expectedToken(IS);
	nextToken();

	//Parse Decl Seq
	p->declSeq = calloc(1, sizeof(struct nodeDeclSeq));
    parseDeclSeq(p->declSeq);
	//Parse Stmt Seq
	p->stmtSeq = calloc(1, sizeof(struct nodeStmtSeq));
	parseStmtSeq(p->stmtSeq);

	expectedToken(END);
	nextToken();
	
    return 1;
}

int parseParams(struct nodeParams *p){
	//Save the first param for function
	p->param1 = (char *)calloc(20, sizeof(char));
    getId(p->param1);
	p->param2 = NULL;
    nextToken();

	if(currentToken() == COMMA){
		nextToken();
		//Save the first param for function
		p->param2 = (char *)calloc(20, sizeof(char));
    	getId(p->param2);
    	nextToken();
	}

}

int parseDeclSeq(struct nodeDeclSeq *p){
	
	//Parse the first decl
	p->decl = calloc(1, sizeof(struct nodeDecl)); 
    parseDecl(p->decl);
	
	//Check if end of the decl seq
	if(currentToken() == BEGIN){
		p->has = 0;
		nextToken();
	}
	else{
		p->has = 1;
		p->declSeq = calloc(1, sizeof(struct nodeDeclSeq));
		parseDeclSeq(p->declSeq);
	}
    return 1;
}

int parseDecl(struct nodeDecl *p){
	
	//Check which decl it is
	if(currentToken() == INTEGER){
		nextToken();
		p->rec = 0;
		p->declInt = calloc(1, sizeof(struct nodeDeclInt));
		parseDeclInt(p->declInt);
	}
	else{
		nextToken();
		p->rec = 1;
		p->declRec = calloc(1, sizeof(struct nodeDeclRec));
		parseDeclRec(p->declRec);
	}
	return 1;
}

int parseDeclInt(struct nodeDeclInt *p){
	
	//Get the id for the int and save it in array
	p->id = (char *)calloc(20, sizeof(char));
	getId(p->id);
	/*strcpy(ids[0][numids], p->id);
	strcpy(ids[1][numids], "int");
	numids++;*/
	nextToken();

	expectedToken(SEMICOLON);
	nextToken();
	return 1;
}

int parseDeclRec(struct nodeDeclRec *p){
	
	//Get the id for the rec and save it in array
	p->id = (char *)calloc(20, sizeof(char));
	getId(p->id);
	/*strcpy(ids[0][numids], p->id);
	strcpy(ids[1][numids], "rec");
	numids++;*/
	nextToken();

	expectedToken(SEMICOLON);
	nextToken();
	return 1;
}

int parseStmtSeq(struct nodeStmtSeq *p){
	
	//Parse the first stmt
	p->has = 0;
	p->stmt = calloc(1, sizeof(struct nodeStmt)); 
    parseStmt(p->stmt);

	//Check if at end of the seq
	if(!(currentToken() == END || currentToken() == ELSE)){
		p->has = 1;
		p->stmtSeq = calloc(1, sizeof(struct nodeStmtSeq));
		parseStmtSeq(p->stmtSeq);
	}
    return 1;
}

int parseStmt(struct nodeStmt *p){
	
	//Check which stmt it is
	if(currentToken() == ID){
		p->assign = calloc(1, sizeof(struct nodeAssign));
		parseAssign(p->assign);
		p->ifStmt = NULL;
		p->loop = NULL;
		p->out = NULL;
		p->call = NULL;
	}
	else if(currentToken() == IF){
		nextToken();
		p->ifStmt = calloc(1, sizeof(struct nodeIf));
		parseIf(p->ifStmt);
		p->assign = NULL;
		p->loop = NULL;
		p->out = NULL;
		p->call = NULL;
	}
	else if(currentToken() == WHILE){
		nextToken();
		p->loop = calloc(1, sizeof(struct nodeLoop));
		parseLoop(p->loop);
		p->assign = NULL;
		p->ifStmt = NULL;
		p->out = NULL;
		p->call = NULL;
	}
	else if(currentToken() == OUT){
		nextToken();
		p->out = calloc(1, sizeof(struct nodeOut));
		parseOut(p->out);
		p->assign = NULL;
		p->loop = NULL;
		p->ifStmt = NULL;
		p->call = NULL;
	}
	else if(currentToken() == BEGIN){
		nextToken();
		p->call = calloc(1, sizeof(struct nodeCall));
		parseCall(p->call);
		p->assign = NULL;
		p->loop = NULL;
		p->ifStmt = NULL;
		p->out = NULL;
	}
	//Semantic check to check for a body
	else{
		printf("ERROR: Procedure body is empty \n");
		exit(0);
	}
	return 1;
}

int parseCall(struct nodeCall *p){
	//Get the id for the call
	p->fid = (char *)calloc(20, sizeof(char));
	getId(p->fid);
	nextToken();

	expectedToken(LPAREN);
	nextToken();

	p->params = calloc(1, sizeof(struct nodeParams));
	parseParams(p->params);

	expectedToken(RPAREN);
	nextToken();

	expectedToken(SEMICOLON);
	nextToken();
}

int parseAssign(struct nodeAssign *p){
	
	//Save the intial id
	p->id = (char *)calloc(20, sizeof(char));
	getId(p->id);

	/*If the id is not in the array of the usedvars add it
	int i;
	for(i = 0; i <= numVars; i++){
		if(strcmp(p->id, usedVars[0][i]) == 0){
			break;
		}
		else{
			strcpy(usedVars[0][numVars] , p->id);
			strcpy(usedVars[1][numVars] , "rec");
			numVars++;
			break;
		}
	}*/
	nextToken();

	//Check which case of the assign it is
	if(currentToken() == ASSIGN){
		nextToken();
		//Check which of the subcases	
		if(currentToken() == NEW){
			nextToken();
			p->type = 2;

			expectedToken(RECORD);
			nextToken();

			expectedToken(LBRACE);
			nextToken();

			p->expr = calloc(1, sizeof(struct nodeExpr));
			parseExpr(p->expr);
			p->index = NULL;
			p->id2 = NULL;
			
			expectedToken(RBRACE);
			nextToken();
			
			expectedToken(SEMICOLON);
			nextToken();
		}
		else if (currentToken() == RECORD){
			nextToken();
			p->type = 3;

			//Get the id and then save in the array if not there
			p->id2 = (char *)calloc(20, sizeof(char));
			getId(p->id2);

			/*int i;
			for(i = 0; i <= numVars; i++){
				if(strcmp(p->id, usedVars[0][i]) == 0){
					break;
				}
				else{
					strcpy(usedVars[0][numVars] , p->id);
					strcpy(usedVars[1][numVars] , "rec");
					numVars++;
					break;
				}
			}*/
			nextToken();

			expectedToken(SEMICOLON);
			nextToken();
		}
		else{
			/*Case with the index being empty change the id to a int
			strcpy(usedVars[1][--numVars] , "int");
			numVars++;
			*/
			p->type = 1;
			p->index = calloc(1, sizeof(struct nodeIndex));
			parseIndex(p->index);

			p->expr = calloc(1, sizeof(struct nodeExpr));
			parseExpr(p->expr);
			p->id2 = NULL;

			expectedToken(SEMICOLON);
			nextToken();
		}
	}
	else{
		//Go through a get the index for the first case
		p->type = 1;

		p->index = calloc(1, sizeof(struct nodeIndex));
		parseIndex(p->index);
		
		expectedToken(ASSIGN);
		nextToken();

		p->expr = calloc(1, sizeof(struct nodeExpr));
		parseExpr(p->expr);
		p->id2 = NULL;

		expectedToken(SEMICOLON);
		nextToken();
	}
}

int parseIf(struct nodeIf *p){

	//Get the cond and parse
	p->cond = calloc(1, sizeof(struct nodeCond));
	parseCond(p->cond);
	
	expectedToken(THEN);
	nextToken();

	//Parse the stmt
	p->stmtSeq1 = calloc(1, sizeof(struct nodeStmtSeq));
	parseStmtSeq(p->stmtSeq1);
	p->stmtSeq2 = NULL;

	//Check if "else" or not
	if(currentToken() == ELSE){
		nextToken();

		p->stmtSeq2 = calloc(1, sizeof(struct nodeStmtSeq));
		parseStmtSeq(p->stmtSeq2);
	}

	expectedToken(END);
	nextToken();

	return 1;
}

int parseLoop(struct nodeLoop *p){

	//parse the loop
	p->cond = calloc(1, sizeof(struct nodeCond));
	parseCond(p->cond);
	
	expectedToken(DO);
	nextToken();

	//parse the stmtseq
	p->stmtSeq = calloc(1, sizeof(struct nodeStmtSeq));
	parseStmtSeq(p->stmtSeq);

	expectedToken(END);
	nextToken();
	
	return 1;

}

int parseOut(struct nodeOut *p){

	expectedToken(LPAREN);
	nextToken();
	
	//Parse the expr
	p->expr = calloc(1, sizeof(struct nodeExpr));
	parseExpr(p->expr);
	
	expectedToken(RPAREN);
	nextToken();

	expectedToken(SEMICOLON);
	nextToken();

	return 1;
}

int parseIndex(struct nodeIndex *p){

	if(currentToken() == LBRACE){
		nextToken();
		p->empty = 1;

		//Parse the Expr
		p->expr = calloc(1, sizeof(struct nodeExpr));
		parseExpr(p->expr);
		
		expectedToken(RBRACE);
		nextToken();
	}
	else{
		//Could be epsilion so empty
		p->empty = 0;
	}

	return 1;
}

int parseCond(struct nodeCond *p){

	//Check which case
	if(currentToken() == NOT){
		p->op = currentToken();
		nextToken();

		p->cond = calloc(1, sizeof(struct nodeCond));
		parseCond(p->cond);
		p->cmpr = NULL;
		
	}
	else{

		p->cmpr = calloc(1, sizeof(struct nodeCmpr));
		parseCmpr(p->cmpr);
		p->op = 1;
		
		if(currentToken() == OR || currentToken() == AND){
			p->op = currentToken();
			nextToken();

			p->cond = calloc(1, sizeof(struct nodeCond));
			parseCond(p->cond);
		}
	}
	return 1;
}

int parseCmpr(struct nodeCmpr *p){
	//Parse the expr
	p->expr1 = calloc(1, sizeof(struct nodeExpr));
	parseExpr(p->expr1);
	
	//Parse the next
	if(currentToken() == EQUAL || currentToken() == LESS){
		p->op = currentToken();
		nextToken();

		p->expr2 = calloc(1, sizeof(struct nodeExpr));
		parseExpr(p->expr2);
	}

	return 1;
}

int parseExpr(struct nodeExpr *p){
	//Parse the term
	p->term = calloc(1, sizeof(struct nodeTerm));
	parseTerm(p->term);

	//Parse the second expr
	if(currentToken() == ADD || currentToken() == SUBTRACT){
		p->op = currentToken();
		nextToken();

		p->expr = calloc(1, sizeof(struct nodeExpr));
		parseExpr(p->expr);
	}

	return 1;
}

int parseTerm(struct nodeTerm *p){

	//Parse the factor
	p->factor = calloc(1, sizeof(struct nodeFactor));
	parseFactor(p->factor);

	//Parse the second term if needed
	if(currentToken() == MULTIPLY || currentToken() == DIVIDE){
		p->op = currentToken();
		nextToken();

		p->term = calloc(1, sizeof(struct nodeTerm));
		parseTerm(p->term);
	}
	return 1;
}

int parseFactor(struct nodeFactor *p){

	p->constNum = 1100;
	p->expr = NULL;
	p->id = NULL;
	p->in = 0;

	//Check which case
	if(currentToken() == ID){
		p->id = (char *)calloc(20, sizeof(char));
		getId(p->id);
		/*Take the id and add to array if not there
		int i;
		for(i = 0; i <= numVars; i++){
			if(strcmp(p->id, usedVars[0][i]) == 0){
				break;
			}
			else{
				strcpy(usedVars[0][numVars] , p->id);
				strcpy(usedVars[1][numVars] , "int");
				numVars++;
				break;
			}
		}*/

		nextToken();

		if(currentToken() == LBRACE){
			/*Means it has to be the record so change it
			strcpy(usedVars[1][--numVars] , "rec");
			numVars++;
			*/
			nextToken();

			p->expr = calloc(1, sizeof(struct nodeExpr));
			parseExpr(p->expr);
			
			expectedToken(RBRACE);
			nextToken();
		}
	}
	else if(currentToken() == CONST){
		p->constNum = getConst();
		nextToken();
	}
	else if(currentToken() == LPAREN){
		nextToken();

		p->expr = calloc(1, sizeof(struct nodeExpr));
		parseExpr(p->expr);
		
		expectedToken(RPAREN);
		nextToken();
		
	}
	else if(currentToken() == IN){
		p->in = 1;
		nextToken();

		expectedToken(LPAREN);
		nextToken();

		expectedToken(RPAREN);
		nextToken();
	}
	else{
		//Final if not in anything then must be a error
		char actualStr[20];
		tokenString(actualStr, currentToken());
		printf("Missing Token before %s \n", actualStr);
		exit(0);
	}

	return 1;
}