/**
 * @file symbol.cpp
 * @author fuechs
 * @brief symbol
 * @version 0.1
 * @date 2023-03-18
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "symbol.hpp"

/// SYMBOL ///

Symbol::~Symbol() { symbol.clear(); }

string &        Symbol::getSymbol() { return symbol; }
const string &  Symbol::getSymbol() const { return symbol; }

Symbol *        Symbol::getParent() { return parent; }
const Symbol *  Symbol::getParent() const { return parent; }

void Symbol::setParent(Symbol *parent) { this->parent = parent; }

/// VARIABLE ///

Variable::Variable(FuxType type, string symbol, Symbol *parent) {
    this->type = type;
    this->symbol = symbol;
    this->parent = parent;
}

Variable::~Variable() { symbol.clear(); }

Symbol::Kind Variable::getKind() const { return VARIBALE; }

string Variable::getInfo() const { return string(); }

const FuxType &Variable::getType() const { return type; }

bool Variable::isDefined() const { return true; /* a variable is always initialised */ }

void Variable::setDefined() { 
    assert(!"Variable::setDefined(): This function must not be called."); 
} 

void Variable::addMember(Symbol *symbol) { 
    assert(!"Variable::addMember(): This function must not be called.");
} 

/// FUNCTION ///

Function::Function(FuxType type, string symbol, 
    FuxType::Vec parameters, Symbol::Vec locals, Symbol *parent) {
        this->type = type;
        this->symbol = symbol;
        this->parameters = parameters;
        this->locals = locals;
        this->parent = parent;
}

Function::~Function() { 
    symbol.clear(); 
    parameters.clear();
    locals.clear();
}

Symbol::Kind Function::getKind() const { return FUNCTION; }

string Function::getInfo() const { return string(); }

const FuxType &Function::getType() const { return type; }

bool Function::isDefined() const { return defined; }

void Function::setDefined() { defined = true; }

void Function::addMember(Symbol *symbol) {
    symbol->setParent(this);
    locals.push_back(symbol);
}

/// TABLE ///

Table::Table() { table = {}; }

Table::~Table() {
    for (Symbol *&symbol : table)
        delete symbol;
    table.clear();
}

Symbol *Table::contains(std::string symbol) {
    for (Symbol *&sym : table)
        if (sym->getSymbol() == symbol)
            return sym;
    return nullptr;
}