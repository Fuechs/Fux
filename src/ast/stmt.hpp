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

#include "../util/metadata.hpp"
#include "../parser/type.hpp"

FUX_AC(class Analyser;)

enum class AST {
    // expressions
    VariadicExpr,
    NullExpr,
    BoolExpr,
    NumberExpr,
    CharExpr,
    StringExpr,
    RangeExpr,
    ArrayExpr,
    SymbolExpr,
    MemberExpr,
    CallExpr,
    UnaryExpr,
    BinaryExpr,
    TypeCastExpr,
    TernaryExpr,
    Expr,

    // statements
    NoOperationStmt,
    VariableStmt,
    InbuiltCallStmt,
    IfElseStmt,
    BlockStmt,
    WhileStmt,
    ForStmt,
    PrototypeStmt,
    FunctionStmt,
    EnumStmt,
    MacroStmt,
    Root,
    Stmt,
};

class Stmt {
public:
    using Ptr = shared_ptr<Stmt>;
    using Vec = vector<Ptr>;

    virtual ~Stmt();
    FUX_BC(virtual Eisdrache::Local &codegen(Eisdrache *eisdrache);)
    FUX_AC(virtual Ptr analyse(Analyser *analyser);)
    virtual AST getASTType();
    virtual Fux::Type getFuxType();
    virtual string getSymbol();
    virtual void debugPrint(size_t indent = 0);

    bool isExpr();

    Metadata meta = Metadata();
};

extern Stmt::Ptr nullStmt;