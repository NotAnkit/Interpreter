#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "core.h"

// Below are the values we want to persist as the scanner runs

// This maintains a pointer to the file
static FILE *fp;

// This is the string making up the current token
static char *tokenString;

// This is the enum value for the current token
static int token;

// Below are the functions that make up the scanner

// Opens the file, find the first token
int scanner_open(char *filename)
{
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error: File %s couldn't be opened!\n", filename);
    }
    tokenString = NULL;
    return nextToken();
}

// Frees memory allocated for the scanner
int scanner_close()
{
    fclose(fp);
    free(tokenString);
}

// Returns the current token (enum value)
int currentToken()
{
    return token;
}

// Finds the next token (or the first token when called by scanner_open)
int nextToken()
{
    // Clear token string for next string
    if(tokenString != NULL){
        free(tokenString);
        tokenString = NULL;
    }

    // Allocate space
    tokenString = (char *)calloc(20, sizeof(char));

    // if not at EOF then read
    if (!feof(fp))
    {
        char currentChar = fgetc(fp);
        // Check if the current char is a white space and to iterate until it isnt
        while(isspace(currentChar) && currentChar != EOF){
            currentChar = fgetc(fp);
        }

        // Make sure did not read EOF
        if(currentChar == EOF){
            token = EOS;
            return EOS;
        }
        
        // Switch Case for all symbols
        switch (currentChar)
        {
        case ';':
            append(tokenString, currentChar);
            token = SEMICOLON;
            return SEMICOLON;
        case ',':
            append(tokenString, currentChar);
            token = COMMA;
            return COMMA;
        case '+':
            append(tokenString, currentChar);
            token = ADD;
            return 1;
        case '-':
            append(tokenString, currentChar);
            token = SUBTRACT;
            return SUBTRACT;
        case '(':
            append(tokenString, currentChar);
            token = LPAREN;
            return LPAREN;
        case ')':
            append(tokenString, currentChar);
            token = RPAREN;
            return RPAREN;
        case '*':
            append(tokenString, currentChar);
            token = MULTIPLY;
            return MULTIPLY;
        case '/':
            append(tokenString, currentChar);
            token = DIVIDE;
            return DIVIDE;
        case '<':
            append(tokenString, currentChar);
            token = LESS;
            return LESS;
        case '.':
            append(tokenString, currentChar);
            token = PERIOD;
            return PERIOD;
         case '[':
            append(tokenString, currentChar);
            token = LBRACE;
            return LBRACE;
         case ']':
            append(tokenString, currentChar);
            token = RBRACE;
            return RBRACE;
        case '=':
            append(tokenString, currentChar);
            token = EQUAL;
            return EQUAL;
        case ':':
            append(tokenString, currentChar);

            // Check if the next char is equals or not
            char nextChar = fgetc(fp);
            if (nextChar == '=')
            {
                append(tokenString, currentChar);
                token = ASSIGN;
                return ASSIGN;
            }
            ungetc(nextChar, fp);
            token = COLON;
            return COLON;
        default:
            break;
        }

        // Check if the char is a character 
        if (isalpha(currentChar))
        {
            append(tokenString, currentChar);
            char nextChar = fgetc(fp);
            // Iterate until the next char is not a digit or char
            while (isalnum(nextChar) && nextChar != EOF)
            {
                append(tokenString, nextChar);
                nextChar = fgetc(fp);
            }

            // Make sure not at EOF
            if(currentChar == EOF){
                token = EOS;
                return EOS;
            }

            // Put back char that broke the loop
            ungetc(nextChar, fp);

            // compare the token string to the keywords
            if (strcmp(tokenString, "while") == 0)
            {
                token = WHILE;
                return WHILE;
            }
            else if (strcmp(tokenString, "and") == 0)
            {
                token = AND;
                return AND;
            }
            else if (strcmp(tokenString, "begin") == 0)
            {
                token = BEGIN;
                return BEGIN;
            }
            else if (strcmp(tokenString, "do") == 0)
            {
                token = DO;
                return DO;
            }
            else if (strcmp(tokenString, "else") == 0)
            {
                token = ELSE;
                return ELSE;
            }
            else if (strcmp(tokenString, "end") == 0)
            {
                token = END;
                return END;
            }
            else if (strcmp(tokenString, "if") == 0)
            {
                token = IF;
                return IF;
            }
            else if (strcmp(tokenString, "in") == 0)
            {
                token = IN;
                return IN;
            }
            else if (strcmp(tokenString, "integer") == 0)
            {
                token = INTEGER;
                return INTEGER;
            }
            else if (strcmp(tokenString, "is") == 0)
            {
                token = IS;
                return IS;
            }
            else if (strcmp(tokenString, "new") == 0)
            {
                token = NEW;
                return NEW;
            }
            else if (strcmp(tokenString, "not") == 0)
            {
                token = NOT;
                return NOT;
            }
            else if (strcmp(tokenString, "or") == 0)
            {
                token = OR;
                return OR;
            }
            else if (strcmp(tokenString, "out") == 0)
            {
                token = OUT;
                return OUT;
            }
            else if (strcmp(tokenString, "procedure") == 0)
            {
                token = PROCEDURE;
                return PROCEDURE;
            }
            else if (strcmp(tokenString, "record") == 0)
            {
                token = RECORD;
                return RECORD;
            }
            else if (strcmp(tokenString, "then") == 0)
            {
                token = THEN;
                return THEN;
            }
            else
            {
                token = ID;
                return ID;
            }
        }

        // Check if the char is a digit
        if (isdigit(currentChar))
        {
            append(tokenString, currentChar);
            char nextChar = fgetc(fp);

            // Iterate while the next char is a digit
            while (isdigit(nextChar) && nextChar != EOF)
            {
                append(tokenString, nextChar);
                nextChar = fgetc(fp);
            }

            // Put back the char
            ungetc(nextChar, fp);

            // First make sure not too big cant fit in a int Make sure the string const is not larger than 1009
            if (strlen(tokenString) > 4 || atoi(tokenString) > 1009)
            {
                token = ERROR;
                printf("ERROR: CONST too large! check code. Max CONST 1009");
                return ERROR;
            }
            token = CONST;
            return CONST;
        }

        // If not caught by switch or if statements, must be invalid so throw error.
        token = ERROR;
        printf("ERROR: Unknown TOKEN %c, check code", currentChar);
        return ERROR;
    }
    // If file was empty return EOF
    token = EOS;
    return EOS;
}

// Passes back the string value, so the parse tree (project 2) will be able to store identifiers
void getId(char *identifier)
{
    strcpy(identifier, tokenString);
}

// Passes back the constant value, so the parse tree (project 2) will be able to store const values
int getConst()
{
    return atoi(tokenString);
}

// Adds the next char to the tokenString 
void append(char *s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}