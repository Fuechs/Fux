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

// FIXME: error reporting: errors resulting out of other errors shouldn't be reported
StmtPtr Parser::parseStmt() {
    StmtPtr stmt = parseFunctionDeclStmt();
    // TODO: get rid of this if
    if (stmt 
    && stmt->getASTType() != AST::CodeBlockAST 
    && stmt->getASTType() != AST::FunctionAST
    && stmt->getASTType() != AST::IfElseAST) // don't throw useless errors
        expect(SEMICOLON);
    return stmt;
}

StmtPtr Parser::parseFunctionDeclStmt() {
    TypePrefix typePrefix = parseTypePrefix();

    if (*current != IDENTIFIER) {
        if (typePrefix.first) {
            error->createError(UNEXPECTED_TOKEN, eat(),
                "expected an identifier after type prefix"); 
            error->addNote(peek(-2), "type prefix found here");
            return nullptr;
        }

        return parseBlockStmt();
    }

    if (peek() != LPAREN) {
        typePrefix.first = true;
        return parseVariableDeclStmt(typePrefix);
    }

    Token &symbolTok = eat(); // for error tracking
    const string &symbol = symbolTok.value;

    expect(LPAREN);
    // TODO: check wether this is a call expression
    StmtList args = StmtList();
    do {
        if (*current == RPAREN)
            break;
        
        StmtPtr arg = parseVariableDeclStmt();

        if (!arg) 
            continue; // error already created by parseVariableDeclStmt
        
        args.push_back(std::move(arg));

        // analyser will check for statements in arguments later
        if (args.back()->isExpr()) {
            if (typePrefix.first) {
                error->createWarning(UNEXPECTED_TOKEN, symbolTok, 
                    "unexpected function call after access modifiers, expected a function declaration instead.");
                error->addNote(peek(-1), "parsed an expression approximately here, making this statement a function call");
                // TODO: exact position
                return nullptr;
            }
            return parseCallExpr(symbol, std::move(args));
        }
    } while (check(COMMA));
    expect(RPAREN, MISSING_BRACKET);

    OptType type = parseTypeSuffix(typePrefix);

    if (!type.first) {
        if (typePrefix.first) {
            error->createError(INCOMPATIBLE_TYPES, eat(),
                "expected a type here since access modifiers and/or a pointer depth were previously parsed");
            return nullptr;
        }        

        // TODO: how to solve this?
        error->createWarning(INCOMPATIBLE_TYPES, *current,
            "automatic typing for functions not supported yet, a function call will be parsed instead");
        error->addNote(*current, "would have expected a COLON ':' or RPOINTER '->' here to make this a function declaration");
        return parseCallExpr(symbol, std::move(args));
    }

    if (*current == SEMICOLON)
        return make_unique<PrototypeAST>(type.second, symbol, args);

    StmtPtr body = parseStmt();
    
    return make_unique<FunctionAST>(type.second, symbol, args, body);
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
        expect(LPAREN);
        ExprPtr condition = parseExpr();
        expect(RPAREN, MISSING_BRACKET);
        StmtPtr thenBody = parseStmt(); 
        if (check(KEY_ELSE)) {
            StmtPtr elseBody = parseStmt(); 
            return make_unique<IfElseAST>(condition, thenBody, elseBody);
        } 
        return make_unique<IfElseAST>(condition, thenBody);
    } 

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
        ExprPtr arg = nullptr;
        if (*current != SEMICOLON)
            arg = parseExpr();
        return make_unique<ReturnCallAST>(arg);
    } else
        return parseVariableDeclStmt();
}

StmtPtr Parser::parseVariableDeclStmt(TypePrefix typePrefix) {
    if (typePrefix.first)
        goto actual;

    typePrefix = parseTypePrefix();

    if (*current != IDENTIFIER) {
        if (!typePrefix.first) // check wether a type prefix was actually parsed
            return parseExpr();
        error->createError(UNEXPECTED_TOKEN, eat(), "expected an identifier after type prefix");
        error->addNote(peek(-1), "type prefix found here");
        return nullptr;
    }
    
    actual:
    const string symbol = eat().value; // get value from identifier
    // FIXME: typePrefix is always empty here
    OptType type = parseTypeSuffix(typePrefix);
    
    if (!type.first) {
        for (int64_t i = 0; i < typePrefix.second.first + 1; i++) // get the '*'s and identifier back
            --current;
        return parseExpr();
    }
    
    if (check(EQUALS)) { // =
        if (!type.second)
            type.second = FuxType(FuxType::AUTO, 0, typePrefix.second.second);
    } else if (check(TRIPLE_EQUALS)) { // ===
        if (!type.second) {
            typePrefix.second.second.push_back(FuxType::CONSTANT);
            type.second = FuxType(FuxType::AUTO, typePrefix.second.first, typePrefix.second.second);
        } else
            type.second.access.push_back(FuxType::CONSTANT);
    } else {
        return make_unique<VariableDeclAST>(symbol, type.second);
    }

    ExprPtr value = parseExpr();
    return make_unique<VariableDeclAST>(symbol, type.second, value);
}

