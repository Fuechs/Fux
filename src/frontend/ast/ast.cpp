/**
 * @file ast.cpp
 * @author fuechs
 * @brief fux abstract syntax tree 
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
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
AST NumberExprAST::getASTType() { return AST::NumberExprAST; }

StringExprAST::~StringExprAST() { delete value; }
AST StringExprAST::getASTType() { return AST::StringExprAST; }

VariableExprAST::~VariableExprAST() { name.clear(); }
AST VariableExprAST::getASTType() { return AST::VariableExprAST; }

AST MemberExprAST::getASTType() { return AST::MemberExprAST; }

AST BinaryExprAST::getASTType() { return AST::BinaryExprAST; }

AST ComparisonExprAST::getASTType() { return AST::ComparisonExprAST; }

AST LogicalExprAST::getASTType() { return AST::LogicalExprAST; }

CallExprAST::~CallExprAST() { callee.clear(); }
AST CallExprAST::getASTType() { return AST::StringExprAST; }

AST AssignmentExprAST::getASTType() { return AST::AssignmentExprAST; }

VariableDeclAST::~VariableDeclAST() { symbol.clear(); }
AST VariableDeclAST::getASTType() { return AST::VariableDeclAST; }
string &VariableDeclAST::getSymbol() { return symbol; }
FuxType &VariableDeclAST::getType() { return type; }
ExprPtr &VariableDeclAST::getValue() { return value; }

AST PutsCallAST::getASTType() { return AST::PutsCallAST; }

AST ReturnCallAST::getASTType() { return AST::ReturnCallAST; }

AST IfElseAST::getASTType() { return AST::IfElseAST; }

void CodeBlockAST::addSub(StmtPtr &sub) { body.push_back(std::move(sub)); }
AST CodeBlockAST::getASTType() { return AST::CodeBlockAST; }

PrototypeAST::~PrototypeAST() { 
    name.clear(); 
    args.clear();
}

string PrototypeAST::getName() { return name; }
ArgMap PrototypeAST::getArgs() { return args; }
FuxType PrototypeAST::getType() { return type; }
AST PrototypeAST::getASTType() { return AST::PrototypeAST; }

AST FunctionAST::getASTType() { return AST::FunctionAST; }

void RootAST::addSub(StmtPtr &sub) { program.push_back(std::move(sub)); }
AST RootAST::getASTType() { return AST::RootAST; }