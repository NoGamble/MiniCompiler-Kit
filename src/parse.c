#include "../include/parse.h"

typedef struct Parser {
    LexerState *lex;
    SymbolTable *sym;
    Token look;
} Parser;

static const char* tokenTypeName(TokenType t)
{
    switch(t)
    {
        case T_BEGIN: return "begin";
        case T_END: return "end";
        case T_INTEGER: return "integer";
        case T_IF: return "if";
        case T_THEN: return "then";
        case T_ELSE: return "else";
        case T_FUNCTION: return "function";
        case T_READ: return "read";
        case T_WRITE: return "write";
        case T_IDENTIFIER: return "identifier";
        case T_NUMBER: return "number";
        case T_EQ: return "=";
        case T_NEQ: return "<>";
        case T_LE: return "<=";
        case T_LT: return "<";
        case T_GE: return ">=";
        case T_GT: return ">";
        case T_MINUS: return "-";
        case T_MUL: return "*";
        case T_ASSIGN: return ":=";
        case T_LPAREN: return "(";
        case T_RPAREN: return ")";
        case T_SEMICOLON: return ";";
        case T_EOLN: return "EOLN";
        case T_EOF: return "EOF";
        case T_PROCEDURE: return "procedure";
        default: return "Unknown";
    }
}

static void writeToken(Parser *p, Token t)
{
    if(p->lex->outputFile)
    {
        fprintf(p->lex->outputFile, "%s %d\n", t.lexme, t.type);
    }
}

static void next(Parser *p)
{
    do {
        p->look = getNextToken(p->lex);
        writeToken(p, p->look);
    } while(p->look.type == T_EOLN);
}

static void syntaxError(Parser *p, const char *msg)
{
    fprintf(p->lex->errorFile, "***LINE:%d %s\n", p->look.line, msg);
}

static void syntaxErrorExpect(Parser *p, TokenType expected)
{
    fprintf(p->lex->errorFile, "***LINE:%d Missing '%s'\n",
            p->look.line,
            tokenTypeName(expected));
}

static void match(Parser *p, TokenType t)
{
    if(p->look.type == t) next(p);
    else
    {
        syntaxErrorExpect(p, t);
        next(p);
    }
}

static void skipEOLN(Parser *p)
{
    while(p->look.type == T_EOLN) next(p);
}

static int isRelop(TokenType t)
{
    return t==T_EQ||t==T_NEQ||t==T_LE||t==T_LT||t==T_GE||t==T_GT;
}

static void parseExpr(Parser *p);

static void parseFactor(Parser *p)
{
    if(p->look.type == T_LPAREN)
    {
        match(p, T_LPAREN);
        parseExpr(p);
        match(p, T_RPAREN);
    }
    else if(p->look.type == T_IDENTIFIER)
    {
        // function call or variable
        Token id = p->look;
        next(p);
        if(p->look.type == T_LPAREN)
        {
            match(p, T_LPAREN);
            if(p->look.type != T_RPAREN)
            {
                parseExpr(p);
            }
            match(p, T_RPAREN);
        }
        (void)id;
    }
    else if(p->look.type == T_NUMBER) next(p);
    else
    {
        fprintf(p->lex->errorFile, "***LINE:%d Invalid factor\n",
                p->look.line);
        next(p);
    }
}

static void parseTerm(Parser *p)
{
    parseFactor(p);
    while(p->look.type == T_MUL)
    {
        match(p, T_MUL);
        parseFactor(p);
    }
}

static void parseExpr(Parser *p)
{
    parseTerm(p);
    while(p->look.type == T_MINUS)
    {
        match(p, T_MINUS);
        parseTerm(p);
    }
}

static void parseCondition(Parser *p)
{
    parseExpr(p);
    if(isRelop(p->look.type))
    {
        next(p);
        parseExpr(p);
    }
}

static void parseAssign(Parser *p)
{
    if(p->look.type == T_IDENTIFIER)
    {
        next(p);
        match(p, T_ASSIGN);
        while(p->look.type == T_EQ) { next(p); }
        parseExpr(p);
    }
    else
    {
        syntaxErrorExpect(p, T_IDENTIFIER);
        next(p);
    }
}

static void parseRead(Parser *p)
{
    match(p, T_READ);
    match(p, T_LPAREN);
    if(p->look.type == T_IDENTIFIER) next(p); else syntaxErrorExpect(p, T_IDENTIFIER);
    match(p, T_RPAREN);
}

static void parseWrite(Parser *p)
{
    match(p, T_WRITE);
    match(p, T_LPAREN);
    parseExpr(p);
    match(p, T_RPAREN);
}

static void parseStmt(Parser *p);
static int isStmtStart(TokenType t);

static void parseIf(Parser *p)
{
    match(p, T_IF);
    parseCondition(p);
    match(p, T_THEN);
    skipEOLN(p);
    parseStmt(p);
    if(p->look.type == T_SEMICOLON)
    {
        fprintf(p->lex->errorFile, "***LINE:%d Unexpected ';' before 'else'\n", p->look.line);
        match(p, T_SEMICOLON);
    }
    skipEOLN(p);
    if(p->look.type == T_ELSE)
    {
        match(p, T_ELSE);
        skipEOLN(p);
        if(!isStmtStart(p->look.type))
        {
            fprintf(p->lex->errorFile, "***LINE:%d Missing statement after 'else'\n", p->look.line);
        }
        else
        {
            parseStmt(p);
        }
    }
}

static int isStmtStart(TokenType t)
{
    return t==T_READ||t==T_WRITE||t==T_IF||t==T_IDENTIFIER;
}

