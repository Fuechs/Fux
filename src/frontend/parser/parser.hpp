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
    Parser(ErrorManager *error, string fileName, bool mainFile = false) 
    : error(error) {
        lexer = new Lexer(readFile(fileName), fileName, error);
        if (mainFile)
            fux.options.fileLines = lexer->getLines();
    }

    ~Parser() {
        delete lexer;
        for (Token &token : tokens)
            delete &token;
        tokens.clear();
    }

    AST *parse();

private:
    TokenList tokens;
    ErrorManager *error;
    Lexer *lexer;

    bool notEOF() {
        return tokens[0].type != EOF;
    }
};