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
    root = make_shared<Root>();
}

Parser::~Parser() {
    delete lexer;
    tokens.clear();
}

Root::Ptr Parser::parse() {
    // lexing
    tokens = lexer->lex();
    lexer->debugPrint();
    
    current = tokens.begin();

    Stmt::Ptr branch;
    while (notEOF()) 
        if ((branch = parseStmt())) // check for nullptr in case of error
            root->addSub(branch);
    
    return root;
}

Stmt::Ptr Parser::parseStmt(bool expectSemicolon) {
    if (*current == SEMICOLON) { // handle while (...); or for (;;);
        if (expectSemicolon)
            eat();
        return make_shared<NoOperationStmt>();
    }

    Stmt::Ptr stmt = parseMacroStmt();

    if (expectSemicolon && stmt 
    && stmt->getASTType() != AST::BlockStmt 
    && stmt->getASTType() != AST::FunctionStmt
    && stmt->getASTType() != AST::IfElseStmt
    && stmt->getASTType() != AST::WhileStmt
    && stmt->getASTType() != AST::ForStmt
    && stmt->getASTType() != AST::EnumStmt
    && stmt->getASTType() != AST::MacroStmt) { // don't throw useless errors
        if (!check(SEMICOLON)) {
            if (stmt->meta.file.empty()) 
                assert(!"metadata not implemented for parsed kind of AST");

            error->plainError(ParseError::UNEXPECTED_TOKEN, "Expected a semicolon ';' after statement",
                stmt->meta, error->createMark(stmt->meta, "Parsed statement", 
                    stmt->meta.lstCol + 1, "Expected semicolon ';' here"));
        } else
            stmt->meta.lstCol++; // add semicolon 
    }

    return stmt;
}

Stmt::Ptr Parser::parseMacroStmt() {
    if (!check(KEY_MACRO))
        return parseEnumStmt();

    Metadata meta = Metadata(fileName, peek(-1));
    Token &symbol = eat(IDENTIFIER);

    if (*current == LBRACE) {
        /*
            macro <symbol> {
                ( <args> ) -> <ret>
                ( <args> ) -> <ret>
                ...
            }
        */
        Token &opening = eat(); // {
        MacroStmt::Case::Vec cases = {};
        
        while (!check(RBRACE)) 
            if (!notEOF()) {
                error->refError(ParseError::MISSING_PAREN, "Macro body was never closed", Metadata(fileName),
                    {error->createUL(peek(-1).line, peek(-1).end + 1, peek(-1).end + 1, 0, "Expected a closing paren (RBRACE '}') here"),
                        error->createHelp(peek(-1).line, "You may have not closed a code block or array inside this macro body")},
                    Metadata(fileName),
                    {error->createUL(opening.line, opening.start, opening.end, 0, "Opening paren found here (LBRACE '{')")});
                    break;
            } else
                cases.push_back(parseMacroCase());

        meta.copyEnd(peek(-1));
        Stmt::Ptr macro = make_shared<MacroStmt>(symbol.value, cases);
        macro->meta = meta;
        return macro;
    } else if (*current == LPAREN) {
        // macro <symbol> ( <args> ) -> <ret>
        MacroStmt::Case *_case = parseMacroCase();
        meta.copyEnd(_case->meta);

        Stmt::Ptr macro = make_shared<MacroStmt>(symbol.value, MacroStmt::Case::Vec({_case}));
        macro->meta = meta;
        return macro;
    } else if (*current == POINTER) {
        // macro <symbol> -> <stmt>
        Token &ptr = eat();
        Stmt::Ptr ret = parseStmt();

        MacroStmt::Case *wildcard = new MacroStmt::Case({MacroStmt::Arg("*", MacroStmt::WILDCARD)}, ret);
        wildcard->meta = Metadata(fileName, ptr);
        wildcard->meta.copyEnd(wildcard->ret->meta);
        meta.copyEnd(wildcard->meta);

        Stmt::Ptr macro = make_shared<MacroStmt>(symbol.value, MacroStmt::Case::Vec({wildcard}));
        macro->meta = meta;
        return macro;
    } else if (*current == SEMICOLON) {
        // macro <symbol> ;
        meta.copyEnd(symbol);
        Stmt::Ptr macro = make_shared<MacroStmt>(symbol.value);
        macro->meta = meta;
        return macro;
    } 
        
    meta.copyEnd(symbol);
    error->plainError(ParseError::UNEXPECTED_TOKEN, "Unexpected token while parsing a macro",
        fileName, error->createMark(meta, "Parsed macro", current->start, "Unexpected token", 
            {error->createHelp(current->line, "Would have expected a macro body, case or prototype here")}));
    Stmt::Ptr macro = make_shared<MacroStmt>(symbol.value);
    macro->meta = meta;
    return macro;
}

