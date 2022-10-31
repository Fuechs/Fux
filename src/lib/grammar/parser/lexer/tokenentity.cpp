/**
 * @file tokenentity.cpp
 * @author fuechs
 * @brief fux token entity
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rigts reserved.
 * 
 */

#include "tokenentity.hpp"

TokenEntityID TokenEntity::getID() {
    return id;
}

void TokenEntity::setID(TokenEntityID id) {
    this->id = id;
}

string TokenEntity::getToken() {
    return tok;
}

int TokenEntity::getColumn() {
    return col;
}

int TokenEntity::getLine() {
    return line;
}

bool TokenEntity::isSingle() {
    return id == SINGLE;
}

TokenType TokenEntity::getTokenType() {
    return type;
}