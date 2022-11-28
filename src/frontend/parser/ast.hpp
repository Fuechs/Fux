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
#include "type.hpp"

enum NodeType {
    // * statements
    AST_ROOT,
    AST_VARIABLE_DECl,

    // * expressions
    AST_NULL_LITERAL,
    AST_NUMERIC_LITERAL,
    AST_IDENTIFIER,
    AST_BINARY_EXPR,
    AST_BINARY_OPERATOR,
    AST_TYPE,

    AST_NONE,
};

static const char *NodeTypeString[] = {
    "AST_ROOT",
    "AST_VARIABLE_DECL",

    "AST_NULL_LITERAL",
    "AST_NUMERIC_LITERAL",
    "AST_IDENTIFIER",
    "AST_BINARY_EXPR",
    "AST_BINARY_OPERATOR",
    "AST_TYPE",

    "AST_NONE",
};

/**
 * @brief exact position of an AST (Node) for error tracking
 * 
 */
struct Position {
    Position(size_t first = 1, size_t last = 1, size_t start = 1, size_t end = 1)
    : first(first), last(last), start(start), end(end) {}

    // line the AST starts 
    size_t first;
    // line the AST ends
    size_t last;
    // column the AST starts 
    size_t start;
    // column the AST ends 
    size_t end; 
};

class AST {
public:
    AST(AST *copy)
    : parent(copy->parent), type(copy->type), pos(copy->pos), body(copy->body), value(copy->value) { }

    AST(AST *parent, NodeType type, Position pos = Position(), string value = "none")
    : parent(parent), type(type), pos(pos), body({}), value(value) {}

    AST(AST *parent, NodeType type, Position& pos, string& value)
    : parent(parent), type(type), pos(pos), body({}), value(value) {}

    AST(AST *parent, NodeType type, Token token)
    : parent(parent), type(type), body({}), value(token.value) {
        pos.first = token.line;
        pos.last = token.line;
        pos.start = token.start;
        pos.end = token.end;
    }

    ~AST() {
        value.clear();
        for (AST *sub : body)
            delete sub;
        body.clear();
    }

    AST *operator[](size_t index) {
        return body[index];
    }

    // shorthand for pushing sub asts
    void addSub(AST *sub);
    // copy position from another AST
    void copyPosition(AST *from);
    // copy position from a Token
    void copyPosition(Token from);
    // print out debug info about the AST and all sub ASTs
    void debugPrint(size_t indent = 0, bool all = true);
    // helper function for debugIndent
    void debugIndent(stringstream &debug, size_t indent);
    // print out literal from AST
    // (+ 1 1) --> (1 + 1)
    void debugLiteral();
    
    AST *parent;
    NodeType type;  
    Position pos;
    vector<AST *> body;
    union { 
        string value; 
        fuxType::Type valueType; 
    };
};