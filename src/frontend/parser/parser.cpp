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
        
    current = tokens.begin();

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
    switch (current->type) {
        case SEMICOLON:         eat(); return parseStmt(parent); // skip semicolon
        case IDENTIFIER:        return parseVarDecl(parent);
        default:                return parseExpr(parent);
    }
}

AST *Parser::parseVarDecl(AST *parent) {
    if ((++current)->type == SEMICOLON) {
        --current;
        Token name = eat();
        eat(); // semicolon
        AST *variable = new AST(parent, AST_VARIABLE_DECl);
        AST *symbol = new AST(variable, AST_IDENTIFIER, name);
        variable->addSub(symbol);
        variable->copyPosition(symbol);
        variable->pos.end++; // semicolon

        return variable;
    }

    if ((++current)->type == COLON) {
        --current;
        Token name = eat();
        eat(); // colon
        AST *variable = new AST(parent, AST_VARIABLE_DECl);
        AST *symbol = new AST(variable, AST_IDENTIFIER, name);
        variable->addSub(symbol);
        variable->copyPosition(symbol);

        AST *varType = parseType(variable);
        if (varType->type != AST_NONE)
            variable->addSub(varType);

        switch (current->type) {
            case EQUALS:            variable->valueType = fuxType::VAR; break;
            case TRIPLE_EQUALS:     variable->valueType = fuxType::CONSTANT; break;
            case SEMICOLON:
                if (varType->type == AST_NONE) {
                    error->createError(UNEXPECTED_TOKEN, *current, "got unexpected SEMICOLON ';' after COLON ':' in variable defenition");
                    return new AST(parent, AST_NONE, eat());
                }
                eat(); // semicolon
                variable->pos.end++;
                return variable;

            default:
                error->createError(UNEXPECTED_TOKEN, *current, "got unexpected "+string(TokenTypeString[current->type])+" '"+current->value+"' after COLON ':' in variable definition");
                return new AST(parent, AST_NONE, eat());
        }
        eat(); 
                
        AST *value = parseExpr(variable);
        variable->addSub(value);
        variable->pos.end = value->pos.end + 1; // semicolon
        expect(SEMICOLON);

        return variable;
    } 
 
    return parseExpr(parent);

}

AST *Parser::parseType(AST *parent) {
    AST *varType = new AST(parent, AST_NONE);
    switch (current->type) {
        case KEY_I32:       varType->valueType = fuxType::I32; break;
        default:            return varType;
    }
    varType->copyPosition(eat());
    return varType;
}

AST *Parser::parseExpr(AST *parent) {
    return parseAdditiveExpr(parent);
}

AST *Parser::parseAdditiveExpr(AST *parent) {
    AST *lhs = parseMultiplicativeExpr(parent);

    while (current->type == PLUS || current->type == MINUS) {
        AST *op = new AST(nullptr, AST_BINARY_OPERATOR, eat());
        AST *rhs = parseMultiplicativeExpr(nullptr);
        AST *copy = new AST(lhs); // copy old lhs
        lhs = new AST(parent, AST_BINARY_EXPR, copy->pos);
        lhs->pos.end = rhs->pos.end;
        // ( op lhs(copy) rhs )
        lhs->addSub(op); op->parent = lhs;
        lhs->addSub(copy); copy->parent = lhs;
        lhs->addSub(rhs); rhs->parent = lhs;
    }

    return lhs;
}

AST *Parser::parseMultiplicativeExpr(AST *parent) {
    AST *lhs = parsePrimaryExpr(parent);

    while (current->type == SLASH || current->type == ASTERISK || current->type == PERCENT) {
        AST *op = new AST(nullptr, AST_BINARY_OPERATOR, eat());
        AST *rhs = parsePrimaryExpr(nullptr);
        AST *copy = new AST(lhs); // copy old lhs
        lhs = new AST(parent, AST_BINARY_EXPR, copy->pos);
        lhs->pos.end = rhs->pos.end;
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

bool Parser::notEOF() { 
    return current->type != _EOF;
}