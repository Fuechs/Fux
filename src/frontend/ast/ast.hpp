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
#include "../../backend/llvmheader.hpp"
#include "../../backend/generator/wrapper.hpp"
#include "../lexer/token.hpp"
#include "../parser/type.hpp"
#include "../parser/value.hpp"
#include "position.hpp"
#include "expr.hpp"

typedef map<string, FuxType> ArgMap;

/// EXPRESSIONS ///

class NumberExprAST : public ExprAST {
    ValueStruct *value;

public:
    template<typename T>
    NumberExprAST(T value) : value(new ValueStruct(value)) {}
    ~NumberExprAST();

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    StmtPtr analyse() override;
    void debugPrint() override;
    
    Position pos = Position();
};

class VariableExprAST : public ExprAST {
    string name;

public:
    VariableExprAST(const string& name) : name(name) {}
    ~VariableExprAST() override { name.clear(); }

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    StmtPtr analyse() override;
    void debugPrint() override;

    Position pos = Position();
};

class BinaryExprAST : public ExprAST {
    char op;
    ExprPtr LHS, RHS;

public:
    BinaryExprAST(char op, ExprPtr &LHS, ExprPtr &RHS) 
    : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    StmtPtr analyse() override;
    void debugPrint() override;

    Position pos = Position();
};

class ComparisonExprAST : public ExprAST {
    TokenType comp;
    ExprPtr LHS, RHS;

public:
    ComparisonExprAST(TokenType comp, ExprPtr &LHS, ExprPtr &RHS)
    : comp(comp), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    StmtPtr analyse() override;
    void debugPrint() override; 

    Position pos = Position();
};

class LogicalExprAST : public ExprAST {
    TokenType logical;
    ExprPtr LHS, RHS;

public:
    LogicalExprAST(TokenType logical, ExprPtr &LHS, ExprPtr &RHS)
    : logical(logical), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    StmtPtr analyse() override;
    void debugPrint() override; 

    Position pos = Position();
};

class CallExprAST : public ExprAST {
    string callee;
    ExprList args;

public:
    CallExprAST(const string &callee, ExprList &args)
    : callee(callee), args(std::move(args)) {}
    ~CallExprAST() override;

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    StmtPtr analyse() override;
    void debugPrint() override;
   
    Position pos = Position();
};

class AssignmentExprAST : public ExprAST {
    ExprPtr dest;
    ExprPtr value;
    bool constant;

public:
    AssignmentExprAST(ExprPtr &dest, ExprPtr &value, const bool constant = false)
    : dest(std::move(dest)), value(std::move(value)), constant(constant) {}

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    StmtPtr analyse() override;
    void debugPrint() override;

    Position pos = Position();
};

/// STATEMENTS ///

class VariableDeclAST : public StmtAST {
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
    StmtPtr analyse() override;
    void debugPrint() override;
    
    Position pos = Position();
};

class PutsCallAST : public StmtAST {
    ExprPtr argument;

public:
    PutsCallAST(ExprPtr &argument) : argument(std::move(argument)) {}
    ~PutsCallAST() { delete &argument; }

    ExprPtr &getArgument();

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    StmtPtr analyse() override;
    void debugPrint() override;
    
    Position pos = Position();
};

class ReturnCallAST : public StmtAST {
    ExprPtr value;

public:
    ReturnCallAST(ExprPtr &value) : value(std::move(value)) {}

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    StmtPtr analyse() override;
    void debugPrint() override;

    Position pos = Position();
};

class IfElseAST : public StmtAST {
    ExprPtr condition;
    StmtPtr thenBody;
    StmtPtr elseBody;

public:
    IfElseAST(ExprPtr &condition, StmtPtr &thenBody, StmtPtr &elseBody = nullStmt)
    : condition(std::move(condition)), thenBody(std::move(thenBody)), elseBody(std::move(elseBody)) {}

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    StmtPtr analyse() override;
    void debugPrint() override;

    Position pos = Position();
};

class CodeBlockAST : public StmtAST {
    StmtList body;

public:
    CodeBlockAST() : body(StmtList()) {}
    CodeBlockAST(StmtList &body) : body(std::move(body)) {}

    Value *codegen(LLVMWrapper *fuxLLVM) override;
    StmtPtr analyse() override;
    void debugPrint() override;

    void addSub(StmtPtr &sub);

    Position pos = Position();
};

typedef unique_ptr<CodeBlockAST> BlockPtr;

// prototype of a function
// name and arguments
class PrototypeAST : public StmtAST {
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
    StmtPtr analyse() override;
    void debugPrint() override;
    
    Position pos = Position();
};

typedef unique_ptr<PrototypeAST> ProtoPtr;

class FunctionAST : public StmtAST {
    ProtoPtr proto;
    BlockPtr body;

public:
    FunctionAST(FuxType type, const string &name, ArgMap args, BlockPtr &body)
    : proto(make_unique<PrototypeAST>(type, name, args)), body(std::move(body)) {}

    FunctionAST(ProtoPtr proto, BlockPtr &body)
    : proto(std::move(proto)), body(std::move(body)) {}

    Function *codegen(LLVMWrapper *fuxLLVM) override;
    StmtPtr analyse() override;
    void debugPrint() override;    

    Position pos = Position();
};


class RootAST : public StmtAST {
    StmtList program;

public:
    RootAST() : program(StmtList()) {}        
    
    Value *codegen(LLVMWrapper *fuxLLVM) override;
    StmtPtr analyse() override;
    void debugPrint() override;

    void addSub(StmtPtr &sub);
    
    Position pos = Position();
};

typedef unique_ptr<RootAST> RootPtr;