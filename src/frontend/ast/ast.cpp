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

AST NullExprAST::getASTType() { return AST::NullExprAST; }

BoolExprAST::~BoolExprAST() { delete value; }
AST BoolExprAST::getASTType() { return AST::BoolExprAST; }

NumberExprAST::~NumberExprAST() { delete value; }
AST NumberExprAST::getASTType() { return AST::NumberExprAST; }

CharExprAST::~CharExprAST() { delete value; }
AST CharExprAST::getASTType() { return AST::CharExprAST; }

StringExprAST::~StringExprAST() { delete value; }
AST StringExprAST::getASTType() { return AST::StringExprAST; }

VariableExprAST::~VariableExprAST() { name.clear(); }
AST VariableExprAST::getASTType() { return AST::VariableExprAST; }

AST MemberExprAST::getASTType() { return AST::MemberExprAST; }

AST UnaryExprAST::getASTType() { return AST::UnaryExprAST; }

AST BinaryExprAST::getASTType() { return AST::BinaryExprAST; }

AST CallExprAST::getASTType() { return AST::StringExprAST; }

AST TypeCastExprAST::getASTType() { return AST::TypeCastExprAST; }

AST TernaryExprAST::getASTType() { return AST::TernaryExprAST; }

VariableDeclAST::~VariableDeclAST() { symbol.clear(); }
AST VariableDeclAST::getASTType() { return AST::VariableDeclAST; }
string &VariableDeclAST::getSymbol() { return symbol; }
FuxType &VariableDeclAST::getType() { return type; }
ExprPtr &VariableDeclAST::getValue() { return value; }

AST InbuiltCallAST::getASTType() { return AST::InbuiltCallAST; }

AST IfElseAST::getASTType() { return AST::IfElseAST; }

void CodeBlockAST::addSub(StmtPtr &sub) { body.push_back(std::move(sub)); }
AST CodeBlockAST::getASTType() { return AST::CodeBlockAST; }

PrototypeAST::~PrototypeAST() { args.clear(); }

ExprPtr &PrototypeAST::getSymbol() { return symbol; }
StmtList &PrototypeAST::getArgs() { return args; }
FuxType &PrototypeAST::getType() { return type; }
AST PrototypeAST::getASTType() { return AST::PrototypeAST; }

AST FunctionAST::getASTType() { return AST::FunctionAST; }

void RootAST::addSub(StmtPtr &sub) { program.push_back(std::move(sub)); }
AST RootAST::getASTType() { return AST::RootAST; }

bool StmtAST::isExpr() { 
    return (this->getASTType() >= AST::NullExprAST 
        && this->getASTType() <= AST::TernaryExprAST);  
}

string BinaryOpValue(BinaryOp &op) { return TokenTypeValue[(TokenType) op]; }

string UnaryOpValue(UnaryOp &op) { 
    switch (op) {
        case UnaryOp::PINC:
        case UnaryOp::SINC:     return "++";
        case UnaryOp::PDEC:
        case UnaryOp::SDEC:     return "--";
        default:                return TokenTypeValue[(TokenType) op]; 
    }
}

string InbuiltsValue(Inbuilts &inbuilt) { return TokenTypeValue[(TokenType) inbuilt]; }