ExprList Parser::parseExprList() { return ExprList(); }

ExprPtr Parser::parseExpr() { return parseAssignmentExpr(); }

ExprPtr Parser::parseAssignmentExpr() { 
    ExprPtr dest = parseTernaryExpr();

    if (current->isAssignment()) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprPtr value = parseExpr();
        return make_unique<BinaryExprAST>(op, dest, value);
    }
    
    return dest;
}

ExprPtr Parser::parseTernaryExpr() { return parseLogicalOrExpr(); }

ExprPtr Parser::parseLogicalOrExpr() { 
    ExprPtr LHS = parseLogicalAndExpr();

    while (check(OR)) {
        ExprPtr RHS = parseExpr();
        LHS = make_unique<BinaryExprAST>(BinaryOp::LOR, LHS, RHS);
    }

    return LHS;
 }

ExprPtr Parser::parseLogicalAndExpr() { 
    ExprPtr LHS = parseBitwiseOrExpr();

    while (check(AND)) {
        ExprPtr RHS = parseExpr();
        LHS = make_unique<BinaryExprAST>(BinaryOp::LAND, LHS, RHS);
    }

    return LHS;
 }

ExprPtr Parser::parseBitwiseOrExpr() {
    ExprPtr LHS = parseBitwiseXorExpr();

    while (check(BIT_OR)) {
        ExprPtr RHS = parseExpr();
        LHS = make_unique<BinaryExprAST>(BinaryOp::BOR, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseBitwiseXorExpr() { return parseBitwiseAndExpr(); }

ExprPtr Parser::parseBitwiseAndExpr() { 
    ExprPtr LHS = parseEqualityExpr();

    while (check(BIT_AND)) {
        ExprPtr RHS = parseExpr();
        LHS = make_unique<BinaryExprAST>(BinaryOp::BAND, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseEqualityExpr() {
    ExprPtr LHS = parseRelationalExpr();

    while (*current == EQUALS_EQUALS || *current == NOT_EQUALS) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprPtr RHS = parseExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseRelationalExpr() {
    ExprPtr LHS = parseBitwiseShiftExpr();

    while (current->isRelational()) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprPtr RHS = parseExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseBitwiseShiftExpr() {
    ExprPtr LHS = parseAdditiveExpr();

    while (*current == BIT_LSHIFT || *current == BIT_RSHIFT) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprPtr RHS = parseExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseAdditiveExpr() {
    ExprPtr LHS = parseMultiplicativeExpr();

    while (*current == PLUS || *current == MINUS) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprPtr RHS = parseExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseMultiplicativeExpr() {
    ExprPtr LHS = parsePowerExpr();

    while (*current == ASTERISK || *current == SLASH || *current == PERCENT) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprPtr RHS = parseExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parsePowerExpr() { 
    ExprPtr LHS = parseAddressExpr();

    while (check(CARET)) {
        ExprPtr RHS = parseExpr();
        LHS = make_unique<BinaryExprAST>(BinaryOp::POW, LHS, RHS);
    }    

    return LHS;
}

ExprPtr Parser::parseAddressExpr() { 
    if (check(BIT_AND)) {
        ExprPtr expr = parseDereferenceExpr();
        return make_unique<UnaryExprAST>(UnaryOp::ADDR, expr);
    }

    return parseDereferenceExpr(); 
}

ExprPtr Parser::parseDereferenceExpr() { 
    if (check(ASTERISK)) {
        ExprPtr expr = parseTypeCastExpr();
        return make_unique<UnaryExprAST>(UnaryOp::DEREF, expr);
    }

    return parseTypeCastExpr(); 
}

ExprPtr Parser::parseTypeCastExpr() { return parseLogBitUnaryExpr(); }

ExprPtr Parser::parseLogBitUnaryExpr() { 
    if (*current == EXCLAMATION || *current == BIT_NOT || *current == QUESTION) {
        UnaryOp op = (UnaryOp) eat().type;
        ExprPtr expr = parsePlusMinusUnaryExpr();
        return make_unique<UnaryExprAST>(op, expr);
    }

    return parsePlusMinusUnaryExpr(); 
}

ExprPtr Parser::parsePlusMinusUnaryExpr() { 
    if (*current == PLUS || *current == MINUS) {
        UnaryOp op = (UnaryOp) eat().type;
        ExprPtr expr = parsePreIncDecExpr();
        return make_unique<UnaryExprAST>(op, expr);
    }
    
    return parsePreIncDecExpr(); 
}

ExprPtr Parser::parsePreIncDecExpr() { 
    if (*current == PLUS_PLUS || *current == MINUS_MINUS) {
        UnaryOp op = eat() == PLUS_PLUS ? UnaryOp::PINC : UnaryOp::PDEC;
        ExprPtr expr = parseIndexExpr();
        return make_unique<UnaryExprAST>(op, expr);
    } 
    
    return parseIndexExpr(); 
}

ExprPtr Parser::parseIndexExpr() { return parseCallExpr(); }

ExprPtr Parser::parseCallExpr(string symbol, StmtList arguments) { 
    if (!symbol.empty() || !arguments.empty())
        goto pre_parsed;

    {if (*current != IDENTIFIER || peek() != LPAREN)
        return parsePostIncDecExpr();

    symbol = eat().value;
    eat(); // (
    StmtList arguments = StmtList();
    do {
        if (*current == RPAREN)
            break;

        ExprPtr arg = parseExpr();
        if (arg)
            arguments.push_back(std::move(arg));
    } while (check(COMMA));
    expect(RPAREN, MISSING_BRACKET);
    return make_unique<CallExprAST>(symbol, arguments);}

    pre_parsed:
    if (peek(-1) != RPAREN) {
        while (check(COMMA)) {
            ExprPtr arg = parseExpr();
            if (arg)
                arguments.push_back(std::move(arg));
        } 
        expect(RPAREN, MISSING_BRACKET);
    }

    return make_unique<CallExprAST>(symbol, arguments);
} 

ExprPtr Parser::parsePostIncDecExpr() {
    ExprPtr expr = parsePrimaryExpr();
    if (*current == PLUS_PLUS || *current == MINUS_MINUS)
        return make_unique<UnaryExprAST>(
            eat() == PLUS_PLUS ? UnaryOp::SINC : UnaryOp::SDEC, expr);
    return expr;
}

ExprPtr Parser::parsePrimaryExpr() {
    Token that = eat();
    switch (that.type) {
        case HEXADECIMAL:
        case NUMBER:        
        case OCTAL:
        case BINARY:        return parseNumberExpr(that);
        case FLOAT:         return make_unique<NumberExprAST, _f64>(stod(that.value));
        case CHAR:          return make_unique<CharExprAST>((_c8) that.value.front()); // TODO: parse c16
        case STRING:        return make_unique<StringExprAST>(that.value); 
        case KEY_TRUE:      return make_unique<BoolExprAST>(true);
        case KEY_FALSE:     return make_unique<BoolExprAST>(false);
        case KEY_NULL:      return make_unique<NullExprAST>();
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

FuxType Parser::parseTypeName(TypePrefix &typePrefix) {
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

    _i64 pointerDepth;
    for (pointerDepth = 0; check(ASTERISK); pointerDepth++);

    bool moved = (access != FuxType::AccessList({FuxType::PUBLIC}) || pointerDepth != 0);

    return TypePrefix(moved, std::pair<_i64, FuxType::AccessList>(pointerDepth, access));
}

Parser::OptType Parser::parseTypeSuffix(TypePrefix &typePrefix) {
    FuxType type = FuxType();
    bool success = true;

    if (check(COLON)) {
        type = parseTypeName(typePrefix);
    } else if (check(RPOINTER)) {
        if (typePrefix.second.first > 0) 
            error->createWarning(INCOMPATIBLE_TYPES, peek(-1), 
                "given pointer depth will be ignored and a reference parsed instead");
        typePrefix.second.first = -1;
        type = parseTypeName(typePrefix);

        if (!type) {
            // don't advance so var decl will be parsed normally
            error->createError(UNEXPECTED_TOKEN, *current, "expected a type after RPOINTER '->'");
            error->addNote(peek(-1), "automatic typing is not supported for references yet");
            // we are not setting success to false here because 
            // the parser should continue parsing the declaration
        }
    } else {
        success = false;
    }
    
    return OptType(success, type);
}

ExprPtr Parser::parseNumberExpr(Token &tok) {
    _u64 value;
    switch (tok.type) {
        case HEXADECIMAL:   value = std::stoull(tok.value, nullptr, 16); break;
        case NUMBER:        value = std::stoull(tok.value); break;
        case OCTAL:         
            tok.value.erase(1, 1); // "0o..." -> "0..."
            value = std::stoull(tok.value, nullptr, 8); 
            break;
        case BINARY:        
            tok.value.erase(0, 2); // "0b..." -> "..."
            value = std::stoull(tok.value, nullptr, 2); 
            break;
        default:            break; // unreachable
    }
    size_t bits = (size_t) log2(value) + 1;
    if      (bits <= 8)     return make_unique<NumberExprAST, _u8>(value);
    else if (bits <= 16)    return make_unique<NumberExprAST, _u16>(value);
    else if (bits <= 32)    return make_unique<NumberExprAST, _u32>(value);
    else if (bits <= 64)    return make_unique<NumberExprAST>(value);
    else                    return nullptr; // unreachable   
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