#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "core.h"
#include "memory.h"
#include "nodes.h"

/*
*
* Data Structs
*
*/

/*
*	Stack and Frame
*/

struct Frame{
    char** iLookup;
	int* iValues;
	int iLen;

	char** rLookup;
	int rLen;

	int** rValues;
};

//Stack of all frames
struct Frame* stack[20];
static int fp;

//These frames are for management such as the current and previous
struct Frame* currentFrame;
struct Frame* callingFrame;

//Maps all the funcation names to the nodes
char* functionMap[20];
struct nodeFunc* functionNodes[20];
static int funcCount;

//Garbage collector counter
static int rCount;

/*
*
* Helper functions
*
*/

// If iden is an integer, return the index. Otherwise, return -1
static int searchInteger(char* iden) {
	int location = -1;
	int i;
	for (i=0; i<(currentFrame->iLen); i++) {
		if (strcmp((currentFrame->iLookup)[i], iden)==0) {
			location = i;
		}
	}
	return location;
}

// If iden is a record, return the index. Otherwise, return -1
static int searchRecord(char* iden) {
	int location = -1;
	int i;
	for (i=0; i<(currentFrame->rLen); i++) {
		if (strcmp((currentFrame->rLookup)[i], iden)==0) {
			location = i;
		}
	}
	return location;
}

// If iden is an id, return the index. Otherwise, return -1
static int searchFunctionMap(char* iden) {
	int location = -1;
	int i;
	for (i=0; i<funcCount; i++) {
		if (strcmp(functionMap[i], iden)==0) {
			location = i;
		}
	}
	return location;
}

/*
*
* Stack Functions
*
*
*/

void stack_init() {
	//Intialize all frames and arrays for functions and stack.
	struct Frame* currentFrame = calloc(1, sizeof(struct Frame));
	struct Frame* callingFrame = calloc(1, sizeof(struct Frame));
	int i;
	for(i = 0; i < 20; i++){
		functionMap[i] = calloc(20, sizeof(char));
		functionNodes[i] = (struct nodeFunc*)calloc(1, sizeof(struct nodeFunc));
		stack[i] = (struct Frame*)calloc(1, sizeof(struct Frame));
	}
	
	fp = -1;
	funcCount = 0;
	rCount = 0;
}

void setParams(struct nodeParams *p, struct nodeParams *c){
	//Check if the second parameter has been set for both
	if(p->param2 != NULL && c->param2 == NULL){
		printf("ERROR: Argument paramter count different than formal parameters \n");
		exit(0);
	}
	//Using helper frame to save the callee frame
	struct Frame* helper = calloc(1, sizeof(struct Frame));

	//Search the record array and save the indexs of the parameters
	int indexParam1 = searchRecord(p->param1);
	int indexParam2 = -1;
	if(p->param2 != NULL){
		if(strcmp(p->param1, p->param2) == 0){
			printf("ERROR: Function parameters not distinct! \n");
			exit(0);
		}
		indexParam2 = searchRecord(p->param2);
	}

	//Change currentFrame to caller to get indexs on parameters
	callingFrame = stack[fp-1];
	helper = currentFrame;
	currentFrame = callingFrame;
	int indexCall1 = searchRecord(c->param1);
	int indexCall2;
	if(p->param2 != NULL){
		indexCall2 = searchRecord(c->param2);
	}

	//return frame to callee
	currentFrame = helper;
	helper = NULL;
	free(helper);

	//Chnage both the values and the lengths
	currentFrame->rValues[indexParam1] = callingFrame->rValues[indexCall1];
	//Increase ref count
	currentFrame->rValues[indexParam1][21]++;
	

	if(indexParam2 != -1){
		currentFrame->rValues[indexParam2] = callingFrame->rValues[indexCall2];
		currentFrame->rValues[indexParam2][21]++;
	}
}

//When calling search the map and get function
struct nodeFunc* getFunc(char* pid){
	int loc = searchFunctionMap(pid);
	if(loc == -1){
		printf("ERROR: Function %s not declared! \n", pid);
		exit(0);
	}
	return functionNodes[loc];
}

//Return control back to caller with its frame
void popStack(){
	//If the frame is the main frame, go through and decrement rCount until no objects
	if(fp == 0){
		while(rCount >= 1){
			rCount--;
			printf("gc:%d\n", rCount);
		}
		rCount = 0;
	}
	else{
		//If any other frame decrement the number of refs, for locals should make zero for params should leave atleast 1
		for (int i=0; i<(currentFrame->rLen); i++) {
			currentFrame->rValues[i][21]--;
			if(currentFrame->rValues[i][21] == 0 && currentFrame->rValues[i][20] != -1){
				rCount--;
				printf("gc:%d\n", rCount);
			}
		}
		fp--;
		currentFrame = stack[fp];
	}
}

