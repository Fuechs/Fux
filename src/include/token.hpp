#ifndef FUX_TOKEN_H
#define FUX_TOKEN_H

typedef enum {

    TT_INT,
    TT_FLOAT,
    TT_PLUS,
    TT_MINUS

} TokenType;

typedef double Value;

typedef struct {
    Value value;
    TokenType type;
} Token;

std::string as_string(TokenType type);

#endif