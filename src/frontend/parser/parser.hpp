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
    Parser(TokenList tokens, ErrorManager *error, string fileName, vector<string> lines) 
    : tokens(tokens), error(error) {
        error->setFileName(fileName);
        error->setLines(lines);
    }

    ~Parser();

    AST *parse();

private:
    TokenList tokens;
    ErrorManager *error;

    void parseGet();
};