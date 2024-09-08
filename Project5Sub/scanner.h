#ifndef SCANNER_H
#define SCANNER_H

#include "core.h"

// Opens the file, find the first token
int scanner_open(char* filename);

// Frees memory allocated for the scanner
int scanner_close();

// Returns the current token (enum value)
int currentToken();

// Finds the next token (or the first token when called by scanner_open)
int nextToken();

// Passes back the string value, so the parse tree (project 2) will be able to store identifiers
void getId(char* identifier);

// Passes back the constant value, so the parse tree (project 2) will be able to store const values
int getConst();

// Adds the next char value to the char array
void append(char* s, char c);

#endif