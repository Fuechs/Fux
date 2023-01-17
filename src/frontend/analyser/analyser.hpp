/**
 * @file analyzer.hpp
 * @author fuechs
 * @brief fux ast analyser header
 * @version 0.1
 * @date 2022-11-27
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../error/error.hpp"
#include "../ast/ast.hpp"
#include "../parser/type.hpp"
#include "expectation.hpp"

class Analyser {
public:
    Analyser(ErrorManager *error, RootPtr &root) 
    : error(error), origin(root) {} 

    // analyse AST 
    StmtPtr analyse();

private:
    ErrorManager *error;
    RootPtr &origin;

    void debugPrint(const string message);
};

// TODO: Existence conditions
// The analyser has to check conditions for existence checks
// and replace these with true, false,
// or if possible remove the whole if statement.
// A list of predefined symbols ("objects") would be required for this.

// The list of defined symbols needs to include details about the object
// to be able to verify overrides.