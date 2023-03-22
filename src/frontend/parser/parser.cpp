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
: fileName(fileName), error(error), mainFile(mainFile) {
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
        return make_unique<NoOperationAST>();
    }

    StmtAST::Ptr stmt = parseFunctionDeclStmt();
    if (expectSemicolon && stmt 
    && stmt->getASTType() != AST::CodeBlockAST 
    && stmt->getASTType() != AST::FunctionAST
    && stmt->getASTType() != AST::IfElseAST
    && stmt->getASTType() != AST::WhileLoopAST
    && stmt->getASTType() != AST::ForLoopAST) // don't throw useless errors
        if (!check(SEMICOLON)) {
            if (!stmt->meta.file) // TODO: implement metadata for every ast
                assert(!"metadata not implemented for parsed kind of AST");
            error->metaError(ParseError::UNEXPECTED_TOKEN, "Expected a Semicolon ';' after Statement", 
                stmt->meta, "Parsed statement", peek(-1).end + 1, "Expected semicolon ';' here");
        }
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
        else if (check(RPAREN)) --depth; // we don't care about any errors here, so we don't check wether depth is < 0
        else if (check(_EOF))   {
            createError(ParseError::MISSING_PAREN, "Expected Closing Paren after Parameter List", peek(-1), "Expected closing paren here",
                *paramBegin, "Opening paren found here"); 
            recover();
            break;
        } else                  eat();

        if (depth <= 0)
            break;
    } // skip ( ... )

    if (*current != COLON && *current != POINTER) {
        current = backToken;
        return parseExpr(); // We have to call parseExpr() instead of parseCallExpr() to handle situations like this one:
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
    eat(RPAREN, ParseError::MISSING_PAREN);

    FuxType type = parseType();

    if (*current == SEMICOLON)
        return make_unique<PrototypeAST>(type, symbol, args);

    FunctionAST::Ptr node = make_unique<FunctionAST>(type, symbol, args);
    parent = &*node;    
    StmtAST::Ptr body = parseStmt();
    parent = nullptr;
    node->setBody(body);
    return std::move(node);
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

    eat(RPAREN, ParseError::MISSING_PAREN);
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
        eat(RPAREN, ParseError::MISSING_PAREN);
    } else if (check(KEY_WHILE)) {
        eat(LPAREN);
        condition = parseExpr();
        eat(RPAREN, ParseError::MISSING_PAREN);
        body = parseStmt();
    } else
        return parseBlockStmt();

    return make_unique<WhileLoopAST>(condition, body, postCondition);
}

