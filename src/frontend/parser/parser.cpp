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

RootAST::Ptr Parser::parse() {
    // lexing
    tokens = lexer->lex();
    lexer->debugPrint();
    
    current = tokens.begin();

    StmtAST::Ptr branch;
    while (notEOF()) 
        if ((branch = parseStmt())) // check for nullptr in case of error
            root->addSub(branch);
    
    return std::move(root);
}

StmtAST::Ptr Parser::parseStmt(bool expectSemicolon) {
    if (*current == SEMICOLON) { // handle while (...); or for (;;);
        if (expectSemicolon)
            eat();
        return nullptr;
    }

    StmtAST::Ptr stmt = parseFunctionDeclStmt();
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

StmtAST::Ptr Parser::parseFunctionDeclStmt() {
    if (*current != IDENTIFIER) 
        return parseForLoopStmt();

    Token::Iter backToken = current;
    string symbol = eat().value;

    if (!check(LPAREN)) {
        current = backToken;
        return parseForLoopStmt();
    }
    
    Token::Iter paramBegin = current;
    for (size_t depth = 1;;) {
        if      (check(LPAREN)) ++depth;
        else if (check(RPAREN)) --depth;
        else if (check(_EOF))   {
            error->createError(MISSING_BRACKET, peek(-1), "expected closing paren");
            error->addNote(*paramBegin, "opening paren found here");
            recover();
            break;
        } else                  eat();

        if (depth == 0)
            break;
    } // skip ( ... )

    if (*current != COLON && *current != POINTER) {
        current = backToken;
        return parseExpr(); // We have to call parseExpr() here to handle situations like this one:
                            // someCall() << someArgument;
    } else
        current = paramBegin;

    StmtAST::Vec args = StmtAST::Vec();
    
    do {
        if (*current == RPAREN)
            break;
        StmtAST::Ptr arg = parseVariableDeclStmt();
        if (arg) // error already created by parseVariableDeclStmt
            args.push_back(std::move(arg)); 
    } while (check(COMMA));
    eat(RPAREN, MISSING_BRACKET);

    FuxType type = parseType();

    if (*current == SEMICOLON)
        return make_unique<PrototypeAST>(type, symbol, args);

    StmtAST::Ptr body = parseStmt();    
    return make_unique<FunctionAST>(type, symbol, args, body);
}

StmtAST::Ptr Parser::parseForLoopStmt() { 
    bool forEach = false;
    StmtAST::Ptr init = nullptr;
    ExprAST::Ptr cond = nullptr;
    ExprAST::Ptr iter = nullptr;

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
    StmtAST::Ptr body = parseStmt();
    if (forEach)    return make_unique<ForLoopAST>(init, iter, body);
    else            return make_unique<ForLoopAST>(init, cond, iter, body);
}

StmtAST::Ptr Parser::parseWhileLoopStmt() {
    bool postCondition = false;
    ExprAST::Ptr condition = nullptr;
    StmtAST::Ptr body = nullptr;

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

StmtAST::Ptr Parser::parseBlockStmt() {
    if (check(LBRACE)) {
        StmtAST::Vec body;
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

StmtAST::Ptr Parser::parseIfElseStmt() {
    if (check(KEY_IF)) {
        eat(LPAREN);
        ExprAST::Ptr condition = parseExpr();
        eat(RPAREN, MISSING_BRACKET);
        StmtAST::Ptr thenBody = parseStmt(); 
        if (check(KEY_ELSE)) {
            StmtAST::Ptr elseBody = parseStmt(); 
            return make_unique<IfElseAST>(condition, thenBody, elseBody);
        } 
        return make_unique<IfElseAST>(condition, thenBody);
    } 

    return parseInbuiltCallStmt();
}

StmtAST::Ptr Parser::parseInbuiltCallStmt() {
    if (current->isInbuiltCall()) {
        Inbuilts callee = (Inbuilts) eat().type;
        ExprAST::Vec args = parseExprList(SEMICOLON);
        return make_unique<InbuiltCallAST>(callee, args);
    }

    return parseVariableDeclStmt();
}

StmtAST::Ptr Parser::parseVariableDeclStmt() {
    if (*current != IDENTIFIER || (peek() != COLON && peek() != POINTER))
        return parseExpr();
    
    const string symbol = eat().value; // get value from identifier
    FuxType type = parseType();

    if (check(TRIPLE_EQUALS)) // ===
        type.access.push_back(FuxType::CONSTANT);
    else if (!check(EQUALS))
        return make_unique<VariableDeclAST>(symbol, type);

    ExprAST::Ptr value = parseExpr();
    return  make_unique<VariableDeclAST>(symbol, type, value);
}

ExprAST::Vec Parser::parseExprList(TokenType end) { 
    ExprAST::Vec list = ExprAST::Vec();
    ExprAST::Ptr expr = parseExpr();
    while (check(COMMA)) {
        if (*current == end)
            break;
        list.push_back(std::move(expr));
        expr = parseExpr();
    }
    list.push_back(std::move(expr));
    return list;
}
 

ExprAST::Ptr Parser::parseExpr() { return parseAssignmentExpr(); }

ExprAST::Ptr Parser::parseAssignmentExpr() { 
    ExprAST::Ptr dest = parsePipeExpr();

    if (current->isAssignment()) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprAST::Ptr value = parsePipeExpr();
        dest = make_unique<BinaryExprAST>(op, dest, value);
    }
    
    return dest;
}

ExprAST::Ptr Parser::parsePipeExpr() {
    ExprAST::Ptr LHS = parseTernaryExpr();

    while (*current == LSHIFT || *current == RSHIFT) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprAST::Ptr RHS = parseTernaryExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprAST::Ptr Parser::parseTernaryExpr() { 
    ExprAST::Ptr condition = parseLogicalOrExpr();

    while (check(QUESTION)) {
        ExprAST::Ptr thenExpr = parseLogicalOrExpr();
        eat(COLON);
        ExprAST::Ptr elseExpr = parseLogicalOrExpr();
        condition = make_unique<TernaryExprAST>(condition, thenExpr, elseExpr);
    }

    return condition; 
}

ExprAST::Ptr Parser::parseLogicalOrExpr() { 
    ExprAST::Ptr LHS = parseLogicalAndExpr();

    while (check(OR)) {
        ExprAST::Ptr RHS = parseLogicalAndExpr();
        LHS = make_unique<BinaryExprAST>(BinaryOp::LOR, LHS, RHS);
    }

    return LHS;
 }

ExprAST::Ptr Parser::parseLogicalAndExpr() { 
    ExprAST::Ptr LHS = parseBitwiseOrExpr();

    while (check(AND)) {
        ExprAST::Ptr RHS = parseBitwiseOrExpr();
        LHS = make_unique<BinaryExprAST>(BinaryOp::LAND, LHS, RHS);
    }

    return LHS;
 }

ExprAST::Ptr Parser::parseBitwiseOrExpr() {
    ExprAST::Ptr LHS = parseBitwiseXorExpr();

    while (check(BIT_OR)) {
        ExprAST::Ptr RHS = parseBitwiseXorExpr();
        LHS = make_unique<BinaryExprAST>(BinaryOp::BOR, LHS, RHS);
    }

    return LHS;
}

ExprAST::Ptr Parser::parseBitwiseXorExpr() { 
    ExprAST::Ptr LHS = parseBitwiseAndExpr(); 

    while (check(BIT_XOR)) {
        ExprAST::Ptr RHS = parseBitwiseAndExpr();
        LHS = make_unique<BinaryExprAST>(BinaryOp::BXOR, LHS, RHS);
    }

    return LHS;
}

ExprAST::Ptr Parser::parseBitwiseAndExpr() { 
    ExprAST::Ptr LHS = parseEqualityExpr();

    while (check(BIT_AND)) {
        ExprAST::Ptr RHS = parseEqualityExpr();
        LHS = make_unique<BinaryExprAST>(BinaryOp::BAND, LHS, RHS);
    }

    return LHS;
}

ExprAST::Ptr Parser::parseEqualityExpr() {
    ExprAST::Ptr LHS = parseRelationalExpr();

    while (*current == EQUALS_EQUALS || *current == NOT_EQUALS) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprAST::Ptr RHS = parseRelationalExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprAST::Ptr Parser::parseRelationalExpr() {
    ExprAST::Ptr LHS = parseBitwiseShiftExpr();

    while (current->isRelational()) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprAST::Ptr RHS = parseBitwiseShiftExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprAST::Ptr Parser::parseBitwiseShiftExpr() {
    ExprAST::Ptr LHS = parseAdditiveExpr();

    while (*current == BIT_LSHIFT || *current == BIT_RSHIFT) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprAST::Ptr RHS = parseAdditiveExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprAST::Ptr Parser::parseAdditiveExpr() {
    ExprAST::Ptr LHS = parseMultiplicativeExpr();

    while (*current == PLUS || *current == MINUS) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprAST::Ptr RHS = parseMultiplicativeExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprAST::Ptr Parser::parseMultiplicativeExpr() {
    ExprAST::Ptr LHS = parsePowerExpr();

    while (*current == ASTERISK || *current == SLASH || *current == PERCENT) {
        BinaryOp op = (BinaryOp) eat().type;
        ExprAST::Ptr RHS = parsePowerExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprAST::Ptr Parser::parsePowerExpr() { 
    ExprAST::Ptr LHS = parseAddressExpr();

    while (check(CARET)) {
        ExprAST::Ptr RHS = parseAddressExpr();
        LHS = make_unique<BinaryExprAST>(BinaryOp::POW, LHS, RHS);
    }    

    return LHS;
}

ExprAST::Ptr Parser::parseAddressExpr() { 
    if (check(BIT_AND)) {
        ExprAST::Ptr expr = parseDereferenceExpr();
        return make_unique<UnaryExprAST>(UnaryOp::ADDR, expr);
    }

    return parseDereferenceExpr(); 
}

ExprAST::Ptr Parser::parseDereferenceExpr() { 
    if (check(ASTERISK)) {
        ExprAST::Ptr expr = parseTypeCastExpr();
        return make_unique<UnaryExprAST>(UnaryOp::DEREF, expr);
    }

    return parseTypeCastExpr(); 
}

ExprAST::Ptr Parser::parseTypeCastExpr() { 
    Token::Iter backToken = current;
    if (check(LPAREN)) {
        FuxType type = parseType(true); // analyser will check wether type is primitive or not
        if (!type || *current != RPAREN) { // TODO: test more possible cases
            current = backToken; // get '*'s, identifier and '(' back
            return parseLogBitUnaryExpr();
        }
        eat(RPAREN, MISSING_BRACKET);
        ExprAST::Ptr expr = parseExpr();
        return make_unique<TypeCastExprAST>(type, expr);
    }
   
    return parseLogBitUnaryExpr(); 
}

ExprAST::Ptr Parser::parseLogBitUnaryExpr() { 
    if (*current == EXCLAMATION || *current == BIT_NOT || *current == QUESTION) {
        UnaryOp op = (UnaryOp) eat().type;
        ExprAST::Ptr expr = parsePlusMinusUnaryExpr();
        return make_unique<UnaryExprAST>(op, expr);
    }

    return parsePlusMinusUnaryExpr(); 
}

ExprAST::Ptr Parser::parsePlusMinusUnaryExpr() { 
    if (*current == PLUS || *current == MINUS) {
        UnaryOp op = (UnaryOp) eat().type;
        ExprAST::Ptr expr = parsePreIncDecExpr();
        return make_unique<UnaryExprAST>(op, expr);
    }
    
    return parsePreIncDecExpr(); 
}

ExprAST::Ptr Parser::parsePreIncDecExpr() { 
    if (*current == PLUS_PLUS || *current == MINUS_MINUS) {
        UnaryOp op = eat() == PLUS_PLUS ? UnaryOp::PINC : UnaryOp::PDEC;
        ExprAST::Ptr expr = parseIndexExpr();
        return make_unique<UnaryExprAST>(op, expr);
    } 
    
    return parseIndexExpr(); 
}

ExprAST::Ptr Parser::parseIndexExpr() { 
    ExprAST::Ptr expr = parseCallExpr(); 
    
    if (check(ARRAY_BRACKET)) 
        return make_unique<BinaryExprAST>(BinaryOp::IDX, expr);
    else if (check(LBRACKET)) {
        ExprAST::Ptr index = parseExpr();
        eat(RBRACKET, MISSING_BRACKET);
        return make_unique<BinaryExprAST>(BinaryOp::IDX, expr, index);
    }

    return expr;
}

ExprAST::Ptr Parser::parseCallExpr() { 
    if (*current != IDENTIFIER && *current != KEY_ASYNC)
        return parsePostIncDecExpr();

    bool asyncCall = false;
    Token::Iter backTok = current;
    
    if (check(KEY_ASYNC))
        asyncCall = true;
    ExprAST::Ptr symbol = parsePrimaryExpr();
        
    if (!check(LPAREN)) {
        current = backTok;
        return parsePostIncDecExpr();
    }

    ExprAST::Vec arguments = ExprAST::Vec();
    if (*current != RPAREN)
        arguments = parseExprList(RPAREN);
    eat(RPAREN, MISSING_BRACKET);
    return make_unique<CallExprAST>(symbol, arguments, asyncCall);
} 

ExprAST::Ptr Parser::parsePostIncDecExpr() {
    ExprAST::Ptr expr = parsePrimaryExpr();
    if (*current == PLUS_PLUS || *current == MINUS_MINUS)
        return make_unique<UnaryExprAST>(
            eat() == PLUS_PLUS ? UnaryOp::SINC : UnaryOp::SDEC, expr);
    return expr;
}

ExprAST::Ptr Parser::parsePrimaryExpr() {
    Token that = eat();

    if (that.isKeyword() && that != IDENTIFIER)
        return make_unique<VariableExprAST>(that.value);

    switch (that.type) {
        case HEXADECIMAL:
        case NUMBER:        
        case OCTAL:
        case BINARY: {
            ExprAST::Ptr beginExpr = parseNumberExpr(that);
            if (check(TRIPLE_DOT)) {
                ExprAST::Ptr endExpr = nullptr;
                Token endTok = eat();
                if (endTok.type == HEXADECIMAL 
                || endTok.type == NUMBER 
                || endTok.type == OCTAL
                || endTok.type == BINARY)
                    endExpr = parseNumberExpr(endTok);
                else {
                    error->createError(GENERIC, endTok,
                        "incomplete range expressions, would have expected a number after '...'");
                    error->addNote(that, "range expression starts here");
                }   

                return make_unique<RangeExprAST>(beginExpr, endExpr);
            }
            return beginExpr;
        }
        case FLOAT:         return make_unique<NumberExprAST, _f64>(stod(that.value));
        case CHAR:          return parseCharExpr(that);
        case STRING:        return make_unique<StringExprAST>(escapeSequences(that.value)); 
        case KEY_TRUE:      return make_unique<BoolExprAST>(true);
        case KEY_FALSE:     return make_unique<BoolExprAST>(false);
        case KEY_NULL:      return make_unique<NullExprAST>();
        case IDENTIFIER:    {
            ExprAST::Ptr primary = make_unique<VariableExprAST>(that.value);

            if (!check(DOT)) 
                return primary;
            
            ExprAST::Ptr member = parsePrimaryExpr(); 
            return make_unique<MemberExprAST>(primary, member);
        }
        case LPAREN: {
            ExprAST::Ptr expr = parseExpr();
            eat(RPAREN, MISSING_BRACKET);
            return expr;
        }
        case LBRACE: {
            ExprAST::Vec elements = parseExprList(RBRACE);
            eat(RBRACE, MISSING_BRACKET);
            return make_unique<ArrayExprAST>(elements);
        }
        case _EOF: {
            error->createError(UNEXPECTED_EOF, that, "did not expect end of file here", true);
            return nullptr;
        }
        default: {        
            stringstream message;
            message 
                << "unexpected token " << TokenTypeString[that.type]
                << " '" << that.value << "' while parsing primary expression";
            error->createError(UNEXPECTED_TOKEN, that, message.str());
            recover();
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
        case POINTER:  pointerDepth = -1; break;
        default:        
            error->createError(UNEXPECTED_TOKEN, typeDenotion,
                "expected a COLON ':' or POINTER '->' here");
            return FuxType();
    }

    while (current->isModifier())
        access.push_back((FuxType::Access) eat().type);
    
    while (check(ASTERISK)) {
        if (pointerDepth != -1) {
            ++pointerDepth;
            continue;
        }

        while(check(ASTERISK)); // skip all '*'
        error->createWarning(INCOMPATIBLE_TYPES, peek(-1),
            "given pointer depth wil be ignored and a reference parsed instead");
        error->addNote(typeDenotion, "the reference got declared here");
    }

    if (!current->isType()) {
        // if (typeDenotion == POINTER) {
        //     error->createError(UNEXPECTED_TOKEN, eat(), "expected a type after POINTER '->'");
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
        ExprAST::Ptr size = parseExpr();
        eat(RBRACKET, MISSING_BRACKET);
        return FuxType::createArray(kind, pointerDepth, access, value, root->addSizeExpr(size));
    } else 
        return FuxType::createStd(kind, pointerDepth, access, value);

    assert(false && "unreachable");
}

ExprAST::Ptr Parser::parseNumberExpr(Token &tok) {
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

ExprAST::Ptr Parser::parseCharExpr(Token &tok) {
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
        err << "got " << TokenTypeString[curTok.type] << " '" << curTok.value << "'";
        if (errType == MISSING_BRACKET)
            err << " instead of " << TokenTypeString[type] << " '" << TokenTypeValue[type] << "'";
        error->createError(errType, curTok, err.str());
        recover();
    }

    return *current; 
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

void Parser::recover(TokenType type) { while (*current != type && *current != _EOF) eat(); }

constexpr bool Parser::notEOF() { return *current != _EOF; }