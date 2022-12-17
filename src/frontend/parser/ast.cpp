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

void RootAST::addSub(ExprPtr &sub) {
    program.push_back(move(sub));
}

ExprList RootAST::getProg() {
    return move(program);
}

// * debugPrint() functions 

void RootAST::debugPrint() {
    if (!fux.options.debugMode)
        return;
        
    for (ExprPtr &sub : program)
        sub->debugPrint();
}

void NumberExprAST::debugPrint() {
    cout << value;
}

void VariableExprAST::debugPrint() {
    cout << name;
}

void BinaryExprAST::debugPrint() {
    cout << "( ";
    LHS->debugPrint();
    cout << " " << op << " ";
    RHS->debugPrint();
    cout << " )\n";
}

void CallExprAST::debugPrint() { return; }
void PrototypeAST::debugPrint() { return; }
void FunctionAST::debugPrint() { return; }