StmtAST::Ptr Parser::parseBlockStmt() {
    if (check(LBRACE)) {
        Token &opening = peek(-1); // '{' position for error reporting 
        StmtAST::Vec body;
        while (!check(RBRACE)) {
            if (!notEOF()) {
                createError(ParseError::MISSING_PAREN, "Code Block was never closed", *(current - 1), "Expected a closing paren (RBRACE '}') here", 
                    opening, "Opening paren found here (LBRACE '{')");
                return make_unique<NoOperationAST>(); 
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
        eat(RPAREN, ParseError::MISSING_PAREN);
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
        Metadata meta = Metadata(fileName, *current);
        Inbuilts callee = (Inbuilts) eat().type;
        ExprAST::Vec args = {};
        if (*current != SEMICOLON)
            args = parseExprList(SEMICOLON);
        meta.copyEnd(peek(-1));
        StmtAST::Ptr stmt = make_unique<InbuiltCallAST>(callee, args);
        stmt->meta = meta;
        return stmt;
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
    else if (!check(EQUALS)) {
        StmtAST::Ptr decl = make_unique<VariableDeclAST>(symbol, type);

        if (parent) {
            parent->addLocal(decl);
            return make_unique<VariableExprAST>(symbol);
        }

        return decl;
    }

    ExprAST::Ptr value = parseExpr();

    if (parent) {
        StmtAST::Ptr decl = make_unique<VariableDeclAST>(symbol, type);
        parent->addLocal(decl);
        bool constant = find(type.access.begin(), type.access.end(), FuxType::CONSTANT) != type.access.end();
        ExprAST::Ptr ref = make_unique<VariableExprAST>(symbol);
        return make_unique<BinaryExprAST>(constant ? BinaryOp::CONASG : BinaryOp::ASG, ref, value);
    }

    return make_unique<VariableDeclAST>(symbol, type, value);
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
    if (*current == BIT_AND) {
        ExprAST::Ptr expr = parseDereferenceExpr();
        return make_unique<UnaryExprAST>(eat(), expr);
    }

    return parseDereferenceExpr(); 
}

ExprAST::Ptr Parser::parseDereferenceExpr() { 
    if (*current == ASTERISK) {
        ExprAST::Ptr expr = parseTypeCastExpr();
        return make_unique<UnaryExprAST>(eat(), expr);
    }

    return parseTypeCastExpr(); 
}

ExprAST::Ptr Parser::parseTypeCastExpr() { 
    Token::Iter backToken = current;
    if (check(LPAREN)) {
        Token &open = peek(-1);
        FuxType type = parseType(true); // analyser will check wether type is primitive or not
        if (!type || *current != RPAREN) { // TODO: test more possible cases
            current = backToken; // get '*'s, identifier and '(' back
            return parseLogBitUnaryExpr();
        }
        eat(RPAREN, ParseError::MISSING_PAREN);
        ExprAST::Ptr expr = parseExpr();
        expr = make_unique<TypeCastExprAST>(type, expr);
        expr->meta.fstLine = open.line;
        expr->meta.fstCol = open.start;
        return expr;
    }
   
    return parseLogBitUnaryExpr(); 
}

ExprAST::Ptr Parser::parseLogBitUnaryExpr() { 
    if (*current == EXCLAMATION || *current == BIT_NOT || *current == QUESTION) {
        Token &op = eat();
        ExprAST::Ptr expr = parsePlusMinusUnaryExpr();
        return make_unique<UnaryExprAST>(op, expr);
    }

    return parsePlusMinusUnaryExpr(); 
}

ExprAST::Ptr Parser::parsePlusMinusUnaryExpr() { 
    if (*current == PLUS || *current == MINUS) {
        Token &op = eat();
        ExprAST::Ptr expr = parsePreIncDecExpr();
        return make_unique<UnaryExprAST>(op, expr);
    }
    
    return parsePreIncDecExpr(); 
}

ExprAST::Ptr Parser::parsePreIncDecExpr() { 
    if (*current == PLUS_PLUS || *current == MINUS_MINUS) {
        Token &op = eat();
        ExprAST::Ptr expr = parsePostIncDecExpr();
        return make_unique<UnaryExprAST>(op, expr);
    } 
    
    return parsePostIncDecExpr(); 
}

ExprAST::Ptr Parser::parsePostIncDecExpr() {
    ExprAST::Ptr expr = parseTopMemberExpr();

    if (*current == PLUS_PLUS || *current == MINUS_MINUS) 
        expr = make_unique<UnaryExprAST>(eat(), expr, true);    
    
    return expr;
}

ExprAST::Ptr Parser::parseTopMemberExpr() {
    ExprAST::Ptr parent = parseIndexExpr();

    if (check(DOT)) {
        parent = make_unique<MemberExprAST>(parent, eat(IDENTIFIER));

        if (check(LPAREN))
            parent = parseCallExpr(std::move(parent));
        else if (*current == ARRAY_BRACKET || *current == LBRACKET)
            parent = parseIndexExpr(std::move(parent));
    }

    return parent;
}

ExprAST::Ptr Parser::parseIndexExpr(ExprAST::Ptr parent) { 
    if (parent == nullptr)
        parent = parseMidMemberExpr(); 
    
    if (check(ARRAY_BRACKET)) {
        parent = make_unique<BinaryExprAST>(BinaryOp::IDX, parent);
        parent->meta.copyEnd(peek(-1));
    } else if (check(LBRACKET)) {
        ExprAST::Ptr index = parseExpr();
        Token &close = eat(RBRACKET, ParseError::MISSING_PAREN);
        parent = make_unique<BinaryExprAST>(BinaryOp::IDX, parent, index);
        parent->meta.copyEnd(close);
    }

    if (check(LPAREN))
        parent = parseCallExpr(std::move(parent));
    else if (*current == ARRAY_BRACKET || *current == LBRACKET)
        parent  = parseIndexExpr(std::move(parent));

    return parent;
}

ExprAST::Ptr Parser::parseMidMemberExpr() {
    ExprAST::Ptr parent = parseCallExpr();

    if (check(DOT)) {
        parent = make_unique<MemberExprAST>(parent, eat(IDENTIFIER));

        if (check(LPAREN))
            parent = parseCallExpr(std::move(parent));
    }

    return parent;
}

ExprAST::Ptr Parser::parseCallExpr(ExprAST::Ptr callee) { 
    Token *asyncTok = nullptr;

    if (callee == nullptr) {
        if (*current != IDENTIFIER && *current != KEY_ASYNC)
            return parseBotMemberExpr();

        Token::Iter backTok = current;

        asyncTok = check(KEY_ASYNC) ? &peek(-1) : nullptr;
        callee = parseBotMemberExpr();
                
        if (!check(LPAREN)) {
            current = backTok;
            return parseBotMemberExpr();
        }
    }
    
    ExprAST::Vec arguments = ExprAST::Vec();
    if (*current != RPAREN)
        arguments = parseExprList(RPAREN);
    eat(RPAREN, ParseError::MISSING_PAREN);
    
    ExprAST::Ptr expr = make_unique<CallExprAST>(callee, arguments, (bool) asyncTok);
    expr->meta.copyEnd(asyncTok ? *asyncTok : peek(-1));

    if (check(LPAREN))
        expr = parseCallExpr(std::move(expr));
    else if (*current == ARRAY_BRACKET || *current == LBRACKET)
        expr = parseIndexExpr(std::move(expr));
    
    return expr;
} 

ExprAST::Ptr Parser::parseBotMemberExpr() {
    ExprAST::Ptr parent = parsePrimaryExpr();

    while (check(DOT)) 
        parent = make_unique<MemberExprAST>(parent, eat(IDENTIFIER));
    
    return parent;
}

ExprAST::Ptr Parser::parsePrimaryExpr() {
    Token that = eat();
    ExprAST::Ptr expr;

    if (that.isType()) {
        expr = make_unique<VariableExprAST>(that.value);
        expr->meta = Metadata(fileName, that);
        return expr;
    }

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
                else 
                    error->simpleError(ParseError::ILLEGAL_OPERANDS, "Incomplete Range Expression", fileName,
                        that.line, endTok.line, that.start, endTok.end, "Range expression indicated by '...' operator.", 
                        endTok.start, "Would have expected an integer here.", 
                        {"Help: The LHS and the RHS of a range expression have to be constants."});
                
                beginExpr = make_unique<RangeExprAST>(beginExpr, endExpr);
                beginExpr->meta.copyEnd(endExpr->meta);
            }
            
            return beginExpr;
        }
        case FLOAT:      
            expr = make_unique<NumberExprAST, _f64>(stod(that.value));
            expr->meta = Metadata(fileName, that);
            return expr;
        case CHAR:          
            return parseCharExpr(that);
        case STRING:        
            expr = make_unique<StringExprAST>(escapeSequences(that.value)); 
            expr->meta = Metadata(fileName, that);
            return expr;
        case KEY_TRUE:      
        case KEY_FALSE:     
            expr = make_unique<BoolExprAST>(that == KEY_TRUE);
            expr->meta = Metadata(fileName, that);
            return expr;
        case KEY_NULL:     
            expr = make_unique<NullExprAST>();
            expr->meta = Metadata(fileName, that);
            return expr;
        case LPAREN: 
            expr = parseExpr();
            eat(RPAREN, ParseError::MISSING_PAREN);
            return expr;
        case LBRACE: {
            ExprAST::Vec elements = parseExprList(RBRACE);
            eat(RBRACE, ParseError::MISSING_PAREN);
            return make_unique<ArrayExprAST>(elements);
        }
        case _EOF: 
            createError(ParseError::UNEXPECTED_EOF, "Unexpected EOF while parsing Primary Expression",
                that, "Expected a primay expression here", 0, "", {}, false, true);
            return make_unique<NullExprAST>();
        default:    
            createError(ParseError::UNEXPECTED_TOKEN, "Unexpected Token while parsing Primary Expression",
                that, "Unexpected token "+string(TokenTypeString[that.type])+" '"+that.value+"'");
            recover();
            return make_unique<NullExprAST>();
    }
}

