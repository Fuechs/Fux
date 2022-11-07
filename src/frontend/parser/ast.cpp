/**
 * @file ast.cpp
 * @author fuechs
 * @brief fux abstract syntax tree 
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "ast.hpp"

void AST::operator=(AST &cpy) {
    this->type = cpy.type;
    this->line = cpy.line;
    this->col = cpy.col;
    this->body = cpy.body;
    this->tokens = cpy.tokens;
}

AST *AST::encapsulate(AstType at) {
    AST *branch = new AST(*this);
    branch->type = at;

    delete &body;
    delete &tokens;

    body.push_back(branch);
    return branch;
}