Stmt::Ptr Parser::parseEnumStmt() {
    if (!check(KEY_ENUM))
        return parseFunctionDeclStmt();

    Metadata meta = Metadata(fileName, peek(-1));
    const string &symbol = eat(IDENTIFIER).value;

    if (*current == SEMICOLON) {
        Stmt::Ptr stmt = make_shared<EnumStmt>(symbol);
        meta.copyEnd(peek(-1));
        stmt->meta = meta;
        return stmt;
    }

    eat(LBRACE);
    vector<string> elements = {};
    do {
        if (*current == RBRACE)
            break;

        elements.push_back(eat(IDENTIFIER).value);
    } while (check(COMMA));
    eat(RBRACE, ParseError::MISSING_PAREN);

    Stmt::Ptr stmt = make_shared<EnumStmt>(symbol, elements);
    meta.copyEnd(peek(-1));
    stmt->meta = meta;
    return stmt;
}

Stmt::Ptr Parser::parseFunctionDeclStmt() {
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

    FunctionStmt::Parameter::Vec parameters = {};
    
    do {
        if (*current == RPAREN)
            break;
        FunctionStmt::Parameter::Ptr parameter = parseFuncParameter();
        if (parameter)
            parameters.push_back(parameter);
    } while (check(COMMA));
    eat(RPAREN, ParseError::MISSING_PAREN);

    FuxType type = parseType();

    if (*current == SEMICOLON) {
        Metadata meta = Metadata(fileName, *backToken);
        meta.copyEnd(type.meta);
        FunctionStmt::Ptr proto = make_shared<FunctionStmt>(meta, symbol, type, parameters);
        return proto;
    }

    FunctionStmt::Ptr node = make_shared<FunctionStmt>(Metadata(fileName, *backToken), symbol, type, parameters);
    parent = &*node;    
    node->body = parseStmt();
    parent = nullptr;
    node->meta.copyEnd(peek(-1));
    return node;
}

Stmt::Ptr Parser::parseForLoopStmt() { 
    bool forEach = false;
    Stmt::Ptr init = nullptr;
    Expr::Ptr cond = nullptr;
    Expr::Ptr iter = nullptr;

    if (!check(KEY_FOR))
        return parseWhileLoopStmt();

    Metadata meta = Metadata(fileName, peek(-1));

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
        else
            iter = make_shared<NullExpr>();
    }

    eat(RPAREN, ParseError::MISSING_PAREN);
    Stmt::Ptr body = parseStmt();
    
    meta.copyEnd(body->meta);
    Stmt::Ptr stmt;

    if (forEach)    
        stmt = make_shared<ForStmt>(init, iter, body);
    else            
        stmt = make_shared<ForStmt>(init, cond, iter, body);
    
    stmt->meta = meta;
    return stmt;
}

Stmt::Ptr Parser::parseWhileLoopStmt() {
    bool postCondition = false;
    Expr::Ptr condition = nullptr;
    Stmt::Ptr body = nullptr;
    Metadata meta = Metadata(fileName, *current);

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

    meta.copyEnd(body->meta);
    Stmt::Ptr stmt = make_shared<WhileStmt>(condition, body, postCondition);
    stmt->meta = meta;
    return stmt;
}

