/**
 * @file scope.hpp
 * @author fuechs
 * @brief scope struct header
 * @version 0.1
 * @date 2023-04-02
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once
 
#include "../../fux.hpp"
#include "symbol.hpp"

#ifdef FUX_ANALYSER

class Scope {
public:
    using Vec = vector<Scope *>;

    Scope(Scope *parent = nullptr);
    ~Scope();

    Scope *leave();
    Scope *enter();

    Symbol *contains(string symbol);
    void insert(Symbol *symbol);

private:
    Scope *parent;
    Scope::Vec children;
    Table table;
}; 

#endif