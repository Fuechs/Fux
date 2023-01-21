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

Parser::Parser(ErrorManager *error, const string &fileName, const string &source, const bool mainFile) 
: error(error), mainFile(mainFile) {
    lexer = new Lexer(source, fileName, error);
    if (mainFile)
        fux.options.fileLines = lexer->getLines();
    root = make_unique<RootAST>();
}

Parser::~Parser() {
    delete lexer;
    tokens.clear();
}

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
    StmtPtr stmt = parseFunctionDeclStmt();
    if (stmt && stmt->getASTType() != AST::CodeBlockAST) // don't throw useless errors
        expect(SEMICOLON);
    return stmt;
}

StmtPtr Parser::parseFunctionDeclStmt() {
    TypePrefix typePrefix = parseTypePrefix();

    if (check(IDENTIFIER) && *current != LPAREN) {
        typePrefix.first = true;
        return parseVariableDeclStmt(typePrefix);
    }
    
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

StmtPtr Parser::parseVariableDeclStmt(TypePrefix typePrefix) {
    if (typePrefix.first)
        goto actual;

    typePrefix = parseTypePrefix();

    if (!check(IDENTIFIER)) {
        if (!typePrefix.first) // check wether a type prefix was actually parsed
            return parseExpr();

        error->createError(UNEXPECTED_TOKEN, eat(), "expected an identifier after type prefix");
        error->addNote(peek(-1), "type prefix found here");
        return nullptr;
    }

    actual:
    const string symbol = peek(-1).value; // get value from identifier
    FuxType type = FuxType();
    
    if (check(COLON))
        type = parseType(typePrefix);
    else if (check(RPOINTER)) { // reference
        if (typePrefix.second.first > 0)
            error->createWarning(INCOMPATIBLE_TYPES, peek(-1), 
                "given pointer depth will be ignored and a reference parsed instead");
        typePrefix.second.first = -1;
        type = parseType(typePrefix);
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
            type = FuxType(FuxType::AUTO, 0, typePrefix.second.second);
    } else if (check(TRIPLE_EQUALS)) { // ===
        if (!type) {
            typePrefix.second.second.push_back(FuxType::CONSTANT);
            type = FuxType(FuxType::AUTO, typePrefix.second.first, typePrefix.second.second);
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

ExprPtr Parser::parseExprList() { return nullptr; }

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

ExprPtr Parser::parseCallExpr() { return parseLogicalExpr(); } 

ExprPtr Parser::parseLogicalExpr() { return parseComparisonExpr(); }

ExprPtr Parser::parseComparisonExpr() { return parseAdditiveExpr(); }

ExprPtr Parser::parseAdditiveExpr() {
    ExprPtr LHS = parseMultiplicativeExpr();

    while (current->type == PLUS || current->type == MINUS) {
        char op = eat().value.front(); // get '+' or '-' 
        ExprPtr RHS = parseExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseMultiplicativeExpr() {
    ExprPtr LHS = parsePowerExpr();

    while (*current == ASTERISK || *current == SLASH || *current == PERCENT) {
        char op = eat().value.front(); // get '*', '/', '%' 
        ExprPtr RHS = parseExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parsePowerExpr() { 
    ExprPtr LHS = parseUnaryExpr();

    while (check(CARET)) {
        ExprPtr RHS = parseExpr();
        LHS = make_unique<BinaryExprAST>('^', LHS, RHS);
    }    

    return LHS;
}

ExprPtr Parser::parseUnaryExpr() { return parsePrimaryExpr(); }

ExprPtr Parser::parsePrimaryExpr() {
    Token that = eat();
    switch (that.type) {
        case NUMBER:        return make_unique<NumberExprAST, _i64>(stoll(that.value));
        case FLOAT:         return make_unique<NumberExprAST, _f64>(stod(that.value));
        case STRING:        return make_unique<StringExprAST>(that.value); 
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
        case _EOF: return nullptr;
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

FuxType Parser::parseType(TypePrefix typePrefix) {
    if (!current->isType()) 
        return FuxType(); // = NO_TYPE; will be checked by analyser
    
    Token typeToken = eat();
    if (check(ARRAY_BRACKET))
        return FuxType::createArray((FuxType::Kind) typeToken.type, typePrefix.second.first, typePrefix.second.second, typeToken.value);
    else if (check(LBRACKET)) {
        ExprPtr size = parseExpr(); 
        expect(RBRACKET, MISSING_BRACKET);
        // FIXME: array size turns into a nullptr, probably in FuxType::operator=()
        return FuxType::createArray((FuxType::Kind) typeToken.type, typePrefix.second.first, typePrefix.second.second, typeToken.value, size);
    } else
        return FuxType::createStd((FuxType::Kind) typeToken.type, typePrefix.second.first, typePrefix.second.second, typeToken.value);
}

Parser::TypePrefix Parser::parseTypePrefix() {

    FuxType::AccessList access = {FuxType::PUBLIC};

    while (current->isModifier())
        access.push_back((FuxType::Access) eat().type);

    _i64 pointerDepth = 0;
    while (check(ASTERISK))
        ++pointerDepth;

    bool moved = (access != FuxType::AccessList({FuxType::PUBLIC}) || pointerDepth != 0);

    return TypePrefix(moved, {pointerDepth, access});
}

Token &Parser::eat() {
    if (*current == _EOF)
        return *current;
    return *current++;
}

Token Parser::expect(TokenType type, ErrorType errType) {
    Token curTok = eat();
    if (curTok != type) {
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
    if (*current != type) 
        return false;

    ++current;
    return true;
}

bool Parser::check(TokenType type, TokenType type0) {
    if (*current != type || peek() != type0)
        return false;
    
    current += 2;
    return true;
}

constexpr bool Parser::notEOF() { return *current != _EOF; }