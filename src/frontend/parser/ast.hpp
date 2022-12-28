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

typedef map<string, Value *> ValueMap;
typedef map<string, fuxType::Type> ArgMap;

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
    virtual Value *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) = 0;
    virtual void debugPrint() = 0;

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
        
    void addSub(ExprPtr &sub);
    ExprList getProg();
    
    Value *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) override;
    void debugPrint() override;
    
    Position pos = Position();
};

class NumberExprAST : public ExprAST {
    fuxType::Type type;
    double value;

public:
    NumberExprAST(fuxType::Type type, double value) : type(type), value(value) {}

    Value *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) override;
    void debugPrint() override;
  
    Position pos = Position();
};

class VariableExprAST : public ExprAST {
    string name;

public:
    VariableExprAST(const string& name) : name(name) {}
    ~VariableExprAST() override { name.clear(); }

    Value *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) override;
    void debugPrint() override;

    Position pos = Position();
};

class BinaryExprAST : public ExprAST {
    char op;
    ExprPtr LHS, RHS;

public:
    BinaryExprAST(char op, ExprPtr &LHS, ExprPtr &RHS) 
    : op(op), LHS(move(LHS)), RHS(move(RHS)) {}

    Value *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) override;
    void debugPrint() override;

    Position pos = Position();
};

class ComparisonExprAST : public ExprAST {
    char comp;
    ExprPtr LHS, RHS;

public:
    ComparisonExprAST(char comp, ExprPtr &LHS, ExprPtr &RHS)
    : comp(comp), LHS(move(LHS)), RHS(move(RHS)) {}

    Value *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) override;
    void debugPrint() override; 

    Position pos = Position();
};

class LogicalExprAST : public ExprAST {
    char logical;
    ExprPtr LHS, RHS;

public:
    LogicalExprAST(char logical, ExprPtr &LHS, ExprPtr &RHS)
    : logical(logical), LHS(move(LHS)), RHS(move(RHS)) {}

    Value *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) override;
    void debugPrint() override; 

    Position pos = Position();
};

class CallExprAST : public ExprAST {
    string callee;
    ExprList args;

public:
    CallExprAST(const string &callee, ExprList &args)
    : callee(callee), args(move(args)) {}
    ~CallExprAST() override { callee.clear(); }

    Value *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) override;
    void debugPrint() override;

    Position pos = Position();
};

class VariableDeclAST : public ExprAST {
    string symbol;
    fuxType::Type type;
    ExprPtr value;

public:
    VariableDeclAST(string symbol, fuxType::Type type = fuxType::NO_TYPE, ExprPtr &value = nullExpr) 
    : symbol(symbol), type(type), value(move(value)) {}
    ~VariableDeclAST() { 
        symbol.clear(); 
        delete &value; 
    }

    string getSymbol();
    fuxType::Type getType();
    ExprPtr &getValue();
    
    Value *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) override;
    void debugPrint() override;

    Position pos = Position();
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
    ~PrototypeAST() override;
    
    string getName();
    ArgMap getArgs();
    fuxType::Type getType();
    
    Function *codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) override;
    void debugPrint() override;

    Position pos = Position();
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
    void debugPrint() override;

    Position pos = Position();
};