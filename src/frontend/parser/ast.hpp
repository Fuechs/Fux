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
    AST_NULL_LITERAL,
    AST_NUMERIC_LITERAL,
    AST_IDENTIFIER,
    AST_BINARY_EXPR,
    AST_BINARY_OPERATOR,
    AST_NONE,
};

static const char *NodeTypeString[] = {
    "AST_ROOT",
    "AST_NULL_LITERAL",
    "AST_NUMERIC_LITERAL",
    "AST_IDENTIFIER",
    "AST_BINARY_EXPR",
    "AST_BINARY_OPERATOR",
    "AST_NONE",
};

class AST {
public:
    AST(AST *copy)
    : parent(copy->parent), type(copy->type), line(copy->line), start(copy->line),
    end(copy->end), body(copy->body), value(copy->value) {}

    AST(AST *parent, NodeType type, size_t line = 1, size_t start = 1, size_t end = 1)
    : parent(parent), type(type), line(line), start(start), end(end), body({}), value("none") {}

    AST(AST *parent, NodeType type, size_t line, size_t start, size_t end, string value)
    : parent(parent), type(type), line(line), start(start), end(end), body({}), value(value) {}

    AST(AST *parent, NodeType type, Token token)
    : parent(parent), type(type), line(token.line), start(token.start), 
    end(token.end), body({}), value(token.value) {}

    ~AST() {
        value.clear();
        for (AST *sub : body)
            delete sub;
        body.clear();
    }

    // void operator=(AST* copy) { 
    //     parent = copy->parent;
    //     type = copy->type;
    //     line = copy->line;
    //     start = copy->start;
    //     end = copy->end;
    //     body = copy->body;
    //     value = copy->value;
    // }

    // shorthand for pushing sub asts
    void addSub(AST *sub);
    void copyPosition(AST *from);
    void debugPrint(size_t indent = 0, bool all = true);
    void debugIndent(stringstream &debug, size_t indent);
    void debugLiteral();
    
    AST *parent;
    NodeType type;  
    size_t line, start, end;
    vector<AST *> body;
    string value;
};

// typedef vector<AST *> AstList;
// typedef vector<AstList> AstLists;