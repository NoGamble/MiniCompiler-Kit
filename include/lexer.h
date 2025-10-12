#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "common.h"

typedef struct
{
    FILE *sourceFile;
    FILE *outputFile;
    FILE *errorFile;
    int currentLine;
    int currentChar;
    int lookhead;

} LexerState;

LexerState* lexerInit(const char *source_path, const char *output_path);
void lexerCleanup(LexerState *state);
Token getNextToken(LexerState *state);
void runLexer(const char *sourcePath, const char *outputPath);

Token getKeywordType(const char *lexme);

int getChar(LexerState *state);
void ungetChar(LexerState *state, int ch);
void skipComment(LexerState *state);

#endif