#ifndef MEMORY_H
#define MEMORY_H

#include "nodes.h"

void memory_init();

void memory_free();

void declare(char* iden, int type);

void store(char* iden, int value);

int recall(char* iden);

void storeRec(char* iden, int index, int value);

int recallRec(char* iden, int index);

void record(char* lhs, char* rhs);

void allocateRecord(char* iden, int size);

void stack_init();

void setParams(struct nodeParams *p, struct nodeParams *c);

struct nodeFunc* getFunc(char* pid);

void popStack();

void addFunc(struct nodeFunc *p);

void refGC(int refCount, int index);

#endif