/**
 * @file ast.cpp
 * @author fuechs
 * @brief fux abstract syntax tree 
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "ast.hpp"

ExprPtr nullExpr = ExprPtr(nullptr);

Position &Position::operator=(Position pos) {
    lStart = pos.lStart;
    lEnd = pos.lStart;
    colStart = pos.colStart;
    colEnd = pos.colEnd;
    return *this; 
}

ExprAST &ExprAST::operator=(ExprAST &ast) {
    *this = ast;
    return *this;
}

void RootAST::addSub(ExprPtr &sub) {
    program.push_back(move(sub));
}

ExprList RootAST::getProg() {
    return move(program);
}

PrototypeAST::~PrototypeAST() { name.clear(); }
string PrototypeAST::getName() { return name; }
ArgMap PrototypeAST::getArgs() { return args; }
fuxType::Type PrototypeAST::getType() { return type; }