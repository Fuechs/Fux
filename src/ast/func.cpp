/**
 * @file func.cpp
 * @author fuechs
 * @brief Function Declaration Statement AST Implementation
 * @version 0.1
 * @date 2023-04-09
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "func.hpp"

FunctionStmt::Parameter::Parameter(Metadata meta, string symbol, FuxType type, Expr::Ptr value, bool variadic)
: meta(meta), symbol(symbol), type(type), value(value), variadic(variadic) {}

FunctionStmt::Parameter::~Parameter() { symbol.clear(); }

FunctionStmt::Parameter &FunctionStmt::Parameter::operator=(const Parameter &copy) {
    symbol = copy.symbol;
    type = copy.type;
    value = copy.value;
    variadic = copy.variadic;
    meta = copy.meta;
    return *this;
}

FunctionStmt::FunctionStmt(Metadata meta, string symbol, FuxType type, Parameter::Vec parameters, Stmt::Ptr body, Stmt::Vec locals) 
: symbol(symbol), type(type), parameters(parameters), body(body), locals(locals) {
    this->meta = meta;
}

FunctionStmt::~FunctionStmt() { 
    symbol.clear(); 
    parameters.clear();
    locals.clear();
}

AST FunctionStmt::getASTType() { return AST::FunctionStmt; }

FuxType FunctionStmt::getFuxType() { return type; }

string FunctionStmt::getSymbol() { return symbol; }