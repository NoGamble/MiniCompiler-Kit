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
    state->lookahead = ' ';

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
    while(isalnum(state->lookahead) || state->lookahead == '_' && state->lookahead != EOF)
    {
        if(index < MAX_IDENTIFERLEN_LEN)
        {
            token.lexme[index] = state->lookahead;
            index++;
        }
        state->lookahead = getChar(state);
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
    while(isdigit(state->lookahead) && state->lookahead != EOF)
    {
        if(index < MAX_NUMBER_LEN)
        {
            token.lexme[index] = state->lookahead;
            index ++;
        }
        state->lookahead = getChar(state);
    }
    token.lexme[index] = '\0';
    token.type = T_NUMBER;
    token.line = state->currentLine;
    return token;
}

Token handleOperator(LexerState *state)
{
    Token token;
    token.line = state->currentLine;
    switch(state->lookahead)
    {
        case ':':
            state->lookahead = getChar(state);
            if(state->lookahead == '=')
            {
                strcpy(token.lexme, ":=");
                token.type = T_ASSIGN;
            } else {
                // error();
            }
        case '=':
            state->lookahead = getChar(state);
            // if(state->lookahead == '=')
    }

    return token;
}

Token getNextToken(LexerState *state)
{
    Token token;
    while(state->lookahead != EOF)
    {
        skipSpace(state);
        // skipComment(state); To be updated...
        if(state->lookahead == EOF)
        {
            break;
        }
        if(state->lookahead == '\n')
        {
            token.type = T_EOLN;
            strcmp(token.lexme, "EOLN");
            token.line = state->currentLine - 1;
            return token;
        }
        else if(isalpha(state->lookahead) || state->lookahead == '_')
        {
            return handleIdentifier(state);
        }
        else if(isdigit(state->lookahead))
        {
            return handleNum(state);
        }
        else {
            return handleOperator(state);
        }
    }
    strcpy(token.lexme, "EOF");
    token.type = T_EOF;
    token.line = state->currentLine;
    return token;
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

void skipComment(LexerState *state) // To be updated ...
{
    if(state->lookahead == '/')
    {
        state->lookahead = getChar(state);
        if(state->lookahead == '*')
        {
            state->lookahead = getChar(state);
            if(state->lookahead == '*')
            {

            }
        }
        else if(state->lookahead == '/')
        {
            while(state->lookahead != '\n' && state->lookahead != EOF) //until lookahead == '\n'
            {
                state->lookahead = getChar(state);
                if (state->lookahead == EOF) {
                    fprintf(state->errorFile, "***LINE:%d Unclosed comment\n", state->currentLine);
                    // Error();
                    break;
                }
            }
        }
        else{
            
        }
    }

}

void skipSpace(LexerState *state)
{
    if(isspace(state->lookahead) && state->lookahead != EOF)
    {
        state->lookahead = getChar(state);
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
    return T_IDENTIFIER;

}