Stmt::Ptr Parser::parseBlockStmt() {
    if (check(LBRACE)) {
        Token &opening = peek(-1); // '{' position for error reporting 
        Stmt::Vec body;
        while (!check(RBRACE)) 
            if (!notEOF()) {
                error->refError(ParseError::MISSING_PAREN, "Code block was never closed", Metadata(fileName),
                    {error->createUL(peek(-1).line, peek(-1).end + 1, peek(-1).end + 1, 0, "Expected a closing paren (RBRACE '}') here"),
                        error->createHelp(peek(-1).line, "You may have not closed a code block or array inside this code block")},
                    Metadata(fileName),
                    {error->createUL(opening.line, opening.start, opening.end, 0, "Opening paren found here (LBRACE '{')")});
                break;
            } else
                body.push_back(parseStmt());
        Stmt::Ptr stmt = make_shared<BlockStmt>(body);
        stmt->meta = Metadata(fileName, opening);
        stmt->meta.copyEnd(peek(-1));
        return stmt;
    } 
    
    return parseIfElseStmt();
}

Stmt::Ptr Parser::parseIfElseStmt() {
    if (check(KEY_IF)) {
        Metadata meta = Metadata(fileName, peek(-1));

        eat(LPAREN);
        Expr::Ptr condition = parseExpr();
        eat(RPAREN, ParseError::MISSING_PAREN);
        
        Stmt::Ptr thenBody = parseStmt(); 

        Stmt::Ptr stmt;
        if (check(KEY_ELSE)) {
            Stmt::Ptr elseBody = parseStmt(); 
            meta.copyEnd(elseBody->meta);
            stmt = make_shared<IfElseStmt>(condition, thenBody, elseBody);
        } else {
            meta.copyEnd(thenBody->meta);
            stmt = make_shared<IfElseStmt>(condition, thenBody);
        }
        
        stmt->meta = meta;
        return stmt;
    } 

    return parseInbuiltCallStmt();
}

Stmt::Ptr Parser::parseInbuiltCallStmt() {
    if (current->isInbuiltCall()) {
        Metadata meta = Metadata(fileName, *current);
        Inbuilts callee = (Inbuilts) eat().type;
        Expr::Vec args = {};
        if (*current != SEMICOLON)
            args = parseExprList(SEMICOLON);
        meta.copyEnd(peek(-1));
        Stmt::Ptr stmt = make_shared<InbuiltCallStmt>(callee, args);
        stmt->meta = meta;
        return stmt;
    }

    return parseVariableStmt();
}

Stmt::Ptr Parser::parseVariableStmt() {
    if (*current != IDENTIFIER || (peek() != COLON && peek() != POINTER))
        return parseExpr();
    
    Metadata meta = Metadata(fileName, *current);
    const string symbol = eat().value; // get value from identifier
    FuxType type = parseType();

    if (check(TRIPLE_EQUALS)) // ===
        type.access.push_back(FuxType::CONSTANT);
    else if (!check(EQUALS)) {
        Stmt::Ptr decl = make_shared<VariableStmt>(symbol, type);
        meta.copyEnd(peek(-1));
        decl->meta = meta;

        if (parent) {
            parent->locals.push_back(decl);
            Expr::Ptr expr = make_shared<SymbolExpr>(symbol);
            expr->meta = meta;
            return expr;
        }

        return decl;
    }

    Expr::Ptr value = parseExpr();

    if (parent) {
        Stmt::Ptr decl = make_shared<VariableStmt>(symbol, type);
        meta.copyEnd(value->meta); 
        decl->meta = meta;
        parent->locals.push_back(decl);

        bool constant = find(type.access.begin(), type.access.end(), FuxType::CONSTANT) != type.access.end();
        Expr::Ptr ref = make_shared<SymbolExpr>(symbol);
        Expr::Ptr expr = make_shared<BinaryExpr>(constant ? BinaryOp::CONASG : BinaryOp::ASG, ref, value);
        expr->meta = meta;
        return expr;
    }

    meta.copyEnd(value->meta);
    Stmt::Ptr stmt = make_shared<VariableStmt>(symbol, type, value);
    stmt->meta = meta;
    return stmt;
}

