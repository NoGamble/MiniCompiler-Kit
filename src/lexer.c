#include "../include/lexer.h"

LexerState* lexerInit(const char *sourceFile, const char *outputFile)
{
    LexerState* state = malloc(sizeof(LexerState));
    if(!state) error("Memory allocation failed!");
    

}


void lexerCleanup(LexerState *state)
{

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

Token getKeywordType(const char *lexme)
{

}
