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
    if (isalpha(current())
    ||  isdigit(current()
    ||  current() == '_')) {
        currentToken.value.push_back(current());
        return true;
    }
    
    return false;
}