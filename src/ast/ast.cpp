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

Stmt::Ptr nullStmt = Stmt::Ptr(nullptr);
Expr::Ptr nullExpr = Expr::Ptr(nullptr);

/// NULL EXPR ///

AST NullExpr::getASTType() { return AST::NullExpr; }

/// BOOL EXPR ///

BoolExpr::~BoolExpr() { delete value; }

AST BoolExpr::getASTType() { return AST::BoolExpr; }

Fux::Type BoolExpr::getFuxType() { return Fux::Type::BOOL; }

/// NUMBER EXPR ///

NumberExpr::~NumberExpr() { delete value; }

AST NumberExpr::getASTType() { return AST::NumberExpr; }

Fux::Type NumberExpr::getFuxType() { return value->type; }

/// CHAR EXPR ///

CharExpr::~CharExpr() { delete value; }

AST CharExpr::getASTType() { return AST::CharExpr; }

Fux::Type CharExpr::getFuxType() { return value->type;  }

/// STRING EXPR ///

StringExpr::~StringExpr() { delete value; }

AST StringExpr::getASTType() { return AST::StringExpr; }

Fux::Type StringExpr::getFuxType() { return Fux::Type::LIT; }

/// RANGE EXPR ///

AST RangeExpr::getASTType() { return AST::RangeExpr; }

// TODO: combine begin & end type
Fux::Type RangeExpr::getFuxType() { return end->getFuxType(); }

/// ARRAY EXPR ///

AST ArrayExpr::getASTType() { return AST::ArrayExpr; }

// TODO: get element types for this
Fux::Type ArrayExpr::getFuxType() { return Fux::Array();  }

/// VARIABLE EXPR ///

SymbolExpr::~SymbolExpr() { name.clear(); }

AST SymbolExpr::getASTType() { return AST::SymbolExpr; }

string SymbolExpr::getSymbol() { return name; }

/// CALL EXPR ///

CallExpr::CallExpr(const string &callee, Expr::Vec &args, bool asyncCall) {
    this->callee = make_shared<SymbolExpr>(callee);
    this->args = std::move(args);
    this->asyncCall = asyncCall;
}

CallExpr::CallExpr(Expr::Ptr &callee, Expr::Vec &args, bool asyncCall) {
    this->callee = std::move(callee);
    this->args = std::move(args);
    this->asyncCall = asyncCall;
    this->meta = this->callee->meta;
}

AST CallExpr::getASTType() { return AST::CallExpr; }

// TODO: evaluate type
Fux::Type CallExpr::getFuxType() { return Fux::Type::NO_TYPE; }

string CallExpr::getSymbol() { return callee->getSymbol(); }

/// MEMBER EXPR ///

MemberExpr::MemberExpr(Expr::Ptr &parent, const Token &member) {
    this->parent = std::move(parent);
    this->member = member.value;
    this->meta = this->parent->meta;
    this->meta.copyEnd(member);
}

MemberExpr::MemberExpr(Expr::Ptr &parent, const string &member) {
    this->parent = std::move(parent);
    this->member = member;
    this->meta = this->parent->meta;
}

AST MemberExpr::getASTType() { return AST::MemberExpr; }

string MemberExpr::getSymbol() { return parent->getSymbol()+"_"+member; }

/// UNARY EXPR ///

UnaryExpr::UnaryExpr(const Token &op, Expr::Ptr &expr, bool postOp) {
    switch (op.type) {
        case PLUS_PLUS:
        case MINUS_MINUS:   
            if (postOp)
                this->op = op == PLUS_PLUS ? UnaryOp::SINC : UnaryOp::SDEC;
            else
                this->op = op == PLUS_PLUS ? UnaryOp::PINC : UnaryOp::PDEC;
            break;
        default:
            this->op = (UnaryOp) op.type;
    }

    this->expr = std::move(expr);
    this->meta = this->expr->meta;
    
    if (postOp)
        this->meta.copyEnd(op);
    else {
        this->meta.fstLine = op.line;
        this->meta.fstCol = op.start;
    }
}

