/**
 * @file parser.cpp
 * @author fuechs
 * @brief fux parser
 * @version 0.1
 * @date 2022-11-06
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "parser.hpp"

AST *Parser::parse() {
    // lexing
    tokens = lexer->lex();
    
    if (fux.options.debugMode)
        for (Token token : tokens)
            token.debugPrint();

    // parsing 
    while (notEOF())
        root->addSub(parseStmt(root));

    return root;
}

AST *Parser::parseStmt(AST *parent) {
    return parseExpr(parent);
}

AST *Parser::parseExpr(AST* parent) {
    return parsePrimaryExpr(parent);
}

AST *Parser::parsePrimaryExpr(AST* parent) {
    Token curTok = eat();
    switch (curTok.type) {
        case IDENTIFIER:    
            return new AST(parent, AST_IDENTIFIER, curTok.line, curTok.start, curTok.value);

        case NUMBER:        
            return new AST(parent, AST_NUMERIC_LITERAL, curTok.line, curTok.start, curTok.value);
        
        default:            
            stringstream err;
            err << TokenTypeString[curTok.type] << " '" << curTok.value << "'";
            error->createError(UNEXPECTED_TOKEN, curTok, err.str());
            return new AST(parent);
    }
}

int64_t Parser::parseNumber(string str) {
    return (int64_t) atoll(str.c_str());
}

Token Parser::eat() {
    Token prev = tokens[0];
    removeFirstToken();
    return prev;
}

bool Parser::notEOF() { 
    return tokens[0].type != _EOF; 
}

void Parser::removeFirstToken() { 
    tokens.erase(tokens.begin()); 
}