Expr::Vec Parser::parseExprList(TokenType end) { 
    Expr::Vec list = Expr::Vec();
    Expr::Ptr expr = parseExpr();

    while (check(COMMA)) {
        if (*current == end)
            break;
        list.push_back(std::move(expr));
        expr = parseExpr();
    }

    list.push_back(std::move(expr));
    return list;
}
 

Expr::Ptr Parser::parseExpr() { return parseAssignmentExpr(); }

Expr::Ptr Parser::parseAssignmentExpr() { 
    Expr::Ptr dest = parsePipeExpr();

    if (current->isAssignment()) {
        BinaryOp op = (BinaryOp) eat().type;
        Expr::Ptr value = parsePipeExpr();
        dest = make_shared<BinaryExpr>(op, dest, value);
    }
    
    return dest;
}

Expr::Ptr Parser::parsePipeExpr() {
    Expr::Ptr LHS = parseTernaryExpr();

    while (*current == LSHIFT || *current == RSHIFT) {
        BinaryOp op = (BinaryOp) eat().type;
        Expr::Ptr RHS = parseTernaryExpr();
        LHS = make_shared<BinaryExpr>(op, LHS, RHS);
    }

    return LHS;
}

Expr::Ptr Parser::parseTernaryExpr() { 
    Expr::Ptr condition = parseLogicalOrExpr();

    while (check(QUESTION)) {
        Expr::Ptr thenExpr = parseLogicalOrExpr();
        eat(COLON);
        Expr::Ptr elseExpr = parseLogicalOrExpr();
        condition = make_shared<TernaryExpr>(condition, thenExpr, elseExpr);
    }

    return condition; 
}

Expr::Ptr Parser::parseLogicalOrExpr() { 
    Expr::Ptr LHS = parseLogicalAndExpr();

    while (check(OR)) {
        Expr::Ptr RHS = parseLogicalAndExpr();
        LHS = make_shared<BinaryExpr>(BinaryOp::LOR, LHS, RHS);
    }

    return LHS;
}

Expr::Ptr Parser::parseLogicalAndExpr() { 
    Expr::Ptr LHS = parseBitwiseOrExpr();

    while (check(AND)) {
        Expr::Ptr RHS = parseBitwiseOrExpr();
        LHS = make_shared<BinaryExpr>(BinaryOp::LAND, LHS, RHS);
    }

    return LHS;
}

Expr::Ptr Parser::parseBitwiseOrExpr() {
    Expr::Ptr LHS = parseBitwiseXorExpr();

    while (check(BIT_OR)) {
        Expr::Ptr RHS = parseBitwiseXorExpr();
        LHS = make_shared<BinaryExpr>(BinaryOp::BOR, LHS, RHS);
    }

    return LHS;
}

Expr::Ptr Parser::parseBitwiseXorExpr() { 
    Expr::Ptr LHS = parseBitwiseAndExpr(); 

    while (check(BIT_XOR)) {
        Expr::Ptr RHS = parseBitwiseAndExpr();
        LHS = make_shared<BinaryExpr>(BinaryOp::BXOR, LHS, RHS);
    }

    return LHS;
}

Expr::Ptr Parser::parseBitwiseAndExpr() { 
    Expr::Ptr LHS = parseEqualityExpr();

    while (check(BIT_AND)) {
        Expr::Ptr RHS = parseEqualityExpr();
        LHS = make_shared<BinaryExpr>(BinaryOp::BAND, LHS, RHS);
    }

    return LHS;
}

Expr::Ptr Parser::parseEqualityExpr() {
    Expr::Ptr LHS = parseRelationalExpr();

    while (*current == EQUALS_EQUALS || *current == NOT_EQUALS) {
        BinaryOp op = (BinaryOp) eat().type;
        Expr::Ptr RHS = parseRelationalExpr();
        LHS = make_shared<BinaryExpr>(op, LHS, RHS);
    }

    return LHS;
}

