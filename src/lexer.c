#include "../include/lexer.h"
#include "../include/keywords.h"

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
    char output[MAX_PATH_LEN];
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

Token handleIdentifier(LexerState *state)
{
    Token token;
    int index = 0;
    while(isalnum(state->lookhead) || state->lookhead == '_' && state->lookhead != EOF)
    {
        if(index < MAX_IDENTIFERLEN_LEN)
        {
            token.lexme[index] = state->lookhead;
            index++;
        }
        state->lookhead = getChar(state);
    }
    token.lexme[index] = '\0';
    token.type = getKeywordType(token.lexme);
    token.line = state->currentLine;
    return token;
}

Token handleNum(LexerState *state)
{
    Token token;
    int index;
    while(isdigit(state->lookhead) && state->lookhead != EOF)
    {
        if(index < MAX_NUMBER_LEN)
        {
            token.lexme[index] = state->lookhead;
            index ++;
        }
    }
    token.lexme[index] = '\0';
    token.type = T_NUMBER;
    return token;
}

Token handleOperator(LexerState *state)
{

}

Token getNextToken(LexerState *state)
{
    Token token;
    while(state->lookhead != EOF)
    {
        skipSpace(state);
        skipComment(state);
        if(state->lookhead == EOF)
        {
            break;
        }
        if(isalpha(state->lookhead) || state->lookhead == '_')
        {
            return handleIdentifier(state);
        }
        else if(isdigit(state->lookhead))
        {
            return handleNum(state);
        }
        else {
            return handleOperator(state);
        }
    }
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

void ungetChar(LexerState *state, int ch)
{
    ungetc(ch, state->sourceFile);
    if(ch == '\n')
    {
        state->currentLine--;
    }
}

void skipComment(LexerState *state)
{
    if(state->lookhead == '/')
    {
        state->lookhead = getChar(state);
        if(state->lookhead == '*')
        {
            
        }
        else if(state->lookhead == '/')
        {
            while(state->lookhead != '\n' && state->lookhead != EOF)
            {
                state->lookhead = getChar(state);
            }
        }
        else{

        }
    }

}

void skipSpace(LexerState *state)
{
    if(isspace(state->lookhead) && state->lookhead != EOF)
    {
        state->lookhead = getChar(state);
    }
}

TokenType getKeywordType(const char *lexme)
{
    for(int i=0; keywords[i].keyword != NULL; i++)
    {
        if(strcmp(lexme, keywords[i].keyword) == 0)
        {
            return keywords[i].type;
        }
    }
    return T_INDENTIFIER;

}
