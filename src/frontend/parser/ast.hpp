/**
 * @file ast.hpp
 * @author fuechs
 * @brief fux abstract syntax tree header
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../lexer/token.hpp"
#include "type.hpp"

#include "../../backend/llvmheader.hpp"
#include "../../backend/generator/wrapper.hpp"

// exact position of an AST for error tracking
// TODO: implement in error and parser
struct Position {
    Position(size_t lStart = 1, size_t lEnd = 1, size_t colStart = 1, size_t colEnd = 1)
    : lStart(lStart), lEnd(lEnd), colStart(colStart), colEnd(colEnd) {}

    Position &operator=(Position pos);

    size_t lStart; // first line
    size_t lEnd; // last line
    size_t colStart; // first column of first line
    size_t colEnd; // last column of last line
};

class ExprAST {
public:
    virtual ~ExprAST() {}
    virtual Value *codegen(LLVMWrapper *fuxLLVM) = 0;
    virtual unique_ptr<ExprAST> analyse() = 0;
    virtual void debugPrint() = 0;
    // root functions
    virtual vector<unique_ptr<ExprAST>> getProg() = 0;
    virtual void addSub(unique_ptr<ExprAST> &sub) = 0;

    ExprAST &operator=(ExprAST &ast);

    Position pos = Position();
};

typedef unique_ptr<ExprAST> ExprPtr;
typedef vector<ExprPtr> ExprList;

extern ExprPtr nullExpr;

class RootAST : public ExprAST {
    ExprList program;

public:
    RootAST() : program(ExprList()) {}
        
    
    Value *codegen(LLVMWrapper *fuxLLVM) override;
    ExprPtr analyse() override;
    void debugPrint() override;
    ExprList getProg() override;
    void addSub(ExprPtr &sub) override;
    
    Position pos = Position();
};

class NumberExprAST : public ExprAST {
    FuxType type;
    double value;

public:
    NumberExprAST(FuxType type, double value) : type(type), value(value) {}

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    ExprPtr analyse() override;
    void debugPrint() override;
    
    // unused
    ExprList getProg() override;
    void addSub(ExprPtr &sub) override;

    Position pos = Position();
};

class VariableExprAST : public ExprAST {
    string name;

public:
    VariableExprAST(const string& name) : name(name) {}
    ~VariableExprAST() override { name.clear(); }

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    ExprPtr analyse() override;
    void debugPrint() override;

    // unused
    ExprList getProg() override;
    void addSub(ExprPtr &sub) override;

    Position pos = Position();
};

class BinaryExprAST : public ExprAST {
    char op;
    ExprPtr LHS, RHS;

public:
    BinaryExprAST(char op, ExprPtr &LHS, ExprPtr &RHS) 
    : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    ExprPtr analyse() override;
    void debugPrint() override;

    // unused
    ExprList getProg() override;
    void addSub(ExprPtr &sub) override;

    Position pos = Position();
};

class ComparisonExprAST : public ExprAST {
    char comp;
    ExprPtr LHS, RHS;

public:
    ComparisonExprAST(char comp, ExprPtr &LHS, ExprPtr &RHS)
    : comp(comp), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    ExprPtr analyse() override;
    void debugPrint() override; 

    // unused
    ExprList getProg() override;
    void addSub(ExprPtr &sub) override;

    Position pos = Position();
};

class LogicalExprAST : public ExprAST {
    char logical;
    ExprPtr LHS, RHS;

public:
    LogicalExprAST(char logical, ExprPtr &LHS, ExprPtr &RHS)
    : logical(logical), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    ExprPtr analyse() override;
    void debugPrint() override; 

    // unused
    ExprList getProg() override;
    void addSub(ExprPtr &sub) override;
    
    Position pos = Position();
};

class CallExprAST : public ExprAST {
    string callee;
    ExprList args;

public:
    CallExprAST(const string &callee, ExprList &args)
    : callee(callee), args(std::move(args)) {}
    ~CallExprAST() override { callee.clear(); }

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    ExprPtr analyse() override;
    void debugPrint() override;

    // unused
    ExprList getProg() override;
    void addSub(ExprPtr &sub) override;
   
   Position pos = Position();
};

class VariableDeclAST : public ExprAST {
    string symbol;
    FuxType type;
    ExprPtr value;

public:
    VariableDeclAST(string symbol, FuxType type = FuxType(), ExprPtr &value = nullExpr) 
    : symbol(symbol), type(type), value(std::move(value)) {}
    ~VariableDeclAST() { 
        symbol.clear(); 
        delete &value; 
    }
    
    Value *codegen(LLVMWrapper *fuxLLVM) override;
    ExprPtr analyse() override;
    void debugPrint() override;

    // unused
    ExprList getProg() override;
    void addSub(ExprPtr &sub) override;
    
    Position pos = Position();
};

class PutsCallAST : public ExprAST {
    ExprPtr argument;

public:
    PutsCallAST(ExprPtr &argument) : argument(std::move(argument)) {}
    ~PutsCallAST() { delete &argument; }

    ExprPtr &getArgument();

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    ExprPtr analyse() override;
    void debugPrint() override;

    // unused
    ExprList getProg() override;
    void addSub(ExprPtr &sub) override;
    
    Position pos = Position();
};

class IfElseAST : public ExprAST {
    ExprPtr condition;
    ExprPtr thenBody;
    ExprPtr elseBody;

public:
    IfElseAST(ExprPtr &condition, ExprPtr &thenBody, ExprPtr &elseBody = nullExpr)
    : condition(std::move(condition)), thenBody(std::move(thenBody)), elseBody(std::move(elseBody)) {}

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    ExprPtr analyse() override;
    void debugPrint() override;

    // unused
    ExprList getProg() override;
    void addSub(ExprPtr &sub) override;

    Position pos = Position();
};

// prototype of a function
// name and arguments
class PrototypeAST : public ExprAST {
    FuxType type;
    string name;
    ArgMap args;

public:
    PrototypeAST(FuxType type, const string &name, ArgMap args)
    : type(type), name(name), args(args) {}
    ~PrototypeAST() override;
    
    string getName();
    ArgMap getArgs();
    FuxType getType();
    
    Function *codegen(LLVMWrapper *fuxLLVM) override;
    ExprPtr analyse() override;
    void debugPrint() override;

    // unused
    ExprList getProg() override;
    void addSub(ExprPtr &sub) override;
    
    Position pos = Position();
};

typedef unique_ptr<PrototypeAST> ProtoPtr;

class FunctionAST : public ExprAST {
    ProtoPtr proto;
    ExprList body;

public:
    FunctionAST(FuxType type, const string &name, ArgMap args, ExprList &body)
    : proto(make_unique<PrototypeAST>(type, name, args)), body(std::move(body)) {}

    FunctionAST(ProtoPtr proto, ExprList &body)
    : proto(std::move(proto)), body(std::move(body)) {}

    Function *codegen(LLVMWrapper *fuxLLVM) override;
    ExprPtr analyse() override;
    void debugPrint() override;

    // unused
    ExprList getProg() override;
    void addSub(ExprPtr &sub) override;
    
    Position pos = Position();
};