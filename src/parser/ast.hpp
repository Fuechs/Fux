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
#include <memory>
// #include <iostream>

namespace fux {

    using 
        std::vector, 
        std::string,
        std::unique_ptr,
        std::move
    ;

    class ExprAST {
    // public:
        // virtual ~ExprAST() {}
    };

    class NumberExprAST : public ExprAST {
    public:
        NumberExprAST(double value) : value(value) {}
    private:
        double value;
    };

    class VariableExprAST : public ExprAST {
    public:
        VariableExprAST(const string &name) : name(name) {}
    private:
        string name;
    };

    class BinaryExprAST : public ExprAST {
    public:
        BinaryExprAST(
            char op,
            unique_ptr<ExprAST> LHS,
            unique_ptr<ExprAST> RHS
        ) : op(op), LHS(move(LHS)), RHS(move(RHS)) {}
    private:
        char op;
        unique_ptr<ExprAST> LHS, RHS;
    };

    class CallExprAST : public ExprAST {
    public:
        CallExprAST(
            const string &callee,
            vector<unique_ptr<ExprAST>> args
        ) : callee(callee), args(move(args)) {}
    private:
        string callee;
        vector<unique_ptr<ExprAST>> args;
    };

    class PrototypeAST {
    public:
        PrototypeAST(
            const string &name, 
            vector<string> args
        ) : name(name), args(std::move(args)) {}

        const string &getName() const { return name; }

    private:
        string name;
        vector<string> args;
    };

    class FunctionAST {

    public:
        FunctionAST(
            unique_ptr<PrototypeAST> proto,
            unique_ptr<ExprAST> body
        ) : proto(move(proto)), body(move(body)) {}
    
    private:
        unique_ptr<PrototypeAST> proto;
        unique_ptr<ExprAST> body;
    };
    
}