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
    void debugPrint(size_t indent = 0) override;
    
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
    void debugPrint(size_t indent = 0) override;
    
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
    void debugPrint(size_t indent = 0) override;
     
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
    void debugPrint(size_t indent = 0) override;
 
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
    void debugPrint(size_t indent = 0) override;
 
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
    void debugPrint(size_t indent = 0) override;
 
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
    void debugPrint(size_t indent = 0) override;
 
    Position pos = Position();
};

class CallExprAST : public ExprAST {
    ExprPtr callee;
    ExprList args;

public:
    CallExprAST(const string &callee, ExprList &args)
    : callee(make_unique<VariableExprAST>(callee)), args(std::move(args)) {}
    CallExprAST(ExprPtr &callee, ExprList &args)
    : callee(std::move(callee)), args(std::move(args)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override; 
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
    
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
    void debugPrint(size_t indent = 0) override;
 
    Position pos = Position();
};

class BinaryExprAST : public ExprAST {
    BinaryOp op;
    ExprPtr LHS, RHS;

public:
    BinaryExprAST(BinaryOp op, ExprPtr &LHS, ExprPtr &RHS = nullExpr) 
    : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;   
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
 
    Position pos = Position();
};

class TypeCastExprAST : public ExprAST {
    FuxType type;
    ExprPtr expr;

public:
    TypeCastExprAST(FuxType type, ExprPtr &expr) 
    : type(type), expr(std::move(expr)) {}
    
    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
    
    Position pos = Position();
};

class TernaryExprAST : public ExprAST {
    ExprPtr condition;
    ExprPtr thenExpr;
    ExprPtr elseExpr;

public:
    TernaryExprAST(ExprPtr &condition, ExprPtr &thenExpr, ExprPtr &elseExpr)
    : condition(std::move(condition)), thenExpr(std::move(thenExpr)), elseExpr(std::move(elseExpr)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override; 
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
    
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
    void debugPrint(size_t indent = 0) override;
     
    Position pos = Position();
};

typedef unique_ptr<VariableDeclAST> VarDeclPtr; 

class InbuiltCallAST : public StmtAST {
    Inbuilts callee;
    ExprList arguments;

public:
    InbuiltCallAST(Inbuilts callee, ExprList &arguments) 
    : callee(callee), arguments(std::move(arguments)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
     
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
    void debugPrint(size_t indent = 0) override;
 
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
    void debugPrint(size_t indent = 0) override;
 
    void addSub(StmtPtr &sub);

    Position pos = Position();
};

class WhileLoopAST : public StmtAST {
    ExprPtr condition;
    StmtPtr body;
    bool postCondition;

public:
    WhileLoopAST(ExprPtr &condition, StmtPtr &body, bool postCondition = false)
    : condition(std::move(condition)), body(std::move(body)), postCondition(postCondition) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
 
    Position pos = Position();
};

class ForLoopAST : public StmtAST {
    bool forEach;
    /*for (*/StmtPtr initial; 
            ExprPtr condition; 
            ExprPtr iterator;//) {
        StmtPtr body;
    // }
public:
    ForLoopAST(StmtPtr &initial, ExprPtr &iterator, StmtPtr &body)
    : forEach(true), initial(std::move(initial)), condition(nullptr), 
        iterator(std::move(iterator)), body(std::move(body)) {}
    ForLoopAST(StmtPtr &initial, ExprPtr &condition, ExprPtr &iterator, StmtPtr &body)
    : forEach(false), initial(std::move(initial)), condition(std::move(condition)),
        iterator(std::move(iterator)), body(std::move(body)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
 
    Position pos = Position();
};

typedef unique_ptr<CodeBlockAST> BlockPtr;

// prototype of a function
// name and arguments
class PrototypeAST : public StmtAST {
    FuxType type;
    ExprPtr symbol;
    StmtList args;

public:
    PrototypeAST(FuxType type, const string &symbol, StmtList &args)
    : type(type), symbol(make_unique<VariableExprAST>(symbol)), args(std::move(args)) {}
    PrototypeAST(FuxType type, ExprPtr &symbol, StmtList &args)
    : type(type), symbol(std::move(symbol)), args(std::move(args)) {}
    ~PrototypeAST() override;
    
    ExprPtr &getSymbol();
    StmtList &getArgs();
    FuxType &getType();
    
    FUX_BC(Function *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
     
    Position pos = Position();
};

typedef unique_ptr<PrototypeAST> ProtoPtr;

class FunctionAST : public StmtAST {
    ProtoPtr proto;
    StmtPtr body;

public:
    FunctionAST(FuxType type, const string &symbol, StmtList &args, StmtPtr &body)
    : proto(make_unique<PrototypeAST>(type, symbol, args)), body(std::move(body)) {}
    FunctionAST(FuxType type, ExprPtr &symbol, StmtList &args, StmtPtr &body)
    : proto(make_unique<PrototypeAST>(type, symbol, args)), body(std::move(body)) {}
    FunctionAST(ProtoPtr &proto, StmtPtr &body)
    : proto(std::move(proto)), body(std::move(body)) {}

    FUX_BC(Function *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;    

    Position pos = Position();
};

class RootAST : public StmtAST {
    StmtList program;
    // resting place for array size expressions
    // references to these are in FuxTypes
    ExprList arraySizeExprs; 

public:
    RootAST() : program(StmtList()) {}        
    
    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtPtr analyse() override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
 
    void addSub(StmtPtr &sub);
    ExprPtr &addSizeExpr(ExprPtr &sizeExpr);
    
    Position pos = Position();
};

typedef unique_ptr<RootAST> RootPtr;