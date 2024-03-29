/**
 * @file lexer.hpp
 * @author fuechs
 * @brief fux lexer header
 * @version 0.1
 * @date 2022-11-01
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */


#pragma once

#include "../../fux.hpp"
#include "token.hpp"
#include "../error/error.hpp"

class Lexer {
public:
    Lexer(const string source, const string &fileName, ErrorManager *error) 
    : fileName(fileName), source(source), tokens({}), currentToken(Token()), 
        idx(0), col(1), line(1), error(error) {
            parseLines();
            error->addSourceFile(fileName, lines);
    }

    ~Lexer() {
        tokens.clear();
        lines.clear();
        source.clear();
    }

    // lex source
    Token::Vec lex();

    vector<string> getLines() { return this->lines; }

    void debugPrint();

private:
    const string &fileName;
    string source;
    vector<string> lines;
    Token::Vec tokens;
    Token currentToken;
    size_t idx, col, line;
    ErrorManager *error;

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
    void getIdentifier();
    // get string
    void getString();
    // get number
    void getNumber();
    // skip comments
    bool skipComment();
    // check identifiers for keywords
    void checkKeyword();

};