#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOLS 100
#define MAX_TOKEN_LEN 100

//TOKEN TYPES

typedef enum {
    TOKEN_INT,
    TOKEN_PRINT,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_ASSIGN,
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF,
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_TOKEN_LEN];
} Token;

FILE *source;
Token currentToken;

//SYMBOL TABLE

typedef struct {
    char name[50];
    int value;
} Symbol;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;

void storeVariable(char *name, int value) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            symbolTable[i].value = value;
            return;
        }
    }
    strcpy(symbolTable[symbolCount].name, name);
    symbolTable[symbolCount].value = value;
    symbolCount++;
}

int getVariable(char *name) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0)
            return symbolTable[i].value;
    }

    printf("Semantic Error: Variable '%s' not defined\n", name);
    exit(1);
}

//ERROR FUNCTION 

void error() {
    printf("Syntax Error\n");
    exit(1);
}

//LEXER

void getNextToken() {
    char ch;

    while ((ch = fgetc(source)) != EOF && isspace(ch));

    if (ch == EOF) {
        currentToken.type = TOKEN_EOF;
        return;
    }

    // Identifier or keyword
    if (isalpha(ch)) {
        int i = 0;
        currentToken.lexeme[i++] = ch;

        while (isalnum(ch = fgetc(source)))
            currentToken.lexeme[i++] = ch;

        currentToken.lexeme[i] = '\0';
        ungetc(ch, source);

        if (strcmp(currentToken.lexeme, "int") == 0)
            currentToken.type = TOKEN_INT;
        else if (strcmp(currentToken.lexeme, "print") == 0)
            currentToken.type = TOKEN_PRINT;
        else
            currentToken.type = TOKEN_IDENTIFIER;

        return;
    }

    // Number
    if (isdigit(ch)) {
        int i = 0;
        currentToken.lexeme[i++] = ch;

        while (isdigit(ch = fgetc(source)))
            currentToken.lexeme[i++] = ch;

        currentToken.lexeme[i] = '\0';
        ungetc(ch, source);

        currentToken.type = TOKEN_NUMBER;
        return;
    }

    switch (ch) {
        case '+': currentToken.type = TOKEN_PLUS; break;
        case '=': currentToken.type = TOKEN_ASSIGN; break;
        case ';': currentToken.type = TOKEN_SEMICOLON; break;
        case '(': currentToken.type = TOKEN_LPAREN; break;
        case ')': currentToken.type = TOKEN_RPAREN; break;
        default: currentToken.type = TOKEN_INVALID; break;
    }
}

//PARSER

void match(TokenType expected) {
    if (currentToken.type == expected)
        getNextToken();
    else
        error();
}

// Forward declarations
void Program();
void StatementList();
void Statement();
void Declaration();
void PrintStatement();
int Expression();
int Term();

// Program → StatementList
void Program() {
    StatementList();
}

// StatementList → Statement*
void StatementList() {
    while (currentToken.type == TOKEN_INT ||
           currentToken.type == TOKEN_PRINT) {
        Statement();
    }
}

// Statement → Declaration | PrintStatement
void Statement() {
    if (currentToken.type == TOKEN_INT)
        Declaration();
    else if (currentToken.type == TOKEN_PRINT)
        PrintStatement();
    else
        error();
}

// Declaration → int identifier = Expression ;
void Declaration() {

    match(TOKEN_INT);

    char varName[50];

    if (currentToken.type == TOKEN_IDENTIFIER) {
        strcpy(varName, currentToken.lexeme);
        match(TOKEN_IDENTIFIER);
    } else {
        error();
    }

    match(TOKEN_ASSIGN);

    int value = Expression();

    match(TOKEN_SEMICOLON);

    storeVariable(varName, value);
}

// PrintStatement → print ( identifier ) ;
void PrintStatement() {

    match(TOKEN_PRINT);
    match(TOKEN_LPAREN);

    char varName[50];

    if (currentToken.type == TOKEN_IDENTIFIER) {
        strcpy(varName, currentToken.lexeme);
        match(TOKEN_IDENTIFIER);
    } else {
        error();
    }

    match(TOKEN_RPAREN);
    match(TOKEN_SEMICOLON);

    printf("%d\n", getVariable(varName));
}

// Expression → Term { + Term }
int Expression() {

    int value = Term();

    while (currentToken.type == TOKEN_PLUS) {
        match(TOKEN_PLUS);
        value += Term();
    }

    return value;
}

// Term → identifier | number
int Term() {

    int value;

    if (currentToken.type == TOKEN_NUMBER) {
        value = atoi(currentToken.lexeme);
        match(TOKEN_NUMBER);
    }
    else if (currentToken.type == TOKEN_IDENTIFIER) {
        value = getVariable(currentToken.lexeme);
        match(TOKEN_IDENTIFIER);
    }
    else {
        error();
    }

    return value;
}

//MAIN

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: ./parser inputfile\n");
        return 1;
    }

    source = fopen(argv[1], "r");
    if (!source) {
        printf("Cannot open file\n");
        return 1;
    }

    getNextToken();
    Program();

    if (currentToken.type != TOKEN_EOF)
        error();

    fclose(source);
    return 0;
}
