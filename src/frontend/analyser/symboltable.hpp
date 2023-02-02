/**
 * @file symboltable.hpp
 * @author fuechs
 * @brief symboltable class header
 * @version 0.1
 * @date 2023-02-02
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "../parser/type.hpp"

// TODO: add support for symbol members, e.g. of classes, structs, enums
struct Symbol {
public:
    enum Kind {
        VAR,        // variable
        FUNC,       // function
        ENUM,       // enum
        STRUCT,     // struct
        CLASS,      // class
        PACKAGE,    // package
        NONE,       
    };
    
    Symbol(Kind kind = NONE, FuxType type = FuxType::NO_TYPE);

    Kind kind;
    FuxType type;
};

class SymbolTable {
public:
    typedef std::unordered_map<std::string, Symbol *> Map;
    typedef std::vector<SymbolTable *> Vec;

    SymbolTable() : table(Map()) {}

    // get Symbol * of name `symbol`
    Symbol *operator[](std::string symbol);

    // aka operator[]
    Symbol *contains(std::string symbol);
    
    // insert given Symbol * `_symbol` at name `symbol`
    void insert(std::string symbol, Symbol *_symbol);
    // create new Symbol * at name `symbol`
    void insert(std::string symbol, Symbol::Kind kind, FuxType type);    
    // erase all symbols named `symbol`
    void erase(std::string symbol);

    // get table size
    size_t size();
    // check wether table is empty
    bool empty();

private:
    Map table;
};