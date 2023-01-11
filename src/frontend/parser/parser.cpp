/**
 * @file parser.cpp
 * @author fuechs
 * @brief fux parser
 * @version 0.1
 * @date 2022-11-06
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#include "parser.hpp"

ExprPtr Parser::parse() {
    // lexing
    tokens = lexer->lex();
    lexer->debugPrint();
    
    current = tokens.begin();

    ExprPtr branch;
    while (notEOF()) 
        if ((branch = parseStmt())) // check for nullptr in case of error
            root->addSub(branch);
    
    return std::move(root);
}

ExprPtr Parser::parseStmt() {
    return parseIfElseStmt();
}

ExprPtr Parser::parseIfElseStmt() {
    ExprPtr stmt;

    if (current->type == KEY_IF) {
        eat();
        expect(LPAREN, ILLEGAL_BRACKET_MISMATCH);
        ExprPtr condition = parseStmt(); // ! parse conditional here
        expect(LPAREN, MISSING_BRACKET);
        ExprPtr thenBody = parseStmt(); // ! parse body or stmt here
        if (current->type == KEY_ELSE) {
            eat();
            ExprPtr elseBody = parseStmt(); // ! parse body or stmt here
            stmt = make_unique<IfElseAST>(condition, thenBody, elseBody);
        } else
            stmt = make_unique<IfElseAST>(condition, thenBody);
    } else
        stmt = parsePutsStmt();

    return stmt;
}

ExprPtr Parser::parsePutsStmt() {
    ExprPtr call;
    if (current->type == KEY_PUTS) {
        eat();
        ExprPtr arg = parseExpr();
        call = make_unique<PutsCallAST>(arg); // just parse expression for now
        expect(SEMICOLON);
    } else 
        call = parseExpr(); // ! skipping variabledeclstmt here

    return call;
}

ExprPtr Parser::parseVariableDeclStmt() {
    // TODO: parse storage modifiers
    const string symbol = expect(IDENTIFIER).value;
    const FuxType type = FuxType(); // TODO: parse type
    expect(EQUALS); // TODO: parse constant
    ExprPtr value = parseExpr();
    expect(SEMICOLON);
    return make_unique<VariableDeclAST>(symbol, type, value);
}

ExprPtr Parser::parseExpr() { return parseAssignmentExpr(); }

ExprPtr Parser::parseAssignmentExpr() { return parseMemberExpr(); }

ExprPtr Parser::parseMemberExpr() { return parseCallExpr(); }

ExprPtr Parser::parseCallExpr() { return parseAdditiveExpr(); } // ! skipping logicalexpr here

ExprPtr Parser::parseLogicalExpr() {
    ExprPtr LHS = parseComparisonExpr();

    while (current->type == AND || current->type == OR || current->type == EXCLAMATION) {
        char logical = current->value.front();
        ++current;
        ExprPtr RHS = parseComparisonExpr();
        LHS = make_unique<LogicalExprAST>(logical, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseComparisonExpr() {
    ExprPtr LHS = parseExpr();

    while ( current->type == EQUALS_EQUALS 
    ||      current->type == NOT_EQUALS 
    ||      current->type == LESSTHAN
    ||      current->type == LTEQUALS
    ||      current->type == GREATERTHAN
    ||      current->type == GTEQUALS) {
        char comp = current->value.front();
        ++current;
        ExprPtr RHS = parseExpr();
        LHS = make_unique<ComparisonExprAST>(comp, LHS, RHS);
    }

    return LHS;
} 

ExprPtr Parser::parseAdditiveExpr() {
    ExprPtr LHS = parseMultiplicativeExpr();

    while (current->type == PLUS || current->type == MINUS) {
        char op = current->value.front(); // get '+' or '-' (optimized out by compiler)
        ++current;
        ExprPtr RHS = parseMultiplicativeExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseMultiplicativeExpr() {
    ExprPtr LHS = parsePrimaryExpr();

    while (current->type == ASTERISK || current->type == SLASH || current->type == PERCENT) {
        char op = current->value.front(); // get '*', '/', '%' (optimized out by compiler)
        ++current;
        ExprPtr RHS = parsePrimaryExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseUnaryExpr() { return nullptr; }

ExprPtr Parser::parsePrimaryExpr() {
    Token that = eat();
    switch (that.type) {
        case NUMBER:        return make_unique<NumberExprAST>(FuxType(FuxType::I64), stod(that.value));
        case IDENTIFIER:    return make_unique<VariableExprAST>(that.value);
        case LPAREN: {
            ExprPtr expr = parseExpr();
            expect(RPAREN, MISSING_BRACKET);
            return expr;
        }
        default: {        
            stringstream message;
            message 
                << "unexpected token " << TokenTypeString[that.type]
                << " '" << that.value << "' while parsing primary expression";
            error->createError(UNEXPECTED_TOKEN, that, message.str());
            return nullptr;
        }
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

Token Parser::peek(size_t steps) {
    size_t old = steps;

    while (steps --> 0)
        ++current;

    Token ret = *current;

    while (steps ++< old)
        --current;

    return ret;
}

bool Parser::notEOF() { 
    return current->type != _EOF;
}