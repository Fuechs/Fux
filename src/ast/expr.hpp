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

class Expr : public Stmt {
public:
    using Ptr = shared_ptr<Expr>;
    using Vec = vector<Ptr>;
    
    // enclosed in paranthesis, used to silence errors / warnings
    bool enclosed = false; 
};

extern Expr::Ptr nullExpr;