Expr::Ptr Parser::parseRelationalExpr() {
    Expr::Ptr LHS = parseBitwiseShiftExpr();

    while (current->isRelational()) {
        BinaryOp op = (BinaryOp) eat().type;
        Expr::Ptr RHS = parseBitwiseShiftExpr();
        LHS = make_shared<BinaryExpr>(op, LHS, RHS);
    }

    return LHS;
}

Expr::Ptr Parser::parseBitwiseShiftExpr() {
    Expr::Ptr LHS = parseAdditiveExpr();

    while (*current == BIT_LSHIFT || *current == BIT_RSHIFT) {
        BinaryOp op = (BinaryOp) eat().type;
        Expr::Ptr RHS = parseAdditiveExpr();
        LHS = make_shared<BinaryExpr>(op, LHS, RHS);
    }

    return LHS;
}

Expr::Ptr Parser::parseAdditiveExpr() {
    Expr::Ptr LHS = parseMultiplicativeExpr();

    while (*current == PLUS || *current == MINUS) {
        BinaryOp op = (BinaryOp) eat().type;
        Expr::Ptr RHS = parseMultiplicativeExpr();
        LHS = make_shared<BinaryExpr>(op, LHS, RHS);
    }

    return LHS;
}

Expr::Ptr Parser::parseMultiplicativeExpr() {
    Expr::Ptr LHS = parsePowerExpr();

    while (*current == ASTERISK || *current == SLASH || *current == PERCENT) {
        BinaryOp op = (BinaryOp) eat().type;
        Expr::Ptr RHS = parsePowerExpr();
        LHS = make_shared<BinaryExpr>(op, LHS, RHS);
    }

    return LHS;
}

Expr::Ptr Parser::parsePowerExpr() { 
    Expr::Ptr LHS = parseAddressExpr();

    while (check(CARET)) {
        Expr::Ptr RHS = parseAddressExpr();
        LHS = make_shared<BinaryExpr>(BinaryOp::POW, LHS, RHS);
    }    

    return LHS;
}

Expr::Ptr Parser::parseAddressExpr() { 
    if (*current == BIT_AND) {
        Token &op = eat();
        Expr::Ptr expr = parseDereferenceExpr();
        return make_shared<UnaryExpr>(op, expr);
    }

    return parseDereferenceExpr(); 
}

Expr::Ptr Parser::parseDereferenceExpr() { 
    if (*current == ASTERISK) {
        Token &op = eat();
        Expr::Ptr expr = parseDereferenceExpr();
        return make_shared<UnaryExpr>(op, expr);
    } 

    return parseTypeCastExpr(); 
}

Expr::Ptr Parser::parseTypeCastExpr() { 
    Token::Iter backToken = current;
    if (check(LPAREN)) {
        Token &open = peek(-1);
        FuxType type = parseType(true); // analyser will check wether type is primitive or not
        if (!type || *current != RPAREN) { // TODO: test more possible cases
            current = backToken; // get '*'s, identifier and '(' back
            return parseLogBitUnaryExpr();
        }
        eat(RPAREN, ParseError::MISSING_PAREN);
        Expr::Ptr expr = parseExpr();
        expr = make_shared<TypeCastExpr>(type, expr);
        expr->meta.fstLine = open.line;
        expr->meta.fstCol = open.start;
        return expr;
    }
   
    return parseLogBitUnaryExpr(); 
}

Expr::Ptr Parser::parseLogBitUnaryExpr() { 
    if (*current == EXCLAMATION || *current == BIT_NOT || *current == QUESTION) {
        Token &op = eat();
        Expr::Ptr expr = parsePlusMinusUnaryExpr();
        return make_shared<UnaryExpr>(op, expr);
    }

    return parsePlusMinusUnaryExpr(); 
}

Expr::Ptr Parser::parsePlusMinusUnaryExpr() { 
    if (*current == PLUS || *current == MINUS) {
        Token &op = eat();
        Expr::Ptr expr = parsePreIncDecExpr();
        return make_shared<UnaryExpr>(op, expr);
    }
    
    return parsePreIncDecExpr(); 
}

