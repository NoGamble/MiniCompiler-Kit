#ifndef LEXER_H
#define LEXER_H


#include "common.h"
#include "keywords.h"

typedef struct
{
    FILE *sourceFile;
    FILE *outputFile;
    FILE *errorFile;
    int currentLine;
    int currentChar;
    int lookahead;

} LexerState;

LexerState* lexerInit(const char *source_path, const char *output_path);
void lexerCleanup(LexerState *state);
Token getNextToken(LexerState *state);
void runLexer(const char *sourcePath, const char *outputPath);

TokenType getKeywordType(const char *lexme);

Token handleIdentifier(LexerState *state);
Token handleNum(LexerState *state);
Token handleOperator(LexerState *state);

int getChar(LexerState *state);
void ungetChar(LexerState *state, int ch);
void skipComment(LexerState *state);
void skipSpace(LexerState *state);

#endif