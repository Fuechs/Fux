/**
 * @file parser.hpp
 * @author fuechs
 * @brief fux parser header
 * @version 0.1
 * @date 2022-11-02
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "ast.hpp"
#include "../error/error.hpp"
#include "../lexer/lexer.hpp"
#include "../lexer/token.hpp"

class Parser {
public:
    Parser(ErrorManager *error, string fileName, string source, bool mainFile = false) 
    : error(error) {
        lexer = new Lexer(source, fileName, error);
        if (mainFile)
            fux.options.fileLines = lexer->getLines();
        root = new AST(nullptr, AST_ROOT);
    }

    ~Parser() {
        delete lexer;
        tokens.clear();
    }

    AST *parse();

private:
    TokenList tokens;
    ErrorManager *error;
    Lexer *lexer;
    AST *root;

    AST *parseStmt(AST *parent);
    AST *parseExpr(AST *parent);
    AST *parseAdditiveExpr(AST *parent);
    AST *parseMultiplicativExpr(AST *parent);
    AST *parsePrimaryExpr(AST *parent);
    
    // int64_t parseNumber(string str);

    Token eat();
    Token expect(TokenType type, ErrorType errType = UNEXPECTED_TOKEN);
    bool notEOF();
};