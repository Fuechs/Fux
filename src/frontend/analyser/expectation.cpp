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

Expectation::Expectation(SymbolTable::Vec tables, Preset preset) {
    this->tables = tables;
    this->kinds = Kinds();

    switch (preset) {
        default: break;
    }
}

Expectation::Expectation(SymbolTable::Vec tables, Kinds kinds) : tables(tables), kinds(kinds) {}