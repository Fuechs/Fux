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

#include "stmt.hpp"

class ExprAST : public StmtAST {
public:
    using Ptr = unique_ptr<ExprAST>;
    using Vec = vector<Ptr>;

    virtual ~ExprAST() {}
    FUX_BC(virtual Eisdrache::Local &codegen(Eisdrache *eisdrache) = 0;)
    virtual StmtAST::Ptr analyse(Analyser *analyser) = 0;
    virtual AST getASTType() = 0;
    virtual FuxType getFuxType() = 0;
    virtual void debugPrint(size_t indent = 0) = 0;

    // enclosed in paranthesis, used to silence errors / warnings
    bool enclosed = false; 
};

extern ExprAST::Ptr nullExpr;