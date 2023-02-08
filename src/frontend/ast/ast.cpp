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

StmtAST::Ptr nullStmt = StmtAST::Ptr(nullptr);
ExprAST::Ptr nullExpr = ExprAST::Ptr(nullptr);

Position &Position::operator=(Position pos) {
    lStart = pos.lStart;
    lEnd = pos.lStart;
    colStart = pos.colStart;
    colEnd = pos.colEnd;
    return *this; 
}

AST NullExprAST::getASTType() { return AST::NullExprAST; }
FuxType NullExprAST::getFuxType() { return FuxType::NO_TYPE; }

BoolExprAST::~BoolExprAST() { delete value; }
AST BoolExprAST::getASTType() { return AST::BoolExprAST; }
FuxType BoolExprAST::getFuxType() { return FuxType::BOOL; }

NumberExprAST::~NumberExprAST() { delete value; }
AST NumberExprAST::getASTType() { return AST::NumberExprAST; }
FuxType NumberExprAST::getFuxType() { return value->type; }

CharExprAST::~CharExprAST() { delete value; }
AST CharExprAST::getASTType() { return AST::CharExprAST; }
FuxType CharExprAST::getFuxType() { return value->type;  }

StringExprAST::~StringExprAST() { delete value; }
AST StringExprAST::getASTType() { return AST::StringExprAST; }
FuxType StringExprAST::getFuxType() { return FuxType::STR; }

AST RangeExprAST::getASTType() { return AST::RangeExprAST; }
// TODO: combine begin & end type
FuxType RangeExprAST::getFuxType() { return end->getFuxType(); }

AST ArrayExprAST::getASTType() { return AST::ArrayExprAST; }
// TODO: get element types for this
FuxType ArrayExprAST::getFuxType() { return FuxType::createArray(FuxType::NO_TYPE);  }

VariableExprAST::~VariableExprAST() { name.clear(); }
AST VariableExprAST::getASTType() { return AST::VariableExprAST; }
FuxType VariableExprAST::getFuxType() { return FuxType::NO_TYPE; }

AST MemberExprAST::getASTType() { return AST::MemberExprAST; }
FuxType MemberExprAST::getFuxType() { return FuxType::NO_TYPE; }

AST UnaryExprAST::getASTType() { return AST::UnaryExprAST; }
// TODO: evaluate type
FuxType UnaryExprAST::getFuxType() { return FuxType::NO_TYPE; }

AST BinaryExprAST::getASTType() { return AST::BinaryExprAST; }
// TODO: evaluate type
FuxType BinaryExprAST::getFuxType() { return FuxType::NO_TYPE; }

AST CallExprAST::getASTType() { return AST::StringExprAST; }
// TODO: evaluate type
FuxType BinaryExprAST::getFuxType() { return FuxType::NO_TYPE; }

AST TypeCastExprAST::getASTType() { return AST::TypeCastExprAST; }
FuxType TypeCastExprAST::getFuxType() { return type; }

AST TernaryExprAST::getASTType() { return AST::TernaryExprAST; }
FuxType TernaryExprAST::getFuxType() { return FuxType::BOOL; }

VariableDeclAST::~VariableDeclAST() { symbol.clear(); }
AST VariableDeclAST::getASTType() { return AST::VariableDeclAST; }
FuxType VariableDeclAST::getFuxType() { return type; }
string &VariableDeclAST::getSymbol() { return symbol; }
FuxType &VariableDeclAST::getType() { return type; }
ExprAST::Ptr &VariableDeclAST::getValue() { return value; }

AST InbuiltCallAST::getASTType() { return AST::InbuiltCallAST; }
FuxType InbuiltCallAST::getFuxType() { return FuxType::NO_TYPE; }

AST IfElseAST::getASTType() { return AST::IfElseAST; }
FuxType IfElseAST::getFuxType() { return FuxType::NO_TYPE; }
 
void CodeBlockAST::addSub(StmtAST::Ptr &sub) { body.push_back(std::move(sub)); }
AST CodeBlockAST::getASTType() { return AST::CodeBlockAST; }
FuxType CodeBlockAST::getFuxType() { return FuxType::NO_TYPE; }

AST WhileLoopAST::getASTType() { return AST::WhileLoopAST; }
FuxType WhileLoopAST::getFuxType() { return FuxType::NO_TYPE; }

AST ForLoopAST::getASTType() { return AST::ForLoopAST; }
FuxType ForLoopAST::getFuxType() { return FuxType::NO_TYPE; }

PrototypeAST::~PrototypeAST() { args.clear(); }
AST PrototypeAST::getASTType() { return AST::PrototypeAST; }
FuxType PrototypeAST::getFuxType() { return type; }
string &PrototypeAST::getSymbol() { return symbol; }
StmtAST::Vec &PrototypeAST::getArgs() { return args; }

AST FunctionAST::getASTType() { return AST::FunctionAST; }
FuxType FunctionAST::getFuxType() { return proto->getFuxType(); }

AST RootAST::getASTType() { return AST::RootAST; }
FuxType RootAST::getFuxType() { return FuxType::NO_TYPE; }

void RootAST::addSub(StmtAST::Ptr &sub) { program.push_back(std::move(sub)); }
_i64 RootAST::addSizeExpr(ExprAST::Ptr &sizeExpr) {
    arraySizeExprs.push_back(std::move(sizeExpr));
    return arraySizeExprs.size() - 1;
}

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