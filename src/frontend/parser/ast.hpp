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

enum NodeType {
    AST_ROOT,
    AST_NUMERIC_LITERAL,
    AST_IDENTIFIER,
    AST_BINARY_EXPR,
    AST_BINARY_OPERATOR,
    AST_NONE,
};

static const char *NodeTypeString[] = {
    "AST_ROOT",
    "AST_NUMERIC_LITERAL",
    "AST_IDENTIFIER",
    "AST_BINARY_EXPR",
    "AST_BINARY_OPERATOR",
    "AST_NONE",
};

class AST {
public:
    AST(AST *parent, NodeType type = AST_NONE, size_t line = 1, size_t col = 1)
    : parent(parent), type(type), line(line), col(col), body({}), value(string()) {}

    AST(AST *parent, NodeType type, size_t line, size_t col, string value)
    : parent(parent), type(type), line(line), col(col), body({}), value(value) {}

    ~AST() {
        value.clear();
        for (AST *sub : body)
            delete sub;
        body.clear();
    }

    // shorthand for pushing sub asts
    void addSub(AST *sub);
    void debugPrint(size_t indent = 0, bool all = true);
    void debugIndent(stringstream &debug, size_t indent);
    
    AST *parent;
    NodeType type;  
    size_t line, col;
    vector<AST *> body;
    string value;
};

// typedef vector<AST *> AstList;
// typedef vector<AstList> AstLists;