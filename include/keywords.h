#ifndef KEYWORDS_H
#define KEYWORDS_H
#include "token.h"
#include <stdio.h>
typedef struct
{
    const char *keyword;
    TokenType type;
} Keyword;

static Keyword keywords[] = {
    {"begin", T_BEGIN},
    {"end", T_END},
    {"integer", T_INTEGER},
    {"if", T_IF},
    {"then", T_THEN},
    {"else", T_ELSE},
    {"function", T_FUNCTION},
    {"read", T_READ},
    {"write", T_WRITE},
    {NULL, T_IDENTIFIER}
};

#endif