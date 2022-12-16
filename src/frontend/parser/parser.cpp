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

RootAST *Parser::parse() {
    // lexing
    tokens = lexer->lex();
    
    if (fux.options.debugMode)
        for (Token token : tokens)
            token.debugPrint();
        
    current = tokens.begin();

    ExprPtr branch;

    while (notEOF()) {
        branch = parseStmt();
        root->addSub(branch);
    }
    
    delete &branch;
    return root;
}

// * orders of precedence
// AssignmentExpr
// MemberExpr
// FunctionCall
// LogicalExpr
// ComparisonExpr
// AdditiveExpr
// MutiplicativeExpr
// UnaryExpr
// PrimaryExpr

ExprPtr Parser::parseStmt() {
    return parsePrimaryExpr();
}

ExprPtr Parser::parsePrimaryExpr() {
    Token that = eat();
    switch (that.type) {
        case NUMBER:        return make_unique<NumberExprAST>(stod(that.value.c_str()));
        case IDENTIFIER:    return make_unique<VariableExprAST>(that.value);
        default:            
            error->createError(UNEXPECTED_TOKEN, that, "unexpected token while parsing primary expression");
            return nullptr;
    }
}


Token Parser::eat() {
    if (current->type == _EOF)
        return *current;
    return *current++;
}

Token Parser::expect(TokenType type, ErrorType errType) {
    Token curTok = eat();
    if (curTok.type != type) {
        stringstream err;
        err 
            << "expected " << TokenTypeString[type] 
            << " '" << TokenTypeValue[type] << "', "
            << "got " << TokenTypeString[curTok.type] 
            << " '" << curTok.value << "' instead";
        error->createError(errType, curTok, err.str());
        return Token();
    }

    return curTok;
}

bool Parser::notEOF() { 
    return current->type != _EOF;
}