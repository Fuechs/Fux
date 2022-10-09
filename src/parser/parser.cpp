/**
 * @file parser.cpp
 * @author fuechs
 * @brief fux parser 
 * @version 0.1
 * @date 2022-09-27
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "parser.hpp"

namespace fux {

    AST Parser::parse() {
        // AST root = AST("root");
        // Statement decl;
        // decl.kind = DECLARATION;
        // Expression expr;
        // expr.kind = INTEGER;
        // expr.value = "1";
        // decl.expr = expr;
        // decl.value = "a";
        // root.stmts.push_back(decl);
        // return root;
        return AST("root");
    }

    /* Token Parser::expectTokenType(TokenType type) {
        if (currentToken->type != type) {
            ParserError(
                "Unexpected Token '"
                +currentToken->value
                +"' ("+TokenTypeString[currentToken->type]+") "
                +"expected '"+TokenTypeValue[type]+"' ("+TokenTypeString[type]+") instead.",
                *currentToken
            );
        }
        return *currentToken++;
    }

    Token Parser::peek() {
        TokenList::iterator prevToken = currentToken;
        TokenList::iterator returnToken = ++currentToken;
        currentToken = prevToken;
        return *returnToken;
    }

    bool Parser::matchTokenType(TokenType type) {
        return currentToken->type == type;
    }

    Type Parser::parseType() {
        Token typeIdent = expectTokenType(IDENTIFIER);
        if (types.find(typeIdent.value) == types.end())
            ParserError(
                "Unknown type '"+typeIdent.value+"'.",
                *currentToken
            );
        return types[typeIdent.value];
    } */

}