UnaryExpr::UnaryExpr(UnaryOp op, Expr::Ptr &expr) {
    this->op = op;
    this->expr = std::move(expr);
    this->meta = this->expr->meta;
}

AST UnaryExpr::getASTType() { return AST::UnaryExpr; }

// TODO: evaluate type
Fux::Type UnaryExpr::getFuxType() { return Fux::Type::NO_TYPE; }

/// BINARY EXPR ///

BinaryExpr::BinaryExpr(BinaryOp op, Expr::Ptr &LHS, Expr::Ptr &RHS) {
    this->op = op;
    this->LHS = std::move(LHS);
    this->RHS = std::move(RHS);
    this->meta = this->LHS->meta;
    if (this->RHS)
        this->meta.copyEnd(this->RHS->meta);
}

AST BinaryExpr::getASTType() { return AST::BinaryExpr; }

// TODO: evaluate type
Fux::Type BinaryExpr::getFuxType() { return Fux::Type::NO_TYPE; }

/// TYPE CAST ///

TypeCastExpr::TypeCastExpr(Fux::Type type, Expr::Ptr &expr) {
    this->type = type;
    this->expr = std::move(expr);
    this->meta = this->type.meta;
    this->meta.copyEnd(this->expr->meta);
}

AST TypeCastExpr::getASTType() { return AST::TypeCastExpr; }

Fux::Type TypeCastExpr::getFuxType() { return type; }

/// TERNARY EXPR ///

TernaryExpr::TernaryExpr(Expr::Ptr &condition, 
    Expr::Ptr &thenExpr, Expr::Ptr &elseExpr) {
        this->condition = std::move(condition);
        this->thenExpr = std::move(thenExpr);
        this->elseExpr = std::move(elseExpr);
        this->meta = this->condition->meta;
        this->meta.copyEnd(this->elseExpr->meta);
}

AST TernaryExpr::getASTType() { return AST::TernaryExpr; }

// TODO: evaluate
Fux::Type TernaryExpr::getFuxType() { return Fux::Type::NO_TYPE; }

/// NO OPERATION ///

AST NoOperationStmt::getASTType() { return AST::NoOperationStmt; }

/// VARIABLE DECLARATION ///

VariableStmt::~VariableStmt() { symbol.clear(); }

AST VariableStmt::getASTType() { return AST::VariableStmt; }

Fux::Type VariableStmt::getFuxType() { return type; }

string VariableStmt::getSymbol() { return symbol; }

Expr::Ptr &VariableStmt::getValue() { return value; }

/// INBUILT CALL ///

AST InbuiltCallStmt::getASTType() { return AST::InbuiltCallStmt; }

/// IF ELSE ///

AST IfElseStmt::getASTType() { return AST::IfElseStmt; }

/// CODE BLOCK ///

void BlockStmt::addSub(Stmt::Ptr &sub) { body.push_back(std::move(sub)); }

AST BlockStmt::getASTType() { return AST::BlockStmt; }

/// WHILE LOOP ///

AST WhileStmt::getASTType() { return AST::WhileStmt; }

/// FOR LOOP ///

AST ForStmt::getASTType() { return AST::ForStmt; }

/// ENUMERATION ///

AST EnumStmt::getASTType() { return AST::EnumStmt; }

Fux::Type EnumStmt::getFuxType() { return Fux::Type::U64; }

string EnumStmt::getSymbol() { return symbol; }

/// ROOT ///

AST Root::getASTType() { return AST::Root; }

string Root::getSymbol() { return "root"; }

void Root::addSub(Stmt::Ptr &sub) { program.push_back(std::move(sub)); }

/// PARENTS ///

Stmt::~Stmt() {}

AST Stmt::getASTType() { return AST::Stmt; }

Fux::Type Stmt::getFuxType() { return Fux::Type::NO_TYPE; }

string Stmt::getSymbol() { return string(); }

bool Stmt::isExpr() { 
    return (this->getASTType() >= AST::VariadicExpr 
        && this->getASTType() <= AST::Expr);  
}

/// OPERATIONS ///

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