Expr::Ptr Parser::parsePreIncDecExpr() { 
    if (*current == PLUS_PLUS || *current == MINUS_MINUS) {
        Token &op = eat();
        Expr::Ptr expr = parsePostIncDecExpr();
        return make_shared<UnaryExpr>(op, expr);
    } 
    
    return parsePostIncDecExpr(); 
}

Expr::Ptr Parser::parsePostIncDecExpr() {
    Expr::Ptr expr = parseTopMemberExpr();

    if (*current == PLUS_PLUS || *current == MINUS_MINUS) 
        expr = make_shared<UnaryExpr>(eat(), expr, true);    
    
    return expr;
}

Expr::Ptr Parser::parseTopMemberExpr() {
    Expr::Ptr parent = parseIndexExpr();

    if (check(DOT)) {
        parent = make_shared<MemberExpr>(parent, eat(IDENTIFIER));

        if (check(LPAREN))
            parent = parseCallExpr(std::move(parent));
        else if (*current == ARRAY_BRACKET || *current == LBRACKET)
            parent = parseIndexExpr(std::move(parent));
    }

    return parent;
}

Expr::Ptr Parser::parseIndexExpr(Expr::Ptr parent) { 
    if (parent == nullptr)
        parent = parseMidMemberExpr(); 
    
    if (check(ARRAY_BRACKET)) {
        parent = make_shared<BinaryExpr>(BinaryOp::IDX, parent);
        parent->meta.copyEnd(peek(-1));
    } else if (check(LBRACKET)) {
        Expr::Ptr index = parseExpr();
        Token &close = eat(RBRACKET, ParseError::MISSING_PAREN);
        parent = make_shared<BinaryExpr>(BinaryOp::IDX, parent, index);
        parent->meta.copyEnd(close);
    }

    if (check(LPAREN))
        parent = parseCallExpr(std::move(parent));
    else if (*current == ARRAY_BRACKET || *current == LBRACKET)
        parent  = parseIndexExpr(std::move(parent));

    return parent;
}

Expr::Ptr Parser::parseMidMemberExpr() {
    Expr::Ptr parent = parseCallExpr();

    if (check(DOT)) {
        parent = make_shared<MemberExpr>(parent, eat(IDENTIFIER));

        if (check(LPAREN))
            parent = parseCallExpr(std::move(parent));
    }

    return parent;
}

Expr::Ptr Parser::parseCallExpr(Expr::Ptr callee) { 
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
    
    Expr::Vec arguments = Expr::Vec();
    if (*current != RPAREN)
        arguments = parseExprList(RPAREN);
    eat(RPAREN, ParseError::MISSING_PAREN);
    
    Expr::Ptr expr = make_shared<CallExpr>(callee, arguments, (bool) asyncTok);
    expr->meta.copyEnd(asyncTok ? *asyncTok : peek(-1));

    if (check(LPAREN))
        expr = parseCallExpr(std::move(expr));
    else if (*current == ARRAY_BRACKET || *current == LBRACKET)
        expr = parseIndexExpr(std::move(expr));
    
    return expr;
} 

Expr::Ptr Parser::parseBotMemberExpr() {
    Expr::Ptr parent = parsePrimaryExpr();

    while (check(DOT)) 
        parent = make_shared<MemberExpr>(parent, eat(IDENTIFIER));
    
    return parent;
}

