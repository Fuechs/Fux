/**
 * @file symboltable.cpp
 * @author fuechs
 * @brief symboltable class
 * @version 0.1
 * @date 2023-02-02
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "symboltable.hpp"

using std::string;

Symbol::Symbol(Kind kind, FuxType type, FuxType::Vec parameters) 
: kind(kind), type(type), parameters(parameters), member(false), parent(nullptr) {}

Symbol::Symbol(Symbol *parent, Kind kind, FuxType type, FuxType::Vec parameters)
: kind(kind), type(type), parameters(parameters), member(true), parent(parent) {}

Symbol *Symbol::operator[](string symbol) { return members.contains(symbol) ? members.at(symbol) : nullptr; }

Symbol *Symbol::addMember(string name, Kind kind, FuxType type, FuxType::Vec parameters) { 
    Symbol *sym = new Symbol(this, kind, type, parameters);
    members[name] = sym;
    return sym;
}

Symbol *SymbolTable::operator[](string symbol) { return table.contains(symbol) ? table.at(symbol) : nullptr; }

Symbol *SymbolTable::contains(string symbol) { return operator[](symbol); }

void SymbolTable::insert(string symbol, Symbol *_symbol) { table[symbol] = _symbol; }
void SymbolTable::insert(string symbol, Symbol::Kind kind, FuxType type) { table[symbol] = new Symbol(kind, type); }
void SymbolTable::erase(string symbol) { table[symbol] = nullptr; /* leave the field for error reporting */ }

auto SymbolTable::begin() { return table.begin(); }
auto SymbolTable::end() { return table.end(); }

size_t SymbolTable::size() { return table.size(); }
bool SymbolTable::empty() { return table.size() == 0; }