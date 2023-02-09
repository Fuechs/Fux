/**
 * @file expectation.cpp
 * @author fuechs
 * @brief expectation struct
 * @version 0.1
 * @date 2023-02-02
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "expectation.hpp"

Expectation::Expectation(ErrorManager *error, SymbolTable *table, Preset preset) {
    this->error = error;
    this->table = table;
    this->kinds = Kinds();

    switch (preset) {
        default: break;
    }
}

Expectation::Expectation(ErrorManager *error, SymbolTable *table, Kinds kinds) 
: table(table), kinds(kinds), error(error) {}