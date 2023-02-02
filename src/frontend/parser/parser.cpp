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
StmtPtr Parser::parseStmt(bool expectSemicolon) {
    if (*current == SEMICOLON) { // handle while (...); or for (;;);
        if (expectSemicolon)
            eat();
        return nullptr;
    }

    StmtPtr stmt = parseFunctionDeclStmt();
    // TODO: get rid of this if
    if (expectSemicolon && stmt 
    && stmt->getASTType() != AST::CodeBlockAST 
    && stmt->getASTType() != AST::FunctionAST
    && stmt->getASTType() != AST::IfElseAST
    && stmt->getASTType() != AST::WhileLoopAST
    && stmt->getASTType() != AST::ForLoopAST) // don't throw useless errors
        eat(SEMICOLON);
    return stmt;
}

StmtPtr Parser::parseFunctionDeclStmt() {
    if (*current != IDENTIFIER) 
        return parseForLoopStmt();

    TokenIter backToken = current;
    ExprPtr symbolExpr = parsePrimaryExpr(); 

    if (!check(LPAREN)) {
        current = backToken;
        return parseForLoopStmt();
    }
    
    TokenIter paramBegin = current;
    for (size_t depth = 1;;) {
        if      (check(LPAREN)) ++depth;
        else if (check(RPAREN)) --depth;
        else if (check(_EOF))   {
            error->createError(MISSING_BRACKET, peek(-1), "expected closing paren");
            error->addNote(*paramBegin, "opening paren found here");
            break;
        } else                  eat();

        if (depth == 0)
            break;
    } // skip ( ... )

    if (*current != COLON && *current != RPOINTER) {
        current = backToken;
        return parseCallExpr();
    } else
        current = paramBegin;

    StmtList args = StmtList();
    
    do {
        if (*current == RPAREN)
            break;
        StmtPtr arg = parseVariableDeclStmt();
        if (arg) // error already created by parseVariableDeclStmt
            args.push_back(std::move(arg)); 
    } while (check(COMMA));
    eat(RPAREN, MISSING_BRACKET);

    FuxType type = parseType();

    if (*current == SEMICOLON)
        return make_unique<PrototypeAST>(type, symbolExpr, args);

    StmtPtr body = parseStmt();    
    return make_unique<FunctionAST>(type, symbolExpr, args, body);
}

StmtPtr Parser::parseForLoopStmt() { 
    bool forEach = false;
    StmtPtr init = nullptr;
    ExprPtr cond = nullptr;
    ExprPtr iter = nullptr;

    if (!check(KEY_FOR))
        return parseWhileLoopStmt();

    eat(LPAREN);

    init = parseStmt(false);

    if (check(KEY_IN)) {
        forEach = true;
        iter = parseExpr();
    } else {
        eat(SEMICOLON); 
        if (!check(SEMICOLON)) {
            cond = parseExpr();
            eat(SEMICOLON);
        }
        if (*current != RPAREN)
            iter = parseExpr();
    }

    eat(RPAREN, MISSING_BRACKET);
    StmtPtr body = parseStmt();
    if (forEach)    return make_unique<ForLoopAST>(init, iter, body);
    else            return make_unique<ForLoopAST>(init, cond, iter, body);
}

StmtPtr Parser::parseWhileLoopStmt() {
    bool postCondition = false;
    ExprPtr condition = nullptr;
    StmtPtr body = nullptr;

    if (check(KEY_DO)) {
        postCondition = true;
        body = parseStmt();
        eat(KEY_WHILE);
        eat(LPAREN);
        condition = parseExpr();
        eat(RPAREN, MISSING_BRACKET);
    } else if (check(KEY_WHILE)) {
        eat(LPAREN);
        condition = parseExpr();
        eat(RPAREN, MISSING_BRACKET);
        body = parseStmt();
    } else
        return parseBlockStmt();

    return make_unique<WhileLoopAST>(condition, body, postCondition);
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
        eat(LPAREN);
        ExprPtr condition = parseExpr();
        eat(RPAREN, MISSING_BRACKET);
        StmtPtr thenBody = parseStmt(); 
        if (check(KEY_ELSE)) {
            StmtPtr elseBody = parseStmt(); 
            return make_unique<IfElseAST>(condition, thenBody, elseBody);
        } 
        return make_unique<IfElseAST>(condition, thenBody);
    } 

    return parseInbuiltCallStmt();
}

StmtPtr Parser::parseInbuiltCallStmt() {
    if (current->isInbuiltCall()) {
        Inbuilts callee = (Inbuilts) eat().type;
        ExprList args = parseExprList();
        return make_unique<InbuiltCallAST>(callee, args);
    }

    return parseVariableDeclStmt();
}

StmtPtr Parser::parseVariableDeclStmt() {
    if (*current != IDENTIFIER || (peek() != COLON && peek() != RPOINTER))
        return parseExpr();
    
    const string symbol = eat().value; // get value from identifier
    FuxType type = parseType();

    if (check(TRIPLE_EQUALS)) // ===
        type.access.push_back(FuxType::CONSTANT);
    else if (!check(EQUALS))
        return make_unique<VariableDeclAST>(symbol, type);

    ExprPtr value = parseExpr();
    return  make_unique<VariableDeclAST>(symbol, type, value);
}

