/**
 * @file ast.hpp
 * @author fuechs
 * @brief fux abstract syntax tree header
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../lexer/token.hpp"
// #include "../error/error.hpp"

enum AstType {
    AST_NONE,
};

class AST {
public:
    AST(AstType type = AST_NONE, size_t line = 1, size_t col = 1)
    : type(type), line(line), col(col), body({}), tokens({}) {}

    AST(AST &cpy) { operator=(cpy); }

    ~AST();

    void operator=(AST &cpy);

    AST *encapsulate(AstType at);

    AstType type;
    size_t line, col;
    AstList body;
    TokenList tokens;
};

typedef vector<AST *> AstList;