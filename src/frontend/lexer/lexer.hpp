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
        error->lines = lines;
        error->fileName = fileName;
    }

    ~Lexer() {
        tokens.clear();
        lines.clear();
        source.clear();
    }

    // lex source
    TokenList lex();

    vector<string> getLines() { return this->lines; }

    void debugPrint();

private:
    string source;
    vector<string> lines;
    TokenList tokens;
    Token currentToken;
    size_t idx, col, line;
    ErrorManager *error;

    bool hasLetter = false; // identifier error

    // split source into vector of lines
    void parseLines();
    // peek to next chararacter
    char peek(int offset = 1);
    // advance to next character
    void advance(int steps = 1);
    // reset column and increment line
    void resetPos();
    // get current character
    char current();
    // get next token
    void getToken();
    // reset currenToken and push to tokens
    void endToken();
    // get identifier
    bool getIdent();
    // get string
    void getString();
    // get number, float
    void getNumber();
    // skip comments
    bool skipComment();
    // check identifiers for keywords
    void checkKeyword();

};