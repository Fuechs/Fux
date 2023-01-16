/**
 * @file ast.cpp
 * @author fuechs
 * @brief fux abstract syntax tree 
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#include "ast.hpp"

StmtPtr nullStmt = StmtPtr(nullptr);
ExprPtr nullExpr = ExprPtr(nullptr);

Position &Position::operator=(Position pos) {
    lStart = pos.lStart;
    lEnd = pos.lStart;
    colStart = pos.colStart;
    colEnd = pos.colEnd;
    return *this; 
}

NumberExprAST::~NumberExprAST() { delete value; }

CallExprAST::~CallExprAST() { callee.clear(); }

PrototypeAST::~PrototypeAST() { 
    name.clear(); 
    args.clear();
}

string PrototypeAST::getName() { return name; }
ArgMap PrototypeAST::getArgs() { return args; }
FuxType PrototypeAST::getType() { return type; }

void RootAST::addSub(StmtPtr &sub) { program.push_back(std::move(sub)); }