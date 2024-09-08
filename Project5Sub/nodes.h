#ifndef NODES_H
#define NODES_H

/*These two arrays store all the ids and their type
char *ids[2][20];
int numids;

char *usedVars[2][20];
int numVars;
*/

/*
*
* All the different structs and their structure
*
*/

 struct nodeProcedure{
    char *pid;
    struct nodeDeclSeq *declSeq;
    struct nodeFuncSeq *funcSeq;
    struct nodeStmtSeq *stmtSeq;

};

struct nodeFuncSeq{
    struct nodeFunc *func;
    struct nodeFuncSeq *funcSeq;
    int has;

};

struct nodeFunc{
    char *pid;
    struct nodeDeclSeq *declSeq;
    struct nodeParams *params;
    struct nodeStmtSeq *stmtSeq;

};

struct nodeParams{
    char *param1;
    char *param2;
};


 struct nodeDeclSeq{
    struct nodeDecl *decl;
    struct nodeDeclSeq *declSeq;
    int has;

};

 struct nodeStmtSeq{
    struct nodeStmt *stmt;
    struct nodeStmtSeq *stmtSeq;
    int has;

};

 struct nodeDecl{
    struct nodeDeclInt *declInt;
    struct nodeDeclRec *declRec;
    int rec;

};

 struct nodeDeclInt{
    char *id;

};

 struct nodeDeclRec{
    char *id;

};

 struct nodeStmt{
    struct nodeAssign *assign;
    struct nodeIf *ifStmt;
    struct nodeLoop *loop;
    struct nodeOut *out;
    struct nodeCall *call;
    
};

struct nodeCall{
    char *fid;
    struct nodeParams *params;
};

 struct nodeAssign{
    char *id;
    char *id2;
    struct nodeIndex *index;
    struct nodeExpr *expr;
    int type;

};

 struct nodeIndex{
    struct nodeExpr *expr;
    int empty;

};

 struct nodeOut{
    struct nodeExpr *expr;

};

 struct nodeIf{
    struct nodeCond *cond;
    struct nodeStmtSeq *stmtSeq1;
    struct nodeStmtSeq *stmtSeq2;

};

 struct nodeLoop{
    struct nodeCond *cond;
    struct nodeStmtSeq *stmtSeq;
    
};

 struct nodeCond{
    struct nodeCond *cond;
    struct nodeCmpr *cmpr;
    int op;

};

 struct nodeCmpr{
    struct nodeExpr *expr1;
    struct nodeExpr *expr2;
    int op;

 };

 struct nodeExpr{
    struct nodeTerm *term;
    struct nodeExpr *expr;
    int op;

};

 struct nodeTerm{
    struct nodeTerm *term;
    struct nodeFactor *factor;
    int op;

};

 struct nodeFactor{
    char *id;
    struct nodeExpr *expr;
    int constNum;
    int in;
};

#endif
