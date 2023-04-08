/**
 * @file lifetime.hpp
 * @author fuechs
 * @brief object lifetime
 * @version 0.1
 * @date 2023-04-03
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../ast/ast.hpp"

#ifdef FUX_ANALYSER

class Lifetime {

    Lifetime(Stmt *begin = nullptr, Stmt *end = nullptr, vector<Stmt *> uses = {});
    ~Lifetime();

    // insert free or delete stmt for the object
    void free();

    Root *root;
    Stmt *begin, *end;
    vector<Stmt *> uses;
};

/* TODO: Optimize lifetimes with AST or with LLVM IR?
x: i64;             begin ________________________ >> push(<expr>); 
x = 0;              use 0 ___ >> x = <expr>; _/ /     -> Throw warning and optimize out
x = <expr>;         use 1 _/                  /
push(x);            use 2; end _____________/
*/

#endif