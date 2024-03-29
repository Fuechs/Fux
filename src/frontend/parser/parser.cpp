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

    StmtAST::Ptr stmt = parseMacroStmt();
    if (expectSemicolon && stmt 
    && stmt->getASTType() != AST::CodeBlockAST 
    && stmt->getASTType() != AST::FunctionAST
    && stmt->getASTType() != AST::IfElseAST
    && stmt->getASTType() != AST::WhileLoopAST
    && stmt->getASTType() != AST::ForLoopAST
    && stmt->getASTType() != AST::EnumerationAST
    && stmt->getASTType() != AST::MacroAST) { // don't throw useless errors
        if (!check(SEMICOLON)) {
            if (!stmt->meta.file) // TODO: implement metadata for every ast
                assert(!"metadata not implemented for parsed kind of AST");

            error->plainError(ParseError::UNEXPECTED_TOKEN, "Expected a semicolon ';' after statement",
                stmt->meta, error->createMark(stmt->meta, "Parsed statement", 
                    stmt->meta.lstCol + 1, "Expected semicolon ';' here"));
        } else
            stmt->meta.lstCol++; // add semicolon 
    }
    return stmt;
}

StmtAST::Ptr Parser::parseMacroStmt() {
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
        MacroAST::Case::Vec cases = {};
        
        while (!check(RBRACE)) 
            if (!notEOF()) {
                error->refError(ParseError::MISSING_PAREN, "Macro body was never closed", error->getMeta(fileName),
                    {error->createUL(peek(-1).line, peek(-1).end + 1, peek(-1).end + 1, 0, "Expected a closing paren (RBRACE '}') here"),
                        error->createHelp(peek(-1).line, "You may have not closed a code block or array inside this macro body")},
                    error->getMeta(fileName),
                    {error->createUL(opening.line, opening.start, opening.end, 0, "Opening paren found here (LBRACE '{')")});
                    break;
            } else
                cases.push_back(parseMacroCase());

        meta.copyEnd(peek(-1));
        StmtAST::Ptr macro = make_unique<MacroAST>(symbol.value, cases);
        macro->meta = meta;
        return macro;
    } else if (*current == LPAREN) {
        // macro <symbol> ( <args> ) -> <ret>
        MacroAST::Case *_case = parseMacroCase();
        meta.copyEnd(_case->meta);

        StmtAST::Ptr macro = make_unique<MacroAST>(symbol.value, MacroAST::Case::Vec({_case}));
        macro->meta = meta;
        return macro;
    } else if (*current == POINTER) {
        // macro <symbol> -> <stmt>
        Token &ptr = eat();
        StmtAST::Ptr ret = parseStmt();

        MacroAST::Case *wildcard = new MacroAST::Case({MacroAST::Arg("*", MacroAST::WILDCARD)}, ret);
        wildcard->meta = Metadata(fileName, ptr);
        wildcard->meta.copyEnd(wildcard->ret->meta);
        meta.copyEnd(wildcard->meta);

        StmtAST::Ptr macro = make_unique<MacroAST>(symbol.value, MacroAST::Case::Vec({wildcard}));
        macro->meta = meta;
        return macro;
    } else if (*current == SEMICOLON) {
        // macro <symbol> ;
        meta.copyEnd(symbol);
        StmtAST::Ptr macro = make_unique<MacroAST>(symbol.value);
        macro->meta = meta;
        return macro;
    } 
        
    meta.copyEnd(symbol);
    error->plainError(ParseError::UNEXPECTED_TOKEN, "Unexpected token while parsing a macro",
        fileName, error->createMark(meta, "Parsed macro", current->start, "Unexpected token", 
            {error->createHelp(current->line, "Would have expected a macro body, case or prototype here")}));
    StmtAST::Ptr macro = make_unique<MacroAST>(symbol.value);
    macro->meta = meta;
    return macro;
}

