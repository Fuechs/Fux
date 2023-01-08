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
    Analyser(ErrorManager *error, ExprPtr &root) 
    : error(error), origin(root) {} 

    // analyse AST 
    ExprPtr analyse();

private:
    ErrorManager *error;
    ExprPtr &origin;

    void debugPrint(const string message);
};