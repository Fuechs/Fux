/**
 * @file stmt.hpp
 * @author fuechs
 * @brief stmt ast class header
 * @version 0.1
 * @date 2023-01-15
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../analyser/expectation.hpp"

enum class AST {
    // expressions
    NullExprAST,
    BoolExprAST,
    NumberExprAST,
    CharExprAST,
    StringExprAST,
    RangeExprAST,
    ArrayExprAST,
    VariableExprAST,
    MemberExprAST,
    CallExprAST,
    UnaryExprAST,
    BinaryExprAST,
    TypeCastExprAST,
    TernaryExprAST,

    // statements
    VariableDeclAST,
    InbuiltCallAST,
    IfElseAST,
    CodeBlockAST,
    PrototypeAST,
    FunctionAST,
    WhileLoopAST,
    ForLoopAST,
    RootAST,
};

class StmtAST {
public:
    typedef unique_ptr<StmtAST> Ptr;
    typedef vector<Ptr> Vec;

    virtual ~StmtAST() {}
    FUX_BC(virtual Value *codegen(LLVMWrapper *fuxLLVM) = 0;)
    virtual Ptr analyse(Expectation exp) = 0;
    virtual AST getASTType() = 0;
    virtual void debugPrint(size_t indent = 0) = 0;

    bool isExpr();

    Position pos = Position();
};

extern StmtAST::Ptr nullStmt;