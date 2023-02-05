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
    
    typedef std::unordered_map<std::string, Symbol *> Map;
    
    Symbol(Kind kind = NONE, FuxType type = FuxType::NO_TYPE, FuxType::Vec parameters = FuxType::Vec());
    Symbol(Symbol *parent, Kind kind = NONE, FuxType type = FuxType::NO_TYPE, FuxType::Vec parameters = FuxType::Vec());
    
    Symbol *operator[](std::string symbol);

    Symbol *addMember(std::string name, Kind kind = NONE, FuxType type = FuxType::NO_TYPE, FuxType::Vec parameters = FuxType::Vec());

    Kind kind;
    FuxType type;
    FuxType::Vec parameters;

    bool member;
    Symbol *parent;
    Map members;
};

class SymbolTable {
public:
    typedef std::vector<SymbolTable *> Vec;

    SymbolTable() : table(Symbol::Map()) {}

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
    Symbol::Map table;
};