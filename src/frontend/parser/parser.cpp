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
    tokens = lexer->lex();
    
    if (fux.options.debugMode)
        for (Token token : tokens)
            token.debugPrint();

    return new AST();
}