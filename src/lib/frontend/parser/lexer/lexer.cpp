/**
 * @file lexer.cpp
 * @author fuechs
 * @brief fux lexer 
 * @version 0.1
 * @date 2022-11-01
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "lexer.hpp"

TokenList Lexer::lex() {
    while (idx < source.length()) {
        getToken();

        switch (currentToken.type) {
            case IDENTIFIER:
                while (getIdent())
                    advance();
                break;

            case STRING:
                getString();
                break;

            case NUMBER:
                getNumber();
                break;

            default: break;
        }

        endToken();
        // update line and column for next token
        currentToken.col = col;
        currentToken.line = line; 
    }

    currentToken.type = _EOF;
    currentToken.value = TokenTypeValue[_EOF];
    endToken();

    return tokens;
}

void Lexer::parseLines() {
    stringstream line;

    for (int i = 0; i < source.length(); i++) {
        if (source[i] == '\n') {
            lines.add(line.str());
            line.str("");
        } else
            line << source[i];
    }

    if (!line.str().empty())
        lines.add(line.str());
}

char Lexer::peek(int offset) {
    return source[idx+offset];
}

void Lexer::advance(int steps) {
    idx += steps;
    col += steps;
}

void Lexer::resetPos() {
    col = 1;
    ++line;
}

char Lexer::current() {
    return source[idx];
}

void Lexer::getToken() {

}

void Lexer::endToken() {
    if (currentToken.type != NONE)
        tokens.push_back(currentToken);
    
    currentToken.type = NONE;
    currentToken.value.erase();
}

bool Lexer::getIdent() {
    if ((isalpha(current())
    ||  isdigit(current())
    ||  current() == '_')
    &&  idx < source.length()) {
        currentToken.value.push_back(current());
        return true;
    }
    
    return false;
}

void Lexer::getString() {
    advance();
    if (idx >= source.length())
        errors->createNewError(EXPECTED_STRING_LITERAL_EOF, line, col, "unterminated string");
    
    while (current() != '"') {
        currentToken.value.push_back(current());
        advance();

        if (idx >= source.length())
            errors->createNewError(EXPECTED_STRING_LITERAL_EOF, line, col, "unterminated string");
    }

    advance();
}

void Lexer::getNumber() {

}