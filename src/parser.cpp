/**
 * @file parser.cpp
 * @author your name (you@domain.com)
 * @brief fux parser 
 * @version 0.1
 * @date 2022-09-27
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs.
 *            All rights reserved.
 *            BSD 3-Clause License
 * 
 */

#include "include/parser.hpp"

namespace fux {

    void Parser::parse() {
        
        while (currentToken->type != EOS) {

            if (!parseFunctionDefinition() && !parseFunctionCall()) {
                exit(0);
            }

        }
    }

    Token Parser::expectType(TokenType type) {
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

    bool Parser::matchType(TokenType type) {
        return currentToken->type == type;
    }

    bool Parser::parseFunctionDefinition() {
        Token functionName = expectType(IDENTIFIER);
        expectType(LPAREN); // TODO: implement paramaters
        expectType(RPAREN);
        expectType(COLON);
        Type functionType = parseType();
        // std::cout << "Function " << functionName.value << " of type " << functionType.value << ".\n";
        parseBlock();

        return false;
    }

    bool Parser::parseFunctionCall() {return false;}

    Type Parser::parseType() {
        Token typeIdent = expectType(IDENTIFIER);
        if (types.find(typeIdent.value) == types.end())
            ParserError(
                "Unknown type '"+typeIdent.value+"'.",
                *currentToken
            );
        return types[typeIdent.value];
    }

    bool parseBlock() {return false;}

}