FuxType Parser::parseType(bool primitive) {
    Metadata meta = Metadata(fileName, *current);
    
    if (primitive) {
        int64_t pointerDepth = 0;
        while(check(ASTERISK)) 
            ++pointerDepth;
        if (!current->isType()) 
            return FuxType(FuxType::NO_TYPE, pointerDepth);
        const FuxType::Kind kind = (FuxType::Kind) current->type;
        const Token &value = eat();
        FuxType ret = FuxType::createPrimitive(kind, pointerDepth, check(ARRAY_BRACKET), value.value);
        meta.copyEnd(value);
        ret.meta = meta;
        return ret;
    }

    FuxType::AccessList access = {FuxType::PUBLIC};
    size_t pointerDepth = 0;

    Token &typeDenotion = eat(); // ':' or '->' for error tracking
    bool reference;
    switch (typeDenotion.type) {
        case COLON:     reference = false; break;
        case POINTER:   reference = true; break;
        default:        
            createError(ParseError::UNEXPECTED_TOKEN, "Unexpected Token while parsing a Type", 
                typeDenotion, "Expected a COLON ':' or POINTER '->' here");
            return FuxType();
    }

    while (current->isModifier())
        access.push_back((FuxType::Access) eat().type);
    
    while (check(ASTERISK)) 
        ++pointerDepth;

    if (!current->isType()) {
        if (pointerDepth > 0)
            createError(ParseError::ILLEGAL_TYPE, "Pointer-Depth on Automatic Type",
                peek(-1), "Given pointer-depth will be ignored",
                0, "", {}, true);
        FuxType ret = FuxType::createStd(FuxType::AUTO, 0, reference, access);
        meta.copyEnd(peek(-1));
        ret.meta = meta;
        return ret;
    }

    const FuxType::Kind kind = (FuxType::Kind) current->type;
    const string &value = eat().value;
    FuxType ret;

    if (check(ARRAY_BRACKET)) 
        ret = FuxType::createArray(kind, pointerDepth, reference, access, value);
    else if (check(LBRACKET)) {
        ExprAST::Ptr size = parseExpr();
        eat(RBRACKET, ParseError::MISSING_PAREN);
        ret = FuxType::createArray(kind, pointerDepth, reference, access, value, root->addSizeExpr(size));
    } else 
        ret = FuxType::createStd(kind, pointerDepth, reference, access, value);
    
    meta.copyEnd(peek(-1));
    ret.meta = meta;
    return ret;
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
    ExprAST::Ptr expr;

    if      (bits <= 8)     expr = make_unique<NumberExprAST, _u8>(value);
    else if (bits <= 16)    expr = make_unique<NumberExprAST, _u16>(value);
    else if (bits <= 32)    expr = make_unique<NumberExprAST, _u32>(value);
    else if (bits <= 64)    expr = make_unique<NumberExprAST>(value);
    else                    expr = nullptr; // unreachable   

    expr->meta = Metadata(fileName, tok);
    return expr;
}

