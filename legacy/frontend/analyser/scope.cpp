/**
 * @file scope.cpp
 * @author fuechs
 * @brief scope struct
 * @version 0.1
 * @date 2023-04-02
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "scope.hpp"

Scope::Scope(Scope *parent) : parent(parent), children({}), table() {}

Scope::~Scope() {
    for (Scope *&child : children)
        delete child;
    children.clear();
}

Scope *Scope::leave() {
    Scope *ret = parent;
    delete this;
    return ret;
}

Scope *Scope::enter() {
    Scope *child = new Scope(this);
    children.push_back(child);
    return child;
}

Symbol *Scope::contains(string symbol) { 
    Symbol *ret = table.contains(symbol);

    if (!ret && parent)                
        ret = parent->contains(symbol); // search in higher scope for symbol
    
    return ret;
}

void Scope::insert(Symbol *symbol) { table.insert(symbol); }