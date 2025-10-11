#include "../include/lexer.h"

LexerState* lexerInit(const char *sourcePath, const char *outputPath)
{
    LexerState* state = (LexerState*)(sizeof(LexerState));
    if(!state)
    {
        error("Memory allocation failed!");
        exit(EXIT_FAILURE);
    }
    memset(state, 0, sizeof(LexerState));
    state->sourceFile = fopen(sourcePath, "r");
    if(!state->sourceFile)
    {
        lexerCleanup(state);
        error("Cann't open the source file!");
    }
    char output[MAXPATHLEN];
    snprintf(output, sizeof(output), "%s.dyd", outputPath);
    state->outputFile = fopen(output, "w");
    if(!state->outputFile)
    {
        lexerCleanup(state);
        error("Cann't create output file!");
    }
    snprintf(output, siezof(output), "%s.err", outputPath);
    state->errorFile = fopen(output, "w");
    if(!state->errorFile)
    {
        lexerCleanup(state);
        error("Cann't create error file!");
    }
    state->currentLine = 1;
    state->currentChar = 0;
    state->lookhead = ' ';

    return state;
}

void lexerCleanup(LexerState *state)
{
    if(state)
    {
        if(state->sourceFile) fclose(state->sourceFile);
        if(state->outputFile) fclose(state->outputFile);
        if(state->errorFile) fclose(state->errorFile);
        free(state);
    }
}

Token getNextToken(LexerState *state)
{

}
void runLexer(const char *sourcePath, const char *outputPath)
{
    LexerState* state = lexerInit(sourcePath, outputPath);
    Token token;
    printf("Starting lexical analysis...\n");
    do
    {
        token = getNextToken(state);  
        fprintf(state->outputFile, "%s %d\n", token.lexme, token.type);
        // printf("Line %d: (%d, %s)\n", token.line, token.type, token.lexme); 
    } while (token.type != T_EOF);
    printf("Lexical Analysis completed! Result has written to %s.dyd\n", outputPath); 
    lexerCleanup(state);
}

int getChar(LexerState *state)
{
    int ch = fgetc(state->sourceFile);
    state->currentChar++;
    if(ch == '\n')
    {
        state->currentChar = 0;
        state->currentLine++;
    }
    return ch;
}

Token getKeywordType(const char *lexme)
{
    
}
