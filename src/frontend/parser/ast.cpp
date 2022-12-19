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

Position &Position::operator=(Position &pos) {
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

// * debugPrint() functions 

void RootAST::debugPrint() {
    if (!fux.options.debugMode)
        return;

    cout << getDebugText() << "Root AST";
    for (ExprPtr &sub : program) {
        cout << "\n";
        sub->debugPrint();
    }
    cout << endl;
}

void NumberExprAST::debugPrint() { cout << value; }

void VariableExprAST::debugPrint() { cout << name; }

void BinaryExprAST::debugPrint() {
    cout << "(";
    LHS->debugPrint();
    cout << " " << op << " ";
    RHS->debugPrint();
    cout << ")";
}

void CallExprAST::debugPrint() { 
    cout << callee << "( ";
    for (ExprPtr &element : args) {
        element->debugPrint();
        cout << ", ";
    }
    cout << ");";
}

void PrototypeAST::debugPrint() {
    cout << name << "( ";
    for (auto &param : args)
        cout << fuxType::TypeString[param.second] << ", ";
    cout << "): " << fuxType::TypeString[type] << ";";
}

void FunctionAST::debugPrint() { 
    cout << proto->getName() << "( ";
    for (auto &param : proto->getArgs()) 
        cout << param.first << ": " << fuxType::TypeString[param.second] << ", ";
    cout << "): "<< fuxType::TypeString[proto->getType()] << " {\n";
    for (ExprPtr &stmt : body) {
        cout << "\t";
        stmt->debugPrint();
        cout << "\n";
    }
    cout << "}";
}