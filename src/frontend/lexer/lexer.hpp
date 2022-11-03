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
    Lexer(const string source = "", const string fileName = "") 
    : source(source), tokens({}), currentToken(Token()), idx(0), col(0), line(0) {
        parseLines();
        errors = new ErrorManager(lines, fileName, false, false);
    }

    ~Lexer() {
        idx = 0;
        col = 0;
        line = 0;
        tokens.clear();
        currentToken.~Token();
        lines.free();
        errors->free();
        delete errors;
        errors = NULL;
        source.clear();
    }

    TokenList lex();

private:
    string source;
    List<string> lines;
    TokenList tokens;
    Token currentToken;
    size_t idx, col, line;
    ErrorManager *errors;

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

    /* 
        from https://stackoverflow.com/a/3418285
        replace all substrings in a string
    */
    void replaceAll(std::string &str, const std::string &from, const std::string &to);
};