ExprAST::Ptr Parser::parseCharExpr(Token &tok) {
    // TODO: add support for c16
    _c8 value = escapeSequences(tok.value).front();
    ExprAST::Ptr expr = make_unique<CharExprAST>(value);
    expr->meta = Metadata(fileName, tok);
    return expr;
}

Token &Parser::eat() {
    if (*current == _EOF)
        return *current;
    return *current++;
}

Token &Parser::eat(TokenType type, ParseError::Type errType) {
    Token curTok = eat();

    if (curTok != type) {
        createError(errType, "Got Unexpected Token "+string(TokenTypeString[curTok.type])+" '"+curTok.value+"'", 
            peek(-1), "", curTok.start, 
            "Expected "+string(TokenTypeString[type])+" '"+TokenTypeValue[type]+"' here instead"); 
        // TODO: better error
        recover();
    }

    return peek(-1); 
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

void Parser::recover(TokenType type) { 
    while (*current != type && *current != _EOF) 
        eat(); 
}

constexpr bool Parser::notEOF() { return *current != _EOF; }

void Parser::createError(
    ParseError::Type type, string title, 
    const Token &token, string info, size_t ptr, string ptrText,
    vector<string> notes, bool warning, bool aggressive) {
        error->simpleError(type, title, fileName, token.line, token.line, token.start, token.end, 
            info, ptr, ptrText, notes, warning, aggressive);
}

void Parser::createError(
    ParseError::Type type, string title,
    const Token &token, string info, 
    const Token &refTok, string refInfo,
    vector<string> notes, bool warning, bool aggressive) {
        error->createError(type, title, 
            fileName, token.line, token.line, token.start, token.end, info, 0, "", 
            fileName, refTok.line, refTok.line, refTok.start, refTok.end, refInfo, 0, "",
            notes, true, warning, aggressive);
}