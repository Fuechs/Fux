/**
 * @file ast.hpp
 * @author fuechs
 * @brief fux abstract syntax tree header
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
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
#include "op.hpp"

typedef map<string, FuxType> ArgMap;

/// EXPRESSIONS ///

class NullExprAST : public ExprAST {
public:    
    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint() override;
    
    Position pos = Position();
};

class BoolExprAST : public ExprAST {
    ValueStruct *value;

public:
    BoolExprAST(bool value) : value(new ValueStruct(value)) {}
    ~BoolExprAST();

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint() override;
    
    Position pos = Position();
};

class NumberExprAST : public ExprAST {
    ValueStruct *value;

public:
    template<typename T>
    NumberExprAST(T value) : value(new ValueStruct(value)) {}
    ~NumberExprAST();

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint() override;
    
    Position pos = Position();
};

class CharExprAST : public ExprAST {
    ValueStruct *value;

public:
    template<typename T>
    CharExprAST(T value) : value(new ValueStruct(value)) {}
    ~CharExprAST();

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint() override;

    Position pos = Position();
};

class StringExprAST : public ExprAST {
    ValueStruct *value;

public:
    StringExprAST(string &value) : value(new ValueStruct(value)) {}
    ~StringExprAST();

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint() override;

    Position pos = Position();
};

class VariableExprAST : public ExprAST {
    string name;

public:
    VariableExprAST(const string& name) : name(name) {}
    ~VariableExprAST() override;

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint() override;

    Position pos = Position();
};

class MemberExprAST : public ExprAST {
    ExprPtr base;
    ExprPtr member;

public:
    MemberExprAST(ExprPtr &base, ExprPtr &member) 
    : base(std::move(base)), member(std::move(member)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint() override;

    Position pos = Position();
};

class UnaryExprAST : public ExprAST {
    UnaryOp op;
    ExprPtr expr;

public:
    UnaryExprAST(UnaryOp op, ExprPtr &expr) : op(op), expr(std::move(expr)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint() override;

    Position pos = Position();
};

class BinaryExprAST : public ExprAST {
    BinaryOp op;
    ExprPtr LHS, RHS;

public:
    BinaryExprAST(BinaryOp op, ExprPtr &LHS, ExprPtr &RHS) 
    : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint() override;

    Position pos = Position();
};

class CallExprAST : public ExprAST {
    string callee;
    StmtList args;

public:
    CallExprAST(const string &callee, StmtList &args)
    : callee(callee), args(std::move(args)) {}
    ~CallExprAST() override;

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
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
    ~VariableDeclAST() override;
    
    string &getSymbol();
    FuxType &getType();
    ExprPtr &getValue();

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint() override;
    
    Position pos = Position();
};

typedef unique_ptr<VariableDeclAST> VarDeclPtr; 

class PutsCallAST : public StmtAST {
    ExprPtr argument;

public:
    PutsCallAST(ExprPtr &argument) : argument(std::move(argument)) {}

    ExprPtr &getArgument();

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint() override;
    
    Position pos = Position();
};

class ReturnCallAST : public StmtAST {
    ExprPtr value;

public:
    ReturnCallAST(ExprPtr &value) : value(std::move(value)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
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

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint() override;

    Position pos = Position();
};

class CodeBlockAST : public StmtAST {
    StmtList body;

public:
    CodeBlockAST() : body(StmtList()) {}
    CodeBlockAST(StmtList &body) : body(std::move(body)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
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
    StmtList args;

public:
    PrototypeAST(FuxType type, const string &name, StmtList &args)
    : type(type), name(name), args(std::move(args)) {}
    ~PrototypeAST() override;
    
    string &getName();
    StmtList &getArgs();
    FuxType &getType();
    
    FUX_BC(Function *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint() override;
    
    Position pos = Position();
};

typedef unique_ptr<PrototypeAST> ProtoPtr;

class FunctionAST : public StmtAST {
    ProtoPtr proto;
    StmtPtr body;

public:
    FunctionAST(FuxType type, const string &name, StmtList &args, StmtPtr &body)
    : proto(make_unique<PrototypeAST>(type, name, args)), body(std::move(body)) {}

    FunctionAST(ProtoPtr &proto, StmtPtr &body)
    : proto(std::move(proto)), body(std::move(body)) {}

    FUX_BC(Function *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint() override;    

    Position pos = Position();
};


class RootAST : public StmtAST {
    StmtList program;

public:
    RootAST() : program(StmtList()) {}        
    
    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint() override;

    void addSub(StmtPtr &sub);
    
    Position pos = Position();
};

typedef unique_ptr<RootAST> RootPtr;