/**
 * @file lexer.hpp
 * @author fuechs
 * @brief fux lexer header
 * @version 0.1
 * @date 2022-11-01
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */


#pragma once

#include "../../../../include.hpp"
#include "token.hpp"
#include "../error.hpp"

class Lexer {
public:
    Lexer(const string source = "") : source(source) {
        tokens = {};
        currentToken.type = NONE;
        currentToken.value = "";
        idx = 0;
        col = 1;
        line = 1;
    }

    TokenList lex();

private:
    const string source;
    TokenList tokens;
    Token currentToken;
    size_t idx, col, line;

    char peek(int offset = 1);
    void advance(int steps = 1);
    void resetPos();
    char current();
    void getToken();
    void endToken();
    bool getIdent();
};