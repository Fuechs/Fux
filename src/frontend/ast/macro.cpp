/**
 * @file macro.cpp
 * @author fuechs
 * @brief Macro Declaration Statement AST Implementation
 * @version 0.1
 * @date 2023-04-14
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "macro.hpp"

MacroStmt::MacroStmt(const string &symbol, Case::Vec cases) : symbol(symbol), cases(cases) {}

MacroStmt::Arg::Arg(Metadata meta, string symbol, ID type, bool array, bool variadic) 
: meta(meta), symbol(symbol), type(type), array(array), variadic(variadic) {}


MacroStmt::Arg &MacroStmt::Arg::operator=(const MacroStmt::Arg &copy) {
    symbol = copy.symbol;
    type = copy.type;
    array = copy.array;
    variadic = copy.variadic;
    meta = copy.meta;
    return *this;
}

AST MacroStmt::getASTType() { return AST::MacroStmt; }

string MacroStmt::getSymbol() { return symbol; }

MacroStmt::Case::Case(Arg::Vec args, Stmt::Ptr &ret) : args(args), ret(std::move(ret)) {}