//Add function to function map to call
void addFunc(struct nodeFunc *p){
	//Make sure the name is unique
	for(int i = 0; i<funcCount; i++){
		if(strcmp(functionMap[i], p->pid) == 0){
			printf("ERROR: multiple functions with Function %s declared! \n", p->pid);
			exit(0);
		}
	}
	strcpy((functionMap[funcCount]), p->pid);
	functionNodes[funcCount] = p;
	funcCount++;
}

/*
*
* Memory functions
*
*/

// Initialize data structures
// Don't worry about freeing memory, will do that in project 5
void memory_init() {
	struct Frame* frame = calloc(1, sizeof(struct Frame));

	frame->iLookup = calloc(20, sizeof(char*));
	frame->rLookup = calloc(20, sizeof(char*));

	frame->iValues = calloc(20, sizeof(int));
	frame->rValues = calloc(20, sizeof(int*));

	int i;
	for(i = 0; i < 20; i++){
		frame->iLookup[i] = calloc(20, sizeof(char));
		frame->rLookup[i] = calloc(20, sizeof(char));
		//Setting the lengths to -1 as a way to know which ones are unitialized
	}
	for(i = 0; i < 22; i++){
		frame->rValues[i] = calloc(22, sizeof(int));
		//Setting the lengths to -1 as a way to know which ones are unitialized
		frame->rValues[i][20] = -1;
	}
	frame->iLen = 0;
	frame->rLen = 0;

	fp++;
	stack[fp] = frame;
	currentFrame = frame;
}

// Handle an integer or record declaration
void declare(char* iden, int type) {
	if(type == 1){
		strcpy(currentFrame->iLookup[(currentFrame->iLen)], iden);
		currentFrame->iLen++;
	}
	else{
		strcpy(currentFrame->rLookup[(currentFrame->rLen)], iden);
		currentFrame->rLen++;
	}
}

// Store a value to a variable. Remember, unindexed stores to a record go to index 0
void store(char* iden, int value) {
	int index = searchInteger(iden);
	if(index != -1){
		currentFrame->iValues[index] = value;
	}
	else{
		//Since the semantic check would have validated that all vars exist if its not in int has to be in rec
		storeRec(iden, 0, value);
	}
}

// Read a value from a variable. Remember, unindexed reads from a record read index 0
int recall(char* iden) {
	int index = searchInteger(iden);
	if(index != -1){
		return currentFrame->iValues[index];
	}
	else{
		//Since the semantic check would have validated that all vars exist if its not in int has to be in rec
		return recallRec(iden, 0);
	}
}

// Store a value to a record variable, at the given index
void storeRec(char* iden, int index, int value) {
	int ind = searchRecord(iden);
	//Using the intialization to check if its null
	if(currentFrame->rValues[ind][20] == -1){
		printf("ERROR: Assigment of NULL record variable");
		exit(0);
	}
	int* array = currentFrame->rValues[ind];
	//Make sure not out of bounds
	if(index >= currentFrame->rValues[ind][20]){
		printf("ERROR: Index out of bounds\n");
		exit(0);
	}
	array[index] = value;
}

// Read a value from a record variable, from the given index
int recallRec(char* iden, int index) {
	int ind = searchRecord(iden);
	int* array = currentFrame->rValues[ind];
	//make sure not out of bounds
	if(index >= currentFrame->rValues[ind][20]){
		printf("ERROR: Index out of bounds\n");
		exit(0);
	}
	return array[index];
}

// Handle "id := record id" type assignment
void record(char* lhs, char* rhs) {
	int index1 = searchRecord(lhs);
	int index2 = searchRecord(rhs);

	//Decrease the ref the one being replaced, if zero decrease number of objects
	currentFrame->rValues[index1][21]--;
	if(currentFrame->rValues[index1][21] == 0){
		rCount--;
		printf("gc:%d\n", rCount);
	}
	//Increase ref count to the rhs
	currentFrame->rValues[index2][21]++;

	//Chnage both the values and the lengths
	currentFrame->rValues[index1] = currentFrame->rValues[index2];
}

// Handle "id := new record[<expr>]" type assignment
void allocateRecord(char* iden, int size) {
	int index = searchRecord(iden);
	int* array = currentFrame->rValues[index];
	array[20] = size;

	//Set the refCount for the object to 1 and increase the number of objects
	array[21] = 1;
	rCount++;
	printf("gc:%d\n", rCount);
	
	//Intialize the new array to zero
	int i;
	for(i = 0; i < size; i++){
		array[i] = 0;
	}	

}
