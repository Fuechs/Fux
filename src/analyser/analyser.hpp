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

#include "../fux.hpp"
#include "../error/error.hpp"
#include "../ast/ast.hpp"
#include "../parser/type.hpp"
#include "symbol.hpp"
#include "scope.hpp"

#ifdef FUX_ANALYSER

class Analyser {
public:
    Analyser(const string &file, Root::Ptr &root);

    // analyse AST 
    Stmt::Ptr analyse();

    // enter new scope
    void enter();
    // leave current scope
    void leave();

    // insert new symbol in current scope
    void insert(Symbol *symbol);
    // search for symbol in current scope
    Symbol *contains(string symbol);

    template<typename To, typename From = Stmt::Ptr>
    typename To::Ptr cast(From &ptr);

    // analyse AST and return casted pointer
    template<typename To, typename From = Stmt::Ptr>
    typename To::Ptr process(From &ptr);

    // mangle a symbol name according to the linked statement
    string mangleSymbol(Stmt::Ptr &link);

private:
    const string &file;
    Root::Ptr &origin;
    Scope *current;

    void debugPrint(const string message);
};

// TODO: Existence conditions
// The analyser has to check conditions for existence checks
// and replace these with true, false,
// or if possible remove the whole if statement.
// A list of predefined symbols ("objects") would be required for this.

// The list of defined symbols needs to include details about the object
// to be able to verify overrides.

#endif