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

#include "../../backend/llvmheader.hpp"

#include <llvm/IR/Value.h>

typedef map<string, Value *> ValueMap;
typedef map<string, fuxType::Type> ArgMap;

class ExprAST {
public:
    virtual ~ExprAST() {}
    virtual Value *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) = 0;
};

typedef unique_ptr<ExprAST> ExprPtr;
typedef vector<ExprPtr> ExprList;

class RootAST : public ExprAST {
    ExprList program;

public:
    RootAST() : program(ExprList()) {}

    void addSub(ExprPtr &sub);
    
    Value *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) override;
};

class NumberExprAST : public ExprAST {
    fuxType::Type type;
    double value;

public:
    NumberExprAST(fuxType::Type type, double value) : type(type), value(value) {}

    Value *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) override;
};

class VariableExprAST : public ExprAST {
    string name;

public:
    VariableExprAST(const string& name) : name(name) {}
    ~VariableExprAST() override { name.clear(); }

    Value *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) override;
};

class BinaryExprAST : public ExprAST {
    char op;
    ExprPtr LHS, RHS;

public:
    BinaryExprAST(char op, ExprPtr &LHS, ExprPtr &RHS) 
    : op(op), LHS(move(LHS)), RHS(move(RHS)) {}

    Value *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) override;
};

class CallExprAST : public ExprAST {
    string callee;
    ExprList args;

public:
    CallExprAST(const string &callee, ExprList &args)
    : callee(callee), args(move(args)) {}
    ~CallExprAST() override { callee.clear(); }

    Value *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) override;
};

// prototype of a function
// name and arguments
class PrototypeAST : public ExprAST {
    fuxType::Type type;
    string name;
    ArgMap args;

public:
    PrototypeAST(fuxType::Type type, const string &name, ArgMap args)
    : type(type), name(name), args(args) {}
    ~PrototypeAST() override { name.clear(); }

    Function *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) override;

    string getName() { return name; }
};

typedef unique_ptr<PrototypeAST> ProtoPtr;

class FunctionAST : public ExprAST {
    ProtoPtr proto;
    ExprList body;

public:
    FunctionAST(fuxType::Type type, const string &name, ArgMap args, ExprList &body)
    : proto(make_unique<PrototypeAST>(type, name, args)), body(move(body)) {}

    FunctionAST(ProtoPtr proto, ExprList &body)
    : proto(move(proto)), body(move(body)) {}

    Function *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) override;
};