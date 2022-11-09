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

AST *Parser::parseStmt(AST *parent) {
    switch (tokens[0].type) {
        case SEMICOLON:         eat(); return parseStmt(parent); // skip semicolon
        case IDENTIFIER:        return parseVarDecl(parent);
        default:                return parseExpr(parent);
    }
}

AST *Parser::parseVarDecl(AST *parent) {
    if (tokens[1].type == SEMICOLON) {
        Token name = eat();
        eat(); // semicolon
        AST *variable = new AST(parent, AST_VARIABLE_DECl);
        AST *symbol = new AST(variable, AST_IDENTIFIER, name);
        variable->addSub(symbol);
        variable->copyPosition(symbol);
        variable->end++; // semicolon

        return variable;
    }

    if (tokens[1].type == COLON) {
        Token name = eat();
        eat(); // colon
        AST *variable = new AST(parent, AST_VARIABLE_DECl);
        AST *symbol = new AST(variable, AST_IDENTIFIER, name);
        variable->addSub(symbol);
        variable->copyPosition(symbol);
        
        if (tokens[0].type == EQUALS)
            variable->valueType = INT;
        else if (tokens[0].type == TRIPLE_EQUALS)
            variable->valueType = CONSTANT;
        eat(); // = or ===
        
        AST *value = parseExpr(variable);
        variable->addSub(value);
        variable->end = value->end + 1; // semicolon
        expect(SEMICOLON);

        return variable;
    } 
 
    return parseExpr(parent);

}

AST *Parser::parseExpr(AST *parent) {
    return parseAdditiveExpr(parent);
}

AST *Parser::parseAdditiveExpr(AST *parent) {
    AST *lhs = parseMultiplicativeExpr(parent);

    while (tokens[0].type == PLUS || tokens[0].type == MINUS) {
        AST *op = new AST(nullptr, AST_BINARY_OPERATOR, eat());
        AST *rhs = parseMultiplicativeExpr(nullptr);
        AST *copy = new AST(lhs); // copy old lhs
        lhs = new AST(parent, AST_BINARY_EXPR, copy->line, copy->start, rhs->end);
        // ( op lhs(copy) rhs )
        lhs->addSub(op); op->parent = lhs;
        lhs->addSub(copy); copy->parent = lhs;
        lhs->addSub(rhs); rhs->parent = lhs;
    }

    return lhs;
}

AST *Parser::parseMultiplicativeExpr(AST *parent) {
    AST *lhs = parsePrimaryExpr(parent);

    while (tokens[0].type == SLASH || tokens[0].type == ASTERISK || tokens[0].type == PERCENT) {
        AST *op = new AST(nullptr, AST_BINARY_OPERATOR, eat());
        AST *rhs = parsePrimaryExpr(nullptr);
        AST *copy = new AST(lhs); // copy old lhs
        lhs = new AST(parent, AST_BINARY_EXPR, copy->line, copy->start, rhs->end);
        // ( op lhs(copy) rhs )
        lhs->addSub(op); op->parent = lhs;
        lhs->addSub(copy); copy->parent = lhs;
        lhs->addSub(rhs); rhs->parent = lhs;
    }

    return lhs;
}

AST *Parser::parsePrimaryExpr(AST *parent) {
    Token curTok = eat(); //! (note to myself) skipping token here
    switch (curTok.type) {
        case LPAREN: {
            AST *value = parseExpr(parent);
            expect(RPAREN, MISSING_BRACKET);
            return value;
        }
        
        case IDENTIFIER:    
            return new AST(parent, AST_IDENTIFIER, curTok);

        case NUMBER:        
            return new AST(parent, AST_NUMERIC_LITERAL, curTok);
        
        case KEY_NULL:
            return new AST(parent, AST_NULL_LITERAL, curTok);
        
        default:            
            stringstream err;
            err << TokenTypeString[curTok.type] << " '" << curTok.value << "'";
            error->createError(UNEXPECTED_TOKEN, curTok, err.str());
            return new AST(parent, AST_NONE);
    }
}


// int64_t Parser::parseNumber(string str) {
//     return (int64_t) atoll(str.c_str());
// }

Token Parser::eat() {
    Token prev = tokens[0];
    if (prev.type == _EOF)
        return prev;
    tokens.erase(tokens.begin()); 
    return prev;
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
    return tokens[0].type != _EOF; 
}