/**
 * @file ast.hpp
 * @author fuechs (fuechsss@gmail.com)
 * @brief fux ast header
 * @version 0.1
 * @date 2022-09-28
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs.
 *            All rights reserved.
 *            BSD 3-Clause License
 * 
 */

#pragma once

#include <vector>
#include <string>
#include <iostream>

namespace fux {

    using std::vector, std::string;

    

    typedef enum ExpressionKind_ENUM {
        INTEGER,
    } ExpressionKind;

    class Expression {
    public:

        ExpressionKind kind;
        string value;
    };

    typedef vector<Expression> ExpressionList;

    typedef enum StatementKind_ENUM {
        DECLARATION,
    } StatementKind;

    class Statement {
    public:

        StatementKind kind;
        Expression expr;
        string value;
    };

    typedef vector<Statement> StatementList;

    class AST {
    public:
        AST(string type) : type(type) {}

        string type;
        StatementList stmts;
    };

}