/**
 * @file ast.hpp
 * @author fuechs 
 * @brief fux ast header
 * @version 0.1
 * @date 2022-09-28
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include <vector>
#include <string>
// #include <iostream>

namespace fux {

    using 
        std::vector, 
        std::string
    ;

    typedef enum BinaryOperator_ENUM {
        ADD,
        SUB,
        MUL,
        DIV,
        NOP,
    } BinaryOperator;

    typedef enum AssignmentOperator_ENUM {
        EQUALS,
        PLUS_EQUALS,
        MINUS_EQUALS,
        // ...
    } AssignmentOperator;

    class Expression {};

    typedef std::shared_ptr<Expression> ExpressionPtr;

    class Statement {
    public:
        Statement(string name = "untitled") : name(name) {}

        string name;
    };

    typedef vector<Statement> StatementList;

    class Block : public Statement {
    public:
        Block(string name = "untitled") : name(name) {}

        string name;
        StatementList stmts;
    };

    class Assignment : public Statement {
    public:
        Assignment(AssignmentOperator aOp, string name, Expression value) 
        : aOp(aOp), name(name), value(value) {}

        AssignmentOperator aOp;
        string name;
        Expression value;

    };

    class BinaryOperation : public Expression {
    public:
        BinaryOperation(BinaryOperator op, ExpressionPtr LHS, ExpressionPtr RHS) 
        : op(op), LHS(LHS), RHS(RHS) {}

        BinaryOperator op = NOP;
        ExpressionPtr LHS {};
        ExpressionPtr RHS {};
    };
    
}