StmtAST::Ptr Parser::parseEnumStmt() {
    if (!check(KEY_ENUM))
        return parseFunctionDeclStmt();

    Metadata meta = Metadata(fileName, peek(-1));
    const string &symbol = eat(IDENTIFIER).value;

    if (*current == SEMICOLON) {
        StmtAST::Ptr stmt = make_unique<EnumerationAST>(symbol);
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

    StmtAST::Ptr stmt = make_unique<EnumerationAST>(symbol, elements);
    meta.copyEnd(peek(-1));
    stmt->meta = meta;
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

    if (*current == SEMICOLON) {
        PrototypeAST::Ptr proto = make_unique<PrototypeAST>(type, symbol, args);
        proto->meta = Metadata(fileName, *backToken);
        proto->meta.copyEnd(type.meta);
        return proto;
    }

    FunctionAST::Ptr node = make_unique<FunctionAST>(type, symbol, args);
    parent = &*node;    
    StmtAST::Ptr body = parseStmt();
    parent = nullptr;
    node->meta = Metadata(fileName, *backToken);
    node->meta.copyEnd(body->meta);
    node->setBody(body);
    return node;
}

StmtAST::Ptr Parser::parseForLoopStmt() { 
    bool forEach = false;
    StmtAST::Ptr init = nullptr;
    ExprAST::Ptr cond = nullptr;
    ExprAST::Ptr iter = nullptr;

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
            iter = make_unique<NullExprAST>();
    }

    eat(RPAREN, ParseError::MISSING_PAREN);
    StmtAST::Ptr body = parseStmt();
    
    meta.copyEnd(body->meta);
    StmtAST::Ptr stmt;

    if (forEach)    
        stmt = make_unique<ForLoopAST>(init, iter, body);
    else            
        stmt = make_unique<ForLoopAST>(init, cond, iter, body);
    
    stmt->meta = meta;
    return stmt;
}

StmtAST::Ptr Parser::parseWhileLoopStmt() {
    bool postCondition = false;
    ExprAST::Ptr condition = nullptr;
    StmtAST::Ptr body = nullptr;
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
    StmtAST::Ptr stmt = make_unique<WhileLoopAST>(condition, body, postCondition);
    stmt->meta = meta;
    return stmt;
}

StmtAST::Ptr Parser::parseBlockStmt() {
    if (check(LBRACE)) {
        Token &opening = peek(-1); // '{' position for error reporting 
        StmtAST::Vec body;
        while (!check(RBRACE)) 
            if (!notEOF()) {
                error->refError(ParseError::MISSING_PAREN, "Code block was never closed", error->getMeta(fileName),
                    {error->createUL(peek(-1).line, peek(-1).end + 1, peek(-1).end + 1, 0, "Expected a closing paren (RBRACE '}') here"),
                        error->createHelp(peek(-1).line, "You may have not closed a code block or array inside this code block")},
                    error->getMeta(fileName),
                    {error->createUL(opening.line, opening.start, opening.end, 0, "Opening paren found here (LBRACE '{')")});
                break;
            } else
                body.push_back(parseStmt());
        StmtAST::Ptr stmt = make_unique<CodeBlockAST>(body);
        stmt->meta = Metadata(fileName, opening);
        stmt->meta.copyEnd(peek(-1));
        return stmt;
    } 
    
    return parseIfElseStmt();
}

StmtAST::Ptr Parser::parseIfElseStmt() {
    if (check(KEY_IF)) {
        Metadata meta = Metadata(fileName, peek(-1));

        eat(LPAREN);
        ExprAST::Ptr condition = parseExpr();
        eat(RPAREN, ParseError::MISSING_PAREN);
        
        StmtAST::Ptr thenBody = parseStmt(); 

        StmtAST::Ptr stmt;
        if (check(KEY_ELSE)) {
            StmtAST::Ptr elseBody = parseStmt(); 
            meta.copyEnd(elseBody->meta);
            stmt = make_unique<IfElseAST>(condition, thenBody, elseBody);
        } else {
            meta.copyEnd(thenBody->meta);
            stmt = make_unique<IfElseAST>(condition, thenBody);
        }
        
        stmt->meta = meta;
        return stmt;
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
    
    Metadata meta = Metadata(fileName, *current);
    const string symbol = eat().value; // get value from identifier
    FuxType type = parseType();

    if (check(TRIPLE_EQUALS)) // ===
        type.access.push_back(FuxType::CONSTANT);
    else if (!check(EQUALS)) {
        StmtAST::Ptr decl = make_unique<VariableDeclAST>(symbol, type);
        meta.copyEnd(peek(-1));
        decl->meta = meta;

        if (parent) {
            parent->addLocal(decl);
            ExprAST::Ptr expr = make_unique<VariableExprAST>(symbol);
            expr->meta = meta;
            return expr;
        }

        return decl;
    }

    ExprAST::Ptr value = parseExpr();

    if (parent) {
        StmtAST::Ptr decl = make_unique<VariableDeclAST>(symbol, type);
        meta.copyEnd(value->meta); 
        decl->meta = meta;
        parent->addLocal(decl);

        bool constant = find(type.access.begin(), type.access.end(), FuxType::CONSTANT) != type.access.end();
        ExprAST::Ptr ref = make_unique<VariableExprAST>(symbol);
        ExprAST::Ptr expr = make_unique<BinaryExprAST>(constant ? BinaryOp::CONASG : BinaryOp::ASG, ref, value);
        expr->meta = meta;
        return expr;
    }

    meta.copyEnd(value->meta);
    StmtAST::Ptr stmt = make_unique<VariableDeclAST>(symbol, type, value);
    stmt->meta = meta;
    return stmt;
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
        Token &op = eat();
        ExprAST::Ptr expr = parseDereferenceExpr();
        return make_unique<UnaryExprAST>(op, expr);
    }

    return parseDereferenceExpr(); 
}