static void parseStmt(Parser *p)
{
    if(p->look.type == T_READ) parseRead(p);
    else if(p->look.type == T_WRITE) parseWrite(p);
    else if(p->look.type == T_IF) parseIf(p);
    else parseAssign(p);
}

static void parseDecl(Parser *p)
{
    // integer id
    match(p, T_INTEGER);
    if(p->look.type == T_IDENTIFIER)
    {
        symtabAddVariable(p->sym, p->look.lexme, VT_INT, VK_VARIABLE);
        next(p);
    }
    else
    {
        syntaxErrorExpect(p, T_IDENTIFIER);
        next(p);
    }
}

static int parseDeclList(Parser *p)
{
    int had = 0;
    while(p->look.type == T_INTEGER)
    {
        parseDecl(p);
        had = 1;
        if(p->look.type == T_SEMICOLON)
        {
            match(p, T_SEMICOLON);
            skipEOLN(p);
        }
        else break;
    }
    return had;
}

static void parseStmtList(Parser *p)
{
    if(!isStmtStart(p->look.type)) return;
    parseStmt(p);
    while(p->look.type == T_SEMICOLON)
    {
        match(p, T_SEMICOLON);
        skipEOLN(p);
        if(isStmtStart(p->look.type))
        {
            parseStmt(p);
        }
        else break;
    }
}

static void parseDeclOrFunc(Parser *p)
{
    match(p, T_INTEGER);
    if(p->look.type == T_FUNCTION)
    {
        match(p, T_FUNCTION);
        if(p->look.type == T_IDENTIFIER)
        {
            char fname[32];
            strncpy(fname, p->look.lexme, sizeof(fname)-1);
            fname[sizeof(fname)-1] = '\0';
            next(p);
            match(p, T_LPAREN);
            if(p->look.type == T_IDENTIFIER)
            {
                symtabEnterProc(p->sym, fname, PT_FUNCTION);
                symtabAddVariable(p->sym, p->look.lexme, VT_INT, VK_PARAMETER);
                next(p);
            }
            match(p, T_RPAREN);
            match(p, T_SEMICOLON);
            match(p, T_BEGIN);
            skipEOLN(p);
            (void)parseDeclList(p);
            parseStmtList(p);
            match(p, T_END);
            symtabExitProc(p->sym);
        }
        else
        {
            syntaxErrorExpect(p, T_IDENTIFIER);
            next(p);
        }
    }
    else if(p->look.type == T_IDENTIFIER)
    {
        symtabAddVariable(p->sym, p->look.lexme, VT_INT, VK_VARIABLE);
        next(p);
    }
    else
    {
        syntaxError(p, "Invalid declaration");
        next(p);
    }
}

static void parseBlock(Parser *p)
{
    match(p, T_BEGIN);
    skipEOLN(p);
    while(p->look.type == T_INTEGER)
    {
        parseDeclOrFunc(p);
        if(p->look.type != T_SEMICOLON)
        {
            fprintf(p->lex->errorFile, "***LINE:%d Missing ';' after declaration\n", p->look.line);
            break;
        }
        match(p, T_SEMICOLON);
        skipEOLN(p);
    }
    // stmt list
    parseStmtList(p);
    // outer block: recover to the last 'end' before EOF
    if(p->look.type == T_END) next(p);
    else
    {
        int sawEnd = 0;
        while(p->look.type != T_EOF)
        {
            if(p->look.type == T_END)
            {
                sawEnd = 1;
            }
            next(p);
        }
        if(!sawEnd)
        {
            fprintf(p->lex->errorFile, "***LINE:%d Missing 'end'\n", p->look.line);
        }
    }
}

void runParser(const char *sourcePath, const char *outputPath)
{
    Parser p;
    char lexbase[256];
    snprintf(lexbase, sizeof(lexbase), "%s.parse", outputPath);
    p.lex = lexerInit(sourcePath, lexbase);
    // Re-route parser outputs: append errors to shared .err and disable .dyd
    if(p.lex->errorFile)
    {
        fclose(p.lex->errorFile);
    }
    char sharedErr[256];
    snprintf(sharedErr, sizeof(sharedErr), "%s.err", outputPath);
    p.lex->errorFile = fopen(sharedErr, "a");
    if(p.lex->outputFile)
    {
        fclose(p.lex->outputFile);
        p.lex->outputFile = NULL;
    }
    // Remove temporary parser-specific files to keep only four outputs
    char parseErr[256];
    char parseDyd[256];
    snprintf(parseErr, sizeof(parseErr), "%s.parse.err", outputPath);
    snprintf(parseDyd, sizeof(parseDyd), "%s.parse.dyd", outputPath);
    remove(parseErr);
    remove(parseDyd);
    p.sym = symtabInit();
    symtabEnterProc(p.sym, "global", PT_PROGRAM);
    printf("----------------------------------------\n");
    printf("Starting syntax analysis...\n");
    next(&p);
    parseBlock(&p);
    while(p.look.type != T_EOF)
    {
        next(&p);
    }
    char varfile[256];
    char profile[256];
    snprintf(varfile, sizeof(varfile), "%s.var", outputPath);
    snprintf(profile, sizeof(profile), "%s.pro", outputPath);
    generateVarFile(p.sym, varfile);
    generateProcFile(p.sym, profile);
    printf("Syntax Analysis completed! \n Result has written to %s.err, %s.var, %s.pro\n", outputPath, outputPath, outputPath);
    printf("----------------------------------------\n");
    symtabExitProc(p.sym);
    symtabCleanup(p.sym);
    lexerCleanup(p.lex);
}