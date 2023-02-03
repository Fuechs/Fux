/**
 * @file expr.hpp
 * @author fuechs
 * @brief expr ast class header
 * @version 0.1
 * @date 2023-01-14
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../backend/generator/wrapper.hpp"
#include "position.hpp"
#include "stmt.hpp"

class ExprAST : public StmtAST {
public:
    typedef unique_ptr<ExprAST> Ptr;
    typedef vector<Ptr> Vec;

    virtual ~ExprAST() {}
    FUX_BC(virtual Value *codegen(LLVMWrapper *fuxLLVM) = 0;)
    virtual StmtAST::Ptr analyse(Expectation exp) = 0;
    virtual AST getASTType() = 0;
    virtual void debugPrint(size_t indent = 0) = 0;

    Position pos = Position();
};

extern ExprAST::Ptr nullExpr;