ExprAST::Ptr Parser::parseDereferenceExpr() { 
    if (*current == ASTERISK) {
        Token &op = eat();
        ExprAST::Ptr expr = parseDereferenceExpr();
        return make_unique<UnaryExprAST>(op, expr);
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

    // Pointer Types will be parsed as dereference expressions.
    // The analyser has to translate these to primitive types. 
    // I don't know wether it is possible to filter these while parsing.
    // e.g.: *(*(i64)) ---Analyser--> **i64  
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
                    endExpr = make_unique<NullExprAST>();
                    endExpr->meta.copyWhole(endTok);
                }

                beginExpr = make_unique<RangeExprAST>(beginExpr, endExpr);
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
            expr->enclosed = true;
            eat(RPAREN, ParseError::MISSING_PAREN);
            return expr;
        case LBRACE: {
            ExprAST::Vec elements = parseExprList(RBRACE);
            eat(RBRACE, ParseError::MISSING_PAREN);
            return make_unique<ArrayExprAST>(elements);
        }
        case TRIPLE_DOT:
            expr = make_unique<VariadicExprAST>();
            expr->meta = Metadata(fileName, that);
            return expr;
        case _EOF: 
            createError(ParseError::UNEXPECTED_EOF, "Unexpected EOF while parsing Primary Expression",
                that, "Expected a primay expression here", 0, "", false, true);
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

MacroAST::Case *Parser::parseMacroCase() {
    eat(LPAREN);
    Metadata meta = Metadata(fileName, peek(-1));
    MacroAST::Arg::Vec args = {};
    
    do {
        if (*current == RPAREN)
            break;
        args.push_back(parseMacroArg());
    } while (check(COMMA));
    
    eat(RPAREN, ParseError::MISSING_PAREN);
    
    eat(POINTER);

    StmtAST::Ptr ret = parseStmt();
    meta.copyEnd(ret->meta);
    MacroAST::Case *that = new MacroAST::Case(args, ret);
    that->meta = meta;
    return that;
}

MacroAST::Arg Parser::parseMacroArg() {
    if (check(ASTERISK)) {
        MacroAST::Arg that("*", MacroAST::WILDCARD);
        that.meta = Metadata(fileName, peek(-1));
        return that;
    }

    if (check(TRIPLE_DOT)) {
        MacroAST::Arg that("...", MacroAST::NONE);
        that.meta = Metadata(fileName, peek(-1));
        return that;
    }

    Token &symbol = eat(IDENTIFIER);
    MacroAST::Arg that(symbol.value, MacroAST::NONE);
    eat(COLON);

    Token &id = eat(IDENTIFIER);
    if (id.value == "type")
        that.type = MacroAST::TYPE;
    else if (id.value == "ident")
        that.type = MacroAST::IDENT;
    else if (id.value == "expr")
        that.type = MacroAST::EXPR;
    else if (id.value == "stmt")
        that.type = MacroAST::STMT;
    else if (id.value == "block")
        that.type = MacroAST::BLOCK;
    else
        error->plainError(ParseError::ILLEGAL_TYPE, "Expected macro parameter type", fileName,
            {error->createMark(symbol.line, id.line, symbol.start, id.end, 
                "Parsed macro parameter", id.start, 
                "Expected a macro parameter type here", 
                {error->createHelp(id.line, "Possible types are: 'type', 'ident', 'expr', 'stmt' or 'block'")})});

    // symbol: id[]
    that.array = check(ARRAY_BRACKET);

    // symbol: id, ...
    // current = ^ 
    that.variadic = (peek() == TRIPLE_DOT);

    that.meta = Metadata(&fileName, nullptr, symbol.line, id.line, symbol.start, id.end);
    return that;
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
            error->getMeta(fileName), {error->createUL(token.line, token.start, token.end, 0, info)},
            error->getMeta(fileName), {error->createUL(refTok.line, refTok.start, refTok.end, 0, refInfo)},
            warning, aggressive);
}