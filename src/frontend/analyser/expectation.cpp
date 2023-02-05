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

Expectation::Expectation(SymbolTable *table, Preset preset) {
    this->table = table;
    this->kinds = Kinds();

    switch (preset) {
        default: break;
    }
}

Expectation::Expectation(SymbolTable *table, Kinds kinds) : table(table), kinds(kinds) {}