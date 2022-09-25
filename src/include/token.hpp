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
    PLUS,           // +
    INTEGER,        // INTEGER
    STRING,         // "STRING"
    IDENTIFIER,     // IDENTIFIER
} TokenType; 



typedef struct {
    TokenType type;
    std::string value;
    size_t line;
    size_t col;
} Token;

std::string token_to_string(Token token);

std::string token_type_to_value(TokenType type);

#endif