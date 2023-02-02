/**
 * @file expectation.hpp
 * @author fuechs
 * @brief expectation struct header
 * @version 0.1
 * @date 2023-01-15
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "symboltable.hpp"

/**
 * TODO: Expectation struct
 * This struct should be passed to the analyse() function of ASTs.
 * It should include expectations (e.g. Integer, Binary Expression, Null Expression, Function Call)
 * in an enum, and what they are for.
 * We need this since we can't access the elements of an AST 
 * without defining a lot of getter functions (because of dynamic pointers).
 * There may be another solution to this. 
 * -->  Define a function for every AST that returns the type.
 *      Such function would have to traverse down the tree 
 *      and be flexible enough for every required kinds of checks.
 *      Could probably be implemented in the respective analyse() functions.
 */
struct Expectation {
    enum Preset {

    };

    enum Kind {

    }; 

    typedef std::vector<Kind> Kinds;

    Expectation(SymbolTable *table, Preset preset);
    Expectation(SymbolTable *table, Kinds kinds);

    Preset preset;
    Kinds list;
    SymbolTable *table;
};