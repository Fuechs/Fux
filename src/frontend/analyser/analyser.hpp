/**
 * @file analyzer.hpp
 * @author fuechs
 * @brief fux ast analyser header
 * @version 0.1
 * @date 2022-11-27
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../error/error.hpp"
#include "../parser/ast.hpp"
#include "../parser/type.hpp"

class Analyser {
public:
    Analyser(ErrorManager *error, RootAST *root) : error(error), root(root) {}

    // analyse AST 
    void analyse() { return; }

private:
    ErrorManager *error;
    RootAST *root;
};