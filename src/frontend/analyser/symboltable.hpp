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

    enum Type {
        INT,        // any integer type
        FLOAT,      // any float type
        CHAR,       // any char type
        STR,        // any string type
        NO_TYPE,
    };

    Symbol(Kind kind = NONE, Type type = NO_TYPE);

    Kind kind;
    Type type;
};

class SymbolTable {
public:
    typedef std::unordered_map<std::string, Symbol *> Map;

    SymbolTable() : table(Map()) {}

    Symbol *operator[](std::string symbol);
    
    void insert(std::string symbol, Symbol *_symbol);
    void insert(std::string symbol, Symbol::Kind kind, Symbol::Type type);    
    void erase(std::string symbol);

    size_t size();
    bool empty();

private:
    Map table;
};