ExprList Parser::parseExprList() { 
    ExprList list = ExprList();
    ExprPtr expr = parseExpr();
    while (check(COMMA)) {
        list.push_back(std::move(expr));
        expr = parseExpr();
    }
    list.push_back(std::move(expr));
    return list;
}
 

ExprPtr Parser::parseExpr() { return parseAssignmentExpr(); }

ExprPtr Parser::parseAssignmentExpr() { 
    ExprPtr dest = parsePipeExpr();

    if (current->isAssignment()) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprPtr value = parsePipeExpr();
        dest = make_unique<BinaryExprAST>(op, dest, value);
    }
    
    return dest;
}

ExprPtr Parser::parsePipeExpr() {
    ExprPtr LHS = parseTernaryExpr();

    while (*current == LSHIFT || *current == RSHIFT) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprPtr RHS = parseTernaryExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseTernaryExpr() { 
    ExprPtr condition = parseLogicalOrExpr();

    while (check(QUESTION)) {
        ExprPtr thenExpr = parseLogicalOrExpr();
        eat(COLON);
        ExprPtr elseExpr = parseLogicalOrExpr();
        condition = make_unique<TernaryExprAST>(condition, thenExpr, elseExpr);
    }

    return condition; 
}

ExprPtr Parser::parseLogicalOrExpr() { 
    ExprPtr LHS = parseLogicalAndExpr();

    while (check(OR)) {
        ExprPtr RHS = parseLogicalAndExpr();
        LHS = make_unique<BinaryExprAST>(BinaryOp::LOR, LHS, RHS);
    }

    return LHS;
 }

ExprPtr Parser::parseLogicalAndExpr() { 
    ExprPtr LHS = parseBitwiseOrExpr();

    while (check(AND)) {
        ExprPtr RHS = parseBitwiseOrExpr();
        LHS = make_unique<BinaryExprAST>(BinaryOp::LAND, LHS, RHS);
    }

    return LHS;
 }

ExprPtr Parser::parseBitwiseOrExpr() {
    ExprPtr LHS = parseBitwiseXorExpr();

    while (check(BIT_OR)) {
        ExprPtr RHS = parseBitwiseXorExpr();
        LHS = make_unique<BinaryExprAST>(BinaryOp::BOR, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseBitwiseXorExpr() { 
    ExprPtr LHS = parseBitwiseAndExpr(); 

    while (check(BIT_XOR)) {
        ExprPtr RHS = parseBitwiseAndExpr();
        LHS = make_unique<BinaryExprAST>(BinaryOp::BXOR, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseBitwiseAndExpr() { 
    ExprPtr LHS = parseEqualityExpr();

    while (check(BIT_AND)) {
        ExprPtr RHS = parseEqualityExpr();
        LHS = make_unique<BinaryExprAST>(BinaryOp::BAND, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseEqualityExpr() {
    ExprPtr LHS = parseRelationalExpr();

    while (*current == EQUALS_EQUALS || *current == NOT_EQUALS) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprPtr RHS = parseRelationalExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseRelationalExpr() {
    ExprPtr LHS = parseBitwiseShiftExpr();

    while (current->isRelational()) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprPtr RHS = parseBitwiseShiftExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseBitwiseShiftExpr() {
    ExprPtr LHS = parseAdditiveExpr();

    while (*current == BIT_LSHIFT || *current == BIT_RSHIFT) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprPtr RHS = parseAdditiveExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseAdditiveExpr() {
    ExprPtr LHS = parseMultiplicativeExpr();

    while (*current == PLUS || *current == MINUS) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprPtr RHS = parseMultiplicativeExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseMultiplicativeExpr() {
    ExprPtr LHS = parsePowerExpr();

    while (*current == ASTERISK || *current == SLASH || *current == PERCENT) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprPtr RHS = parsePowerExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parsePowerExpr() { 
    ExprPtr LHS = parseAddressExpr();

    while (check(CARET)) {
        ExprPtr RHS = parseAddressExpr();
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

ExprPtr Parser::parseTypeCastExpr() { 
    TokenIter backToken = current;
    if (check(LPAREN)) {
        FuxType type = parseType(true); // analyser will check wether type is primitive or not
        if (!type || *current != RPAREN) { // TODO: test more possible cases
            current = backToken; // get '*'s, identifier and '(' back
            return parseLogBitUnaryExpr();
        }
        eat(RPAREN, MISSING_BRACKET);
        ExprPtr expr = parseExpr();
        return make_unique<TypeCastExprAST>(type, expr);
    }
   
    return parseLogBitUnaryExpr(); 
}

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

ExprPtr Parser::parseIndexExpr() { 
    ExprPtr expr = parseCallExpr(); 
    
    if (check(ARRAY_BRACKET)) 
        return make_unique<BinaryExprAST>(BinaryOp::IDX, expr);
    else if (check(LBRACKET)) {
        ExprPtr index = parseExpr();
        eat(RBRACKET, MISSING_BRACKET);
        return make_unique<BinaryExprAST>(BinaryOp::IDX, expr, index);
    }

    return expr;
}

ExprPtr Parser::parseCallExpr() { 
    if (*current != IDENTIFIER)
        return parsePostIncDecExpr();

    TokenIter backTok = current;
    ExprPtr symbol = parsePrimaryExpr();
        
    if (!check(LPAREN)) {
        current = backTok;
        return parsePostIncDecExpr();
    }

    ExprList arguments = ExprList();
    if (*current != RPAREN)
        arguments = parseExprList();
    eat(RPAREN, MISSING_BRACKET);
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
        case CHAR:          return parseCharExpr(that);
        case STRING:        return make_unique<StringExprAST>(escapeSequences(that.value)); 
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
            eat(RPAREN, MISSING_BRACKET);
            return expr;
        }
        case _EOF: {
            error->createError(UNEXPECTED_EOF, that, "did not expect end of file here.");
            return nullptr;
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

FuxType Parser::parseType(bool primitive) {
    if (primitive) {
        int64_t pointerDepth = 0;
        while(check(ASTERISK)) 
            ++pointerDepth;
        if (!current->isType()) 
            return FuxType(FuxType::NO_TYPE, pointerDepth);
        const FuxType::Kind kind = (FuxType::Kind) current->type;
        const string &value = eat().value;
        return FuxType::createPrimitive(kind, pointerDepth, check(ARRAY_BRACKET), value);
    }

    FuxType::AccessList access = {FuxType::PUBLIC};
    int64_t pointerDepth;

    Token &typeDenotion = eat(); // ':' or '->' for error tracking
    switch (typeDenotion.type) {
        case COLON:     pointerDepth = 0; break;
        case RPOINTER:  pointerDepth = -1; break;
        default:        
            error->createError(UNEXPECTED_TOKEN, typeDenotion,
                "expected a COLON ':' or RPOINTER '->' here");
            return FuxType();
    }

    while (current->isModifier())
        access.push_back((FuxType::Access) eat().type);
    
    while (check(ASTERISK)) {
        if (pointerDepth != -1) {
            ++pointerDepth;
            break;
        }

        while(check(ASTERISK)); // skip all '*'
        error->createWarning(INCOMPATIBLE_TYPES, peek(-1),
            "given pointer depth wil be ignored and a reference parsed instead");
        error->addNote(typeDenotion, "the reference got declared here");
    }

    if (!current->isType()) {
        // if (typeDenotion == RPOINTER) {
        //     error->createError(UNEXPECTED_TOKEN, eat(), "expected a type after RPOINTER '->'");
        //     error->addNote(typeDenotion, "automatic typing is not supported for references yet");
        //     return FuxType(); 
        // } 

        if (pointerDepth > 0)
            error->createWarning(INCOMPATIBLE_TYPES, peek(-1), 
                "given pointer depth will be ignored for automatic type");
        return FuxType::createStd(FuxType::AUTO, 0, access);
    }

    const FuxType::Kind kind = (FuxType::Kind) current->type;
    const string &value = eat().value;

    if (check(ARRAY_BRACKET)) 
        return FuxType::createArray(kind, pointerDepth, access, value);
    else if (check(LBRACKET)) {
        ExprPtr size = parseExpr();
        ExprPtr &sizeRef = root->addSizeExpr(size);
        eat(RBRACKET, MISSING_BRACKET);
        return FuxType::createArray(kind, pointerDepth, access, value, sizeRef);
    } else 
        return FuxType::createStd(kind, pointerDepth, access, value);

    assert(false && "unreachable");
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
    size_t bits = (size_t) log2(value) + 1; // get amount of bits in (binary) value
    if      (bits <= 8)     return make_unique<NumberExprAST, _u8>(value);
    else if (bits <= 16)    return make_unique<NumberExprAST, _u16>(value);
    else if (bits <= 32)    return make_unique<NumberExprAST, _u32>(value);
    else if (bits <= 64)    return make_unique<NumberExprAST>(value);
    else                    return nullptr; // unreachable   
}

ExprPtr Parser::parseCharExpr(Token &tok) {
    // TODO: add support for c16
    _c8 value;
    value = escapeSequences(tok.value).front();
    return make_unique<CharExprAST>(value);
}

Token &Parser::eat() {
    if (*current == _EOF)
        return *current;
    return *current++;
}

Token &Parser::eat(TokenType type, ErrorType errType) {
    Token curTok = eat();

    if (curTok != type) {
        stringstream err;
        err 
            << "expected " << TokenTypeString[type] 
            << " '" << TokenTypeValue[type] << "', "
            << "got " << TokenTypeString[curTok.type] 
            << " '" << curTok.value << "' instead";
        error->createError(errType, curTok, err.str());
    }

    return *(current - 1); // avoid Wreturn-stack-address
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