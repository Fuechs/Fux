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

#include "../metadata.hpp"
#include "../parser/type.hpp"

class Analyser;

enum class AST {
    // expressions
    VariadicExprAST,
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
    NoOperationAST,
    VariableDeclAST,
    InbuiltCallAST,
    IfElseAST,
    CodeBlockAST,
    WhileLoopAST,
    ForLoopAST,
    PrototypeAST,
    FunctionAST,
    EnumerationAST,
    MacroAST,
    RootAST,
};

class StmtAST {
public:
    using Ptr = unique_ptr<StmtAST>;
    using Vec = vector<Ptr>;

    virtual ~StmtAST() {}
    FUX_BC(virtual Eisdrache::Local &codegen(Eisdrache *eisdrache) = 0;)
    virtual Ptr analyse(Analyser *analyser) = 0;
    virtual AST getASTType() = 0;
    virtual FuxType getFuxType() = 0;
    virtual string getSymbol();
    virtual void debugPrint(size_t indent = 0) = 0;

    bool isExpr();

    Metadata meta = Metadata();
};

extern StmtAST::Ptr nullStmt;