/**
 * @file expr.hpp
 * @author fuechs
 * @brief expr ast class header
 * @version 0.1
 * @date 2023-01-14
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#pragma once

#include "../../backend/generator/wrapper.hpp"
#include "position.hpp"

class ExprAST {
public:
    virtual ~ExprAST() {}
    virtual Value *codegen(LLVMWrapper *fuxLLVM) = 0;
    virtual unique_ptr<ExprAST> analyse() = 0;
    virtual void debugPrint() = 0;

    // root functions
    virtual void addSub(unique_ptr<ExprAST> &sub) = 0;

    ExprAST &operator=(ExprAST &ast);

    Position pos = Position();
};

typedef unique_ptr<ExprAST> ExprPtr;
typedef vector<ExprPtr> ExprList;

extern ExprPtr nullExpr;