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
    StmtAST::Ptr analyse(Expectation exp) override;
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
    StmtAST::Ptr analyse(Expectation exp) override;
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
    StmtAST::Ptr analyse(Expectation exp) override;
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
    StmtAST::Ptr analyse(Expectation exp) override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
 
    Position pos = Position();
};

class StringExprAST : public ExprAST {
    ValueStruct *value;

public:
    StringExprAST(string value) : value(new ValueStruct(value)) {}
    ~StringExprAST();

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtAST::Ptr analyse(Expectation exp) override; 
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
    StmtAST::Ptr analyse(Expectation exp) override;  
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
 
    Position pos = Position();
};

class MemberExprAST : public ExprAST {
    ExprAST::Ptr base;
    ExprAST::Ptr member;

public:
    MemberExprAST(ExprAST::Ptr &base, ExprAST::Ptr &member) 
    : base(std::move(base)), member(std::move(member)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtAST::Ptr analyse(Expectation exp) override;    
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
 
    Position pos = Position();
};

class CallExprAST : public ExprAST {
    ExprAST::Ptr callee;
    ExprAST::Vec args;

public:
    CallExprAST(const string &callee, ExprAST::Vec &args)
    : callee(make_unique<VariableExprAST>(callee)), args(std::move(args)) {}
    CallExprAST(ExprAST::Ptr &callee, ExprAST::Vec &args)
    : callee(std::move(callee)), args(std::move(args)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtAST::Ptr analyse(Expectation exp) override; 
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
    
    Position pos = Position();
};

class UnaryExprAST : public ExprAST {
    UnaryOp op;
    ExprAST::Ptr expr;

public:
    UnaryExprAST(UnaryOp op, ExprAST::Ptr &expr) : op(op), expr(std::move(expr)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtAST::Ptr analyse(Expectation exp) override;  
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
 
    Position pos = Position();
};

class BinaryExprAST : public ExprAST {
    BinaryOp op;
    ExprAST::Ptr LHS, RHS;

public:
    BinaryExprAST(BinaryOp op, ExprAST::Ptr &LHS, ExprAST::Ptr &RHS = nullExpr) 
    : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtAST::Ptr analyse(Expectation exp) override;   
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
 
    Position pos = Position();
};

class TypeCastExprAST : public ExprAST {
    FuxType type;
    ExprAST::Ptr expr;

public:
    TypeCastExprAST(FuxType type, ExprAST::Ptr &expr) 
    : type(type), expr(std::move(expr)) {}
    
    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtAST::Ptr analyse(Expectation exp) override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
    
    Position pos = Position();
};

class TernaryExprAST : public ExprAST {
    ExprAST::Ptr condition;
    ExprAST::Ptr thenExpr;
    ExprAST::Ptr elseExpr;

public:
    TernaryExprAST(ExprAST::Ptr &condition, ExprAST::Ptr &thenExpr, ExprAST::Ptr &elseExpr)
    : condition(std::move(condition)), thenExpr(std::move(thenExpr)), elseExpr(std::move(elseExpr)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtAST::Ptr analyse(Expectation exp) override; 
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
    
    Position pos = Position();
};

/// STATEMENTS ///

class VariableDeclAST : public StmtAST {
    string symbol;
    FuxType type;
    ExprAST::Ptr value;

public:
    VariableDeclAST(string symbol, FuxType type = FuxType(), ExprAST::Ptr &value = nullExpr) 
    : symbol(symbol), type(type), value(std::move(value)) {}
    ~VariableDeclAST() override;
    
    string &getSymbol();
    FuxType &getType();
    ExprAST::Ptr &getValue();

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtAST::Ptr analyse(Expectation exp) override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
     
