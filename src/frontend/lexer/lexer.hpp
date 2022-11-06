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

#include "../../fux.hpp"
#include "token.hpp"
#include "../error/error.hpp"

class Lexer {
public:
    Lexer(const string source, const string fileName, ErrorManager *error) 
    : source(source), tokens({}), currentToken(Token()), idx(0), col(1), line(1), error(error) {
        parseLines();
        error->setLines(this->lines);
        error->setFileName(fileName);
    }

    ~Lexer() {
        tokens.clear();
        lines.clear();
        source.clear();
        error->setFileName(fux.options.fileName);
    }

    TokenList lex();

private:
    string source;
    vector<string> lines;
    TokenList tokens;
    Token currentToken;
    size_t idx, col, line;
    ErrorManager *error;

    bool hasLetter = false; // identifier error

    void parseLines();
    char peek(int offset = 1);
    void advance(int steps = 1);
    void resetPos();
    char current();
    void getToken();
    void endToken();
    bool getIdent();
    void getString();
    void getNumber();
    bool skipComment();

};