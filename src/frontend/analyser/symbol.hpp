/**
 * @file symbol.hpp
 * @author fuechs
 * @brief symbol header
 * @version 0.1
 * @date 2023-03-18
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../parser/type.hpp"
#include "../metadata.hpp"

class Symbol {
public:
    using Vec = std::vector<Symbol *>;

    enum Kind {
        VARIBALE,
        FUNCTION,
        ENUM,
        STRUCT,
        CLASS,
        NONE,
    };

    virtual ~Symbol();
    
    string &        getSymbol();
    const string &  getSymbol() const;

    Symbol *        getParent();
    const Symbol *  getParent() const;

    Metadata &      getMeta();
    const Metadata &getMeta() const;

    void setParent(Symbol *parent);

    virtual Kind getKind() const = 0;
    // get debug info of this symbol
    virtual string getInfo() const = 0;
    virtual const FuxType &getType() const = 0;
    // check wether symbol is defined (has a value, body, etc.)
    virtual bool isDefined() const = 0;
    // set defined to true
    virtual void setDefined() = 0;
    // add a sub member
    virtual void addMember(Symbol *symbol) = 0;

protected:
    string symbol;
    Symbol *parent;
    Metadata meta;
};

class Variable : public Symbol {
public:
    Variable(FuxType type = FuxType::NO_TYPE, string symbol = "", Metadata meta = Metadata(), Symbol *parent = nullptr);
    ~Variable() override;

    Kind getKind() const override;
    string getInfo() const override;
    const FuxType &getType() const override;
    bool isDefined() const override;
    void setDefined() override;
    void addMember(Symbol *symbol) override;

private:
    FuxType type;
};

class Function : public Symbol {
public:
    Function(FuxType type = FuxType::NO_TYPE, string symbol = "", 
        FuxType::Vec parameters = {}, Symbol::Vec locals = {}, 
        Metadata meta = Metadata(), Symbol *parent = nullptr);
    ~Function() override;
    
    Kind getKind() const override;
    string getInfo() const override;
    const FuxType &getType() const override;
    bool isDefined() const override;
    void setDefined() override;
    void addMember(Symbol *symbol) override;

private: 
    FuxType type;
    FuxType::Vec parameters;
    Symbol::Vec locals;
    bool defined = false;
};

// TODO: (soon) class, struct, typedef, enum, macro

class Table {
public:
    Table();
    ~Table();

    Symbol *contains(std::string symbol);
    void insert(Symbol *symbol);

private:
    Symbol::Vec table;
};