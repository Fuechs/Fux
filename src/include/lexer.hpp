#ifndef LEXER_FUX_H
#define LEXER_FUX_H

#include <iostream>
#include <string>
#include <vector>

#include "token.hpp"

class Lexer {
public:
    Lexer(std::string source) {
        this->source = source;
    }

    std::vector<Token> lex();

private:
    std::string source;
    size_t idx = 0; // char idx in source
    std::vector<Token> tokens;

    char peek(int offset = 1) {
        return source[idx+offset];
    }

    Token tokenize();
};

#endif