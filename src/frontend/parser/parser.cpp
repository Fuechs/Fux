/**
 * @file parser.cpp
 * @author fuechs
 * @brief fux parser
 * @version 0.1
 * @date 2022-11-06
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "parser.hpp"

RootPtr Parser::parse() {
    // lexing
    tokens = lexer->lex();
    lexer->debugPrint();
    
    current = tokens.begin();

    StmtPtr branch;
    while (notEOF()) 
        if ((branch = parseStmt())) // check for nullptr in case of error
            root->addSub(branch);
    
    return std::move(root);
}

StmtPtr Parser::parseStmt() {
    StmtPtr stmt = parseIfElseStmt();
    if (stmt) // don't throw useless second error 
        expect(SEMICOLON);
    return stmt;
}

StmtPtr Parser::parseFunctionDeclStmt() {
    return parseBlockStmt();
}

StmtPtr Parser::parseBlockStmt() {
    if (check(LBRACE)) {
        StmtList body;
        while (!check(RBRACE)) {
            if (!notEOF()) {
                error->createError(MISSING_BRACKET, *(current - 1),
                    "Code block was never closed.");
                return nullptr; 
            }
            body.push_back(parseStmt());
        }
        return make_unique<CodeBlockAST>(body);
    } 
    
    return parseIfElseStmt();
}

StmtPtr Parser::parseIfElseStmt() {
    if (check(KEY_IF)) {
        eat();
        expect(LPAREN, ILLEGAL_BRACKET_MISMATCH);
        ExprPtr condition = parseExpr();
        expect(LPAREN, MISSING_BRACKET);
        StmtPtr thenBody = parseStmt(); 
        if (check(KEY_ELSE)) {
            StmtPtr elseBody = parseStmt(); 
            return make_unique<IfElseAST>(condition, thenBody, elseBody);
        } else
            return make_unique<IfElseAST>(condition, thenBody);
    } else
        return parsePutsStmt();
}

StmtPtr Parser::parsePutsStmt() {
    if (check(KEY_PUTS)) {
        ExprPtr arg = parseExpr();
        return make_unique<PutsCallAST>(arg);
    } else 
        return parseReturnStmt();
}

StmtPtr Parser::parseReturnStmt() {
    if (check(KEY_RETURN)) {
        ExprPtr arg = parseExpr();
        return make_unique<ReturnCallAST>(arg);
    } else
        return parseVariableDeclStmt();
}

StmtPtr Parser::parseVariableDeclStmt() {
    // TODO: parse storage modifiers
    FuxType::AccessList access = {FuxType::PUBLIC};
    // TODO: parse pointer depth
    size_t pointerDepth = 0;

    if (!check(IDENTIFIER))
        return parseExpr();

    const string symbol = peek(-1).value; // get value from identifier
    FuxType type = FuxType();
    
    if (check(COLON))
        type = parseType(pointerDepth, access);
    else if (check(RPOINTER)) { // reference
        if (pointerDepth > 0)
            error->createWarning(INCOMPATIBLE_TYPES, peek(-1), 
                "given pointer depth will be ignored and a reference parsed instead");
        type = parseType(-1, access);
        if (!type) {
            error->createError(UNEXPECTED_TOKEN, *current++, "expected a type after RPOINTER '->'");
            error->addNote(peek(-2), "automatic typing is not supported for references yet");
            return nullptr; // failed statement
        }
    } else {
        --current;
        return parseExpr();
    }
    
    if (check(EQUALS)) { // =
        if (!type)
            type = FuxType(FuxType::AUTO, 0, access);
    } else if (check(TRIPLE_EQUALS)) { // ===
        if (!type) {
            access.push_back(FuxType::CONSTANT);
            type = FuxType(FuxType::AUTO, pointerDepth, access);
        } else
            type.access.push_back(FuxType::CONSTANT);
    } else {
        error->createError(UNEXPECTED_TOKEN, *current, "expected an EQUALS '=' or a TRIPLE_EQUALS '===' in variable declaration");
        return nullptr;
    }

    ExprPtr value = parseExpr();
    return make_unique<VariableDeclAST>(symbol, type, value);
}

ExprPtr Parser::parseExpr() { return parseAssignmentExpr(); }

ExprPtr Parser::parseAssignmentExpr() { 
    ExprPtr dest = parseCallExpr();

    if (check(EQUALS)) {
        ExprPtr value = parseExpr();
        return make_unique<AssignmentExprAST>(dest, value);
    } 
    
    if (check(TRIPLE_EQUALS)) {
        ExprPtr value = parseExpr();
        return make_unique<AssignmentExprAST>(dest, value, true);
    }
    
    return std::move(dest);
}

ExprPtr Parser::parseCallExpr() { return parseAdditiveExpr(); } // ! skipping logicalexpr here

ExprPtr Parser::parseLogicalExpr() {
    ExprPtr LHS = parseComparisonExpr();

    while (current->type == AND || current->type == OR) {
        TokenType logical = eat().type;
        ExprPtr RHS = parseComparisonExpr();
        LHS = make_unique<LogicalExprAST>(logical, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseComparisonExpr() {
    ExprPtr LHS = parseExpr();

    while (current->type >= EQUALS_EQUALS && current->type <= GTEQUALS) {
        TokenType comp = eat().type;
        ExprPtr RHS = parseExpr();
        LHS = make_unique<ComparisonExprAST>(comp, LHS, RHS);
    }

    return LHS;
} 

ExprPtr Parser::parseAdditiveExpr() {
    ExprPtr LHS = parseMultiplicativeExpr();

    while (current->type == PLUS || current->type == MINUS) {
        char op = current->value.front(); // get '+' or '-' 
        ++current;
        ExprPtr RHS = parseMultiplicativeExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseMultiplicativeExpr() {
    ExprPtr LHS = parsePowerExpr();

    while (current->type == ASTERISK || current->type == SLASH || current->type == PERCENT) {
        char op = current->value.front(); // get '*', '/', '%' 
        ++current;
        ExprPtr RHS = parsePowerExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parsePowerExpr() { 
    ExprPtr LHS = parseUnaryExpr();

    while (check(CARET)) {
        ExprPtr RHS = parseUnaryExpr();
        LHS = make_unique<BinaryExprAST>('^', LHS, RHS);
    }    

    return LHS;
}

ExprPtr Parser::parseUnaryExpr() { return parsePrimaryExpr(); }

ExprPtr Parser::parsePrimaryExpr() {
    Token that = eat();
    // TODO: unary epxr
    switch (that.type) {
        case NUMBER:        return make_unique<NumberExprAST, _i64>(stoll(that.value));
        case FLOAT:         return make_unique<NumberExprAST, _f64>(stod(that.value));
        case IDENTIFIER:    {
            ExprPtr primary = make_unique<VariableExprAST>(that.value);

            if (!check(DOT)) 
                return primary;
            
            ExprPtr member = parsePrimaryExpr(); 
            return make_unique<MemberExprAST>(primary, member);
        }
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
            return parsePrimaryExpr();
        }
    }
}

FuxType Parser::parseType(_i64 pointerDepth, FuxType::AccessList access) {
    if (!notEOF() || !current->isType()) 
        return FuxType(); // = NO_TYPE; will be checked by analyser
    
    Token typeToken = eat();
    if (check(ARRAY_BRACKET))
        return FuxType::createArray((FuxType::Kind) typeToken.type, pointerDepth, access, typeToken.value);
    else if (check(LBRACKET)) {
        ExprPtr size = parseExpr(); 
        expect(RBRACKET, MISSING_BRACKET);
        return FuxType::createArray((FuxType::Kind) typeToken.type, pointerDepth, access, typeToken.value, size);
    } else
        return FuxType::createStd((FuxType::Kind) typeToken.type, pointerDepth, access, typeToken.value);
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

Token &Parser::peek(size_t steps) { return *(current + steps); }

bool Parser::check(TokenType type) {
    if (!notEOF() || current->type != type) 
        return false;
    
    ++current;
    return true;
}

bool Parser::check(TokenType type, TokenType type0) {
    if (current->type != type || peek().type != type0)
        return false;
    
    current += 2;
    return true;
}

constexpr bool Parser::notEOF() { return current->type != _EOF; }