#ifndef CORE_H
#define CORE_H

// This enum serves as our tokens
enum core {
    // Keywords
    AND,
    BEGIN,
    DO,
    ELSE,
    END,
    IF,
    IN,
	INTEGER,
    IS,
    NEW,
    NOT,
    OR,
    OUT,
    PROCEDURE,
    RECORD,
    THEN,
    WHILE,
    // Symbols
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    ASSIGN,
    EQUAL,
    LESS,
    COLON,
    SEMICOLON,
    PERIOD,
	COMMA,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    // Others
    CONST,
    ID,
    EOS,
    ERROR
};

#endif