Expr::Ptr Parser::parsePrimaryExpr() {
    Token that = eat();
    Expr::Ptr expr;

    // Pointer Types will be parsed as dereference expressions.
    // The analyser has to translate these to primitive types. 
    // I don't know wether it is possible to filter these while parsing.
    // e.g.: *(*(i64)) ---Analyser--> **i64  
    if (that.isType()) {
        expr = make_shared<SymbolExpr>(that.value);
        expr->meta = Metadata(fileName, that);
        return expr;
    }

    switch (that.type) {
        case HEXADECIMAL:
        case NUMBER:        
        case OCTAL:
        case BINARY: {
            Expr::Ptr beginExpr = parseNumberExpr(that);

            if (check(TRIPLE_DOT)) {
                Expr::Ptr endExpr = nullptr;
                Token &endTok = eat();
                if (endTok.type == HEXADECIMAL 
                || endTok.type == NUMBER 
                || endTok.type == OCTAL
                || endTok.type == BINARY)
                    endExpr = parseNumberExpr(endTok);
                else 
                    error->plainError(ParseError::ILLEGAL_OPERANDS, "IncompleterRange expression", fileName,
                        error->createMark(that.line, endTok.line, that.start, endTok.end,
                            "Range expression indicated by '...' operato.", endTok.start, "Would have expected an integer here",
                            {error->createHelp(endTok.line, "The LHS and RHS of a range expression have to be constants")}));

                if (endExpr.get() == nullptr) {
                    endExpr = make_shared<NullExpr>();
                    endExpr->meta.copyWhole(endTok);
                }

                beginExpr = make_shared<RangeExpr>(beginExpr, endExpr);
            }
            
            return beginExpr;
        }
        case FLOAT:      
            expr = make_shared<NumberExpr, _f64>(stod(that.value));
            expr->meta = Metadata(fileName, that);
            return expr;
        case CHAR:          
            return parseCharExpr(that);
        case STRING:        
            expr = make_shared<StringExpr>(escapeSequences(that.value)); 
            expr->meta = Metadata(fileName, that);
            return expr;
        case KEY_TRUE:      
        case KEY_FALSE:     
            expr = make_shared<BoolExpr>(that == KEY_TRUE);
            expr->meta = Metadata(fileName, that);
            return expr;
        case KEY_NULL:     
            expr = make_shared<NullExpr>();
            expr->meta = Metadata(fileName, that);
            return expr;
        case LPAREN: 
            expr = parseExpr();
            expr->enclosed = true;
            eat(RPAREN, ParseError::MISSING_PAREN);
            return expr;
        case LBRACE: {
            Expr::Vec elements = parseExprList(RBRACE);
            eat(RBRACE, ParseError::MISSING_PAREN);
            return make_shared<ArrayExpr>(elements);
        }
        case _EOF: 
            createError(ParseError::UNEXPECTED_EOF, "Unexpected EOF while parsing Primary Expression",
                peek(-2), "Last token pased", peek(-1).start, "Expected an expression here", false);
            return nullptr;
        default:    
            createError(ParseError::UNEXPECTED_TOKEN, "Unexpected Token while parsing Primary Expression",
                that, "Unexpected token "+string(TokenTypeString[that.type])+" '"+that.value+"'");
            recover();
            return make_shared<NullExpr>();
    }
}

FuxType Parser::parseType(bool primitive) {
    Metadata meta = Metadata(fileName, *current);
    
    if (primitive) {
        bool reference = check(POINTER);

        int64_t pointerDepth = 0;
        while (check(ASTERISK)) 
            ++pointerDepth;

        if (!current->isType()) 
            return FuxType(FuxType::NO_TYPE, pointerDepth);
        
        const FuxType::Kind kind = (FuxType::Kind) current->type;
        const Token &value = eat();
        FuxType ret = FuxType::createPrimitive(kind, pointerDepth, reference, check(ARRAY_BRACKET), value.value);
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
        Expr::Ptr size = parseExpr();
        eat(RBRACKET, ParseError::MISSING_PAREN);
        ret = FuxType::createArray(kind, pointerDepth, reference, access, value, root->addSizeExpr(size));
    } else 
        ret = FuxType::createStd(kind, pointerDepth, reference, access, value);
    
    meta.copyEnd(peek(-1));
    ret.meta = meta;
    return ret;
}

Expr::Ptr Parser::parseNumberExpr(Token &tok) {
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

    Expr::Ptr expr = make_shared<NumberExpr>(value);
    expr->meta = Metadata(fileName, tok);
    return expr;
}

Expr::Ptr Parser::parseCharExpr(Token &tok) {
    // TODO: add support for c16
    _c8 value = escapeSequences(tok.value).front();
    Expr::Ptr expr = make_shared<CharExpr>(value);
    expr->meta = Metadata(fileName, tok);
    return expr;
}

