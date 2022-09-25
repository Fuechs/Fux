#ifndef TOKEN_FUX_H
#define TOKEN_FUX_H

#include <string>

// All tokens we need to lex Hello World for now
typedef enum {
    HASH,           // #
    LESST,          // <
    GREATERT,       // >
    LPAREN,         // (
    RPAREN,         // )
    LBRACE,         // {
    RBRACE,         // }
    COLON,          // :
    DOT,            // .
    SEMICOLON,      // ;
    STRING,         // "STRING"
    IDENTIFIER,     // IDENTIFIER
} TokenType; 

typedef struct {
    TokenType type;
    std::string value;
} Token;

std::string token_to_string(Token token);

std::string token_type_to_value(TokenType type);

#endif