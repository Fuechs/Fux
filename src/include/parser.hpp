#ifndef PARSER_FUX_H
#define PARSER_FUX_H

#include <iostream>
#include <vector>

#include "token.hpp"

class Parser {
public:
    Parser(std::vector<Token> tokens) {
        this->tokens = tokens;
    }

    int parse();

private:
    int idx = 0;
    std::vector<Token> tokens;

    void eat(TokenType expected) {
        
        if (peek().type == expected) 
            idx++;
        else {
            std::cout << "Unexpected Token: ";
            std::cout << "'" << peek().value << "'\n";
            std::cout << "Expected '" << token_type_to_value(expected) << "' instead.\n";
        }

    }

    Token peek(int offset = 1) {
        return tokens[idx+offset];
    }
};

#endif