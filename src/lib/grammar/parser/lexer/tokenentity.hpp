/**
 * @file tokenentity.hpp
 * @author fuechs
 * @brief fux token entitiy header
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "../../../../include.hpp"
#include "tokentype.hpp"

enum TokenEntityID {
    IDENTIFIER,
    NATIVE_TYPE,
    TYPE_IDENTIFIER,
    INTEGER_LITERAL,
    HEX_LITERAL,
    PACKAGENAME,
    STRING_LITERAL,
    CHAR_LITERAL,
    LITERAL,
    VALUE,
    ACCESSTYPE,
    SINGLELINE_COMMENT,
    RETURN_STATEMENT,

    SINGLE,     // single token
    NOENTITIY, 
};

class TokenEntity {
public:
    TokenEntity(string val, TokenEntityID tokenid, int col, int line, TokenType type = NONE)
    : tok(val), type(type), id(tokenid), col(col), line(line) {}

    TokenEntity() 
    : tok(""), type(NONE), id(NOENTITIY), col(0), line(0) {}

    ~TokenEntity() { tok.clear(); } 

    bool operator!=(const TokenEntityID &id) {
        return this->id != id;
    }

    void operator=(const TokenEntity &entity) {
        tok = entity.tok;
        line = entity.line;
        col = entity.col;
        id = entity.id;
        type = entity.type;
    }

    bool operator!=(const TokenType &ty) {
        return this->type != ty;
    }

    bool operator==(const string &s) {
        return this->tok == s;
    }

    TokenEntityID getID();
    void setID(TokenEntityID id);
    string getToken();
    int getColumn();
    int getLine();
    bool isSingle();
    TokenType getTokenType();

private:
    int line;
    int col;
    TokenEntityID id;
    TokenType type;
    string tok;
}; 