/**
 * @file analyzer.hpp
 * @author fuechs
 * @brief fux ast analyser header
 * @version 0.1
 * @date 2022-11-27
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../error/error.hpp"
#include "../parser/ast.hpp"
#include "../parser/type.hpp"

class Analyser {
public:
    Analyser(ErrorManager *error, RootAST *root) 
    : error(error), origin(root), mod(new RootAST()) {} 

    ~Analyser() { delete mod; }

    // analyse AST 
    void analyse() { return; }

private:
    ErrorManager *error;
    RootAST *origin;
    RootAST *mod;

    void debugPrint(const string message);
};