MacroStmt::Case *Parser::parseMacroCase() {
    eat(LPAREN);
    Metadata meta = Metadata(fileName, peek(-1));
    MacroStmt::Arg::Vec args = {};
    
    do {
        if (*current == RPAREN)
            break;
        args.push_back(parseMacroArg());
    } while (check(COMMA));
    
    eat(RPAREN, ParseError::MISSING_PAREN);
    
    eat(POINTER);

    Stmt::Ptr ret = parseStmt();
    meta.copyEnd(ret->meta);
    MacroStmt::Case *that = new MacroStmt::Case(args, ret);
    that->meta = meta;
    return that;
}

MacroStmt::Arg Parser::parseMacroArg() {
    if (check(ASTERISK)) {
        MacroStmt::Arg that("*", MacroStmt::WILDCARD);
        that.meta = Metadata(fileName, peek(-1));
        return that;
    }

    Token &symbol = eat(IDENTIFIER);
    MacroStmt::Arg that(symbol.value, MacroStmt::NONE);
    eat(COLON);

    Token &id = eat(IDENTIFIER);
    if (id.value == "type")
        that.type = MacroStmt::TYPE;
    else if (id.value == "ident")
        that.type = MacroStmt::IDENT;
    else if (id.value == "expr")
        that.type = MacroStmt::EXPR;
    else if (id.value == "stmt")
        that.type = MacroStmt::STMT;
    else if (id.value == "block")
        that.type = MacroStmt::BLOCK;
    else
        error->plainError(ParseError::ILLEGAL_TYPE, "Expected macro parameter type", fileName,
            error->createMark(symbol.line, id.line, symbol.start, id.end, 
                "Parsed macro parameter", id.start, 
                "Expected a macro parameter type here", 
                {error->createHelp(id.line, "Possible types are: 'type', 'ident', 'expr', 'stmt' or 'block'")}));

    // symbol: id[]
    that.array = check(ARRAY_BRACKET);

    // symbol: id, ...
    that.variadic = check(COMMA, TRIPLE_DOT);

    that.meta = Metadata(fileName, symbol.line, id.line, symbol.start, id.end);
    return that;
}

FunctionStmt::Parameter::Ptr Parser::parseFuncParameter() {
    // TODO: add proper errors

    Metadata meta = Metadata(fileName);
    Token &symbol = eat(IDENTIFIER);
    meta.copyWhole(symbol);
    FuxType type = parseType();
    Expr::Ptr value = nullptr;

    if (check(EQUALS)) {
        value = parseExpr();
        meta.copyEnd(value->meta);
    } else
        meta.copyEnd(type.meta);

    // <parameter>, ...                
    bool variadic = check(COMMA, TRIPLE_DOT);
    if (variadic)
        meta.copyEnd(peek(-1));

    return make_shared<FunctionStmt::Parameter>(meta, symbol.value, type, value, variadic);
}

Token &Parser::eat() {
    if (*current == _EOF)
        return *current;
    return *current++;
}

Token &Parser::eat(TokenType type, ParseError::Type errType) {
    Token curTok = eat();

    if (curTok != type) {
        createError(errType, "Got unexpected token "+string(TokenTypeString[curTok.type])+" '"+curTok.value+"'", 
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
    bool warning, bool aggressive) {
        if (warning)
            error->plainWarning(type, title, fileName, 
                error->createMark(token.line, token.line, token.start, token.end, info, ptr, ptrText));
        else
            error->plainError(type, title, fileName, 
                error->createMark(token.line, token.line, token.start, token.end, info, ptr, ptrText));
}

void Parser::createError(
    ParseError::Type type, string title,
    const Token &token, string info, 
    const Token &refTok, string refInfo,
    bool warning, bool aggressive) {
        error->refError(type, title, 
            Metadata(fileName), {error->createUL(token.line, token.start, token.end, 0, info)},
            Metadata(fileName), {error->createUL(refTok.line, refTok.start, refTok.end, 0, refInfo)},
            warning, aggressive);
}