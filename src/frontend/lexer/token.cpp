/**
 * @file token.cpp
 * @author fuechs
 * @brief fux token
 * @version 0.1
 * @date 2023-01-22
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "token.hpp"

Token::Token(TokenType type, string value) 
: type(type), value(value), line(1), start(1), end(1) {}

Token::~Token() { value.clear(); }

Token &Token::operator=(const Token &copy) {
    this->type = copy.type;
    this->value = copy.value;
    this->line = copy.line;
    this->start = copy.start;
    this->end = copy.end;
    return *this;
}

bool Token::operator==(TokenType type) const { return this->type == type; }

bool Token::operator!=(TokenType type) const { return this->type != type; }

string Token::str() const {
    stringstream ss;
    ss  << "<Token, " << TokenTypeString[type] << ", '"  
        << value << "', "
        << "Line " << line << ":" << start << ">";
    // <Token, TYPE, 'value', Line line:start>
    return ss.str();
}


bool Token::isKeyword() const { return (type >= KEY_GET && type <= KEY_CLASS); }

bool Token::isType() const { return ((type >= KEY_VOID && type <= KEY_VAR) || type == IDENTIFIER); }

bool Token::isModifier() const { return (type >= KEY_SAFE && type <= KEY_FIXED); }

bool Token::isRelational() const { return (type >= LESSTHAN && type <= GTEQUALS); }

bool Token::isAssignment() const { return (type >= EQUALS && type <= SWAP); }

bool Token::isInbuiltCall() const { return (type >= KEY_RETURN && type <= KEY_GOTO); }