    Position pos = Position();
};

typedef unique_ptr<VariableDeclAST> VarDeclPtr; 

class InbuiltCallAST : public StmtAST {
    Inbuilts callee;
    ExprAST::Vec arguments;

public:
    InbuiltCallAST(Inbuilts callee, ExprAST::Vec &arguments) 
    : callee(callee), arguments(std::move(arguments)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtAST::Ptr analyse(Expectation exp) override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
     
    Position pos = Position();
};

class IfElseAST : public StmtAST {
    ExprAST::Ptr condition;
    StmtAST::Ptr thenBody;
    StmtAST::Ptr elseBody;

public:
    IfElseAST(ExprAST::Ptr &condition, StmtAST::Ptr &thenBody, StmtAST::Ptr &elseBody = nullStmt)
    : condition(std::move(condition)), thenBody(std::move(thenBody)), elseBody(std::move(elseBody)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtAST::Ptr analyse(Expectation exp) override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
 
    Position pos = Position();
};

class CodeBlockAST : public StmtAST {
    StmtAST::Vec body;

public:
    CodeBlockAST() : body(StmtAST::Vec()) {}
    CodeBlockAST(StmtAST::Vec &body) : body(std::move(body)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtAST::Ptr analyse(Expectation exp) override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
 
    void addSub(StmtAST::Ptr &sub);

    Position pos = Position();
};

class WhileLoopAST : public StmtAST {
    ExprAST::Ptr condition;
    StmtAST::Ptr body;
    bool postCondition;

public:
    WhileLoopAST(ExprAST::Ptr &condition, StmtAST::Ptr &body, bool postCondition = false)
    : condition(std::move(condition)), body(std::move(body)), postCondition(postCondition) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtAST::Ptr analyse(Expectation exp) override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
 
    Position pos = Position();
};

class ForLoopAST : public StmtAST {
    bool forEach;
    /*for (*/StmtAST::Ptr initial; 
            ExprAST::Ptr condition; 
            ExprAST::Ptr iterator;//) {
        StmtAST::Ptr body;
    // }
public:
    ForLoopAST(StmtAST::Ptr &initial, ExprAST::Ptr &iterator, StmtAST::Ptr &body)
    : forEach(true), initial(std::move(initial)), condition(nullptr), 
        iterator(std::move(iterator)), body(std::move(body)) {}
    ForLoopAST(StmtAST::Ptr &initial, ExprAST::Ptr &condition, ExprAST::Ptr &iterator, StmtAST::Ptr &body)
    : forEach(false), initial(std::move(initial)), condition(std::move(condition)),
        iterator(std::move(iterator)), body(std::move(body)) {}

    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtAST::Ptr analyse(Expectation exp) override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
 
    Position pos = Position();
};

typedef unique_ptr<CodeBlockAST> BlockPtr;

// prototype of a function
// name and arguments
class PrototypeAST : public StmtAST {
    FuxType type;
    ExprAST::Ptr symbol;
    StmtAST::Vec args;

public:
    PrototypeAST(FuxType type, const string &symbol, StmtAST::Vec &args)
    : type(type), symbol(make_unique<VariableExprAST>(symbol)), args(std::move(args)) {}
    PrototypeAST(FuxType type, ExprAST::Ptr &symbol, StmtAST::Vec &args)
    : type(type), symbol(std::move(symbol)), args(std::move(args)) {}
    ~PrototypeAST() override;
    
    ExprAST::Ptr &getSymbol();
    StmtAST::Vec &getArgs();
    FuxType &getType();
    
    FUX_BC(Function *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtAST::Ptr analyse(Expectation exp) override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
     
    Position pos = Position();
};

typedef unique_ptr<PrototypeAST> ProtoPtr;

class FunctionAST : public StmtAST {
    ProtoPtr proto;
    StmtAST::Ptr body;

public:
    FunctionAST(FuxType type, const string &symbol, StmtAST::Vec &args, StmtAST::Ptr &body)
    : proto(make_unique<PrototypeAST>(type, symbol, args)), body(std::move(body)) {}
    FunctionAST(FuxType type, ExprAST::Ptr &symbol, StmtAST::Vec &args, StmtAST::Ptr &body)
    : proto(make_unique<PrototypeAST>(type, symbol, args)), body(std::move(body)) {}
    FunctionAST(ProtoPtr &proto, StmtAST::Ptr &body)
    : proto(std::move(proto)), body(std::move(body)) {}

    FUX_BC(Function *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtAST::Ptr analyse(Expectation exp) override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;    

    Position pos = Position();
};

class RootAST : public StmtAST {
    StmtAST::Vec program;
    // resting place for array size expressions
    // FuxTypes refer to these by IDs
    ExprAST::Vec arraySizeExprs; 

public:
    RootAST() : program(StmtAST::Vec()) {}        
    
    FUX_BC(Value *codegen(LLVMWrapper *fuxLLVM) override;)
    StmtAST::Ptr analyse(Expectation exp) override;
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
 
    void addSub(StmtAST::Ptr &sub);
    _i64 addSizeExpr(ExprAST::Ptr &sizeExpr);
    
    Position pos = Position();
};

typedef unique_ptr<RootAST> RootPtr;