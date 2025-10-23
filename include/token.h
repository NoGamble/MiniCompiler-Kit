typedef enum TokenType{
    // Keywords: 1-9
    T_BEGIN = 1,
    T_END = 2,
    T_INTEGER = 3,
    T_IF = 4,
    T_THEN = 5,
    T_ELSE = 6,
    T_FUNCTION = 7,
    T_READ = 8,
    T_WRITE = 9,
    
    // Identifiers and literals: 10-11
    T_IDENTIFIER = 10,     // Variable/function names
    T_NUMBER = 11,          // Numeric constants

    // Operators and delimiters: 12-23
    T_EQ = 12,
    T_NEQ = 13,             // Not equal (<>)
    T_LE = 14,
    T_LT = 15,
    T_GE = 16,
    T_GT = 17,
    T_MINUS = 18,
    T_MUL = 19,
    T_ASSIGN = 20,          // Assign (:=)
    T_LPAREN = 21,
    T_RPAREN = 22,
    T_SEMICOLON = 23,

    // Special tokens
    T_EOLN = 24,
    T_EOF = 25,

    //Unknown or error
    T_PROCEDURE = 26,
    T_UNKNOWN = 27

} TokenType;

typedef struct
{
    char lexme[32];
    TokenType type;
    int line;
} Token;
