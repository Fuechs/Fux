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
    program.push_back(std::move(sub));
}

NumberExprAST::~NumberExprAST() { delete &value; }

PrototypeAST::~PrototypeAST() { name.clear(); }
string PrototypeAST::getName() { return name; }
ArgMap PrototypeAST::getArgs() { return args; }
FuxType PrototypeAST::getType() { return type; }

// unused root ast stuff
void NumberExprAST::addSub(ExprPtr &sub) { return; }
void VariableExprAST::addSub(ExprPtr &sub) { return; }
void BinaryExprAST::addSub(ExprPtr &sub) { return; }
void ComparisonExprAST::addSub(ExprPtr &sub) { return; }
void LogicalExprAST::addSub(ExprPtr &sub) { return; }
void CallExprAST::addSub(ExprPtr &sub) { return; }
void VariableDeclAST::addSub(ExprPtr &sub) { return; }
void PutsCallAST::addSub(ExprPtr &sub) { return; }
void IfElseAST::addSub(ExprPtr &sub) { return; }
void PrototypeAST::addSub(ExprPtr &sub) { return; }
void FunctionAST::addSub(ExprPtr &sub) { return; }