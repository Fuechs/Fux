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

/// NULL EXPR ///

AST NullExprAST::getASTType() { return AST::NullExprAST; }

FuxType NullExprAST::getFuxType() { return FuxType::NO_TYPE; }

/// BOOL EXPR ///

BoolExprAST::~BoolExprAST() { delete value; }

AST BoolExprAST::getASTType() { return AST::BoolExprAST; }

FuxType BoolExprAST::getFuxType() { return FuxType::BOOL; }

/// NUMBER EXPR ///

NumberExprAST::~NumberExprAST() { delete value; }

AST NumberExprAST::getASTType() { return AST::NumberExprAST; }

FuxType NumberExprAST::getFuxType() { return value->type; }

/// CHAR EXPR ///

CharExprAST::~CharExprAST() { delete value; }

AST CharExprAST::getASTType() { return AST::CharExprAST; }

FuxType CharExprAST::getFuxType() { return value->type;  }

/// STRING EXPR ///

StringExprAST::~StringExprAST() { delete value; }

AST StringExprAST::getASTType() { return AST::StringExprAST; }

FuxType StringExprAST::getFuxType() { return FuxType::LIT; }

/// RANGE EXPR ///

AST RangeExprAST::getASTType() { return AST::RangeExprAST; }

// TODO: combine begin & end type
FuxType RangeExprAST::getFuxType() { return end->getFuxType(); }

/// ARRAY EXPR ///

AST ArrayExprAST::getASTType() { return AST::ArrayExprAST; }

// TODO: get element types for this
FuxType ArrayExprAST::getFuxType() { return FuxType::createArray(FuxType::NO_TYPE);  }

/// VARIABLE EXPR ///

VariableExprAST::~VariableExprAST() { name.clear(); }

AST VariableExprAST::getASTType() { return AST::VariableExprAST; }

FuxType VariableExprAST::getFuxType() { return FuxType::NO_TYPE; }

string VariableExprAST::getSymbol() { return name; }

/// CALL EXPR ///

CallExprAST::CallExprAST(const string &callee, ExprAST::Vec &args, bool asyncCall) {
    this->callee = make_unique<VariableExprAST>(callee);
    this->args = std::move(args);
    this->asyncCall = asyncCall;
}

CallExprAST::CallExprAST(ExprAST::Ptr &callee, ExprAST::Vec &args, bool asyncCall) {
    this->callee = std::move(callee);
    this->args = std::move(args);
    this->asyncCall = asyncCall;
    this->meta = this->callee->meta;
}

AST CallExprAST::getASTType() { return AST::StringExprAST; }

// TODO: evaluate type
FuxType CallExprAST::getFuxType() { return FuxType::NO_TYPE; }

string CallExprAST::getSymbol() { return callee->getSymbol(); }

/// MEMBER EXPR ///

MemberExprAST::MemberExprAST(ExprAST::Ptr &parent, const Token &member) {
    this->parent = std::move(parent);
    this->member = member.value;
    this->meta = this->parent->meta;
    this->meta.copyEnd(member);
}

MemberExprAST::MemberExprAST(ExprAST::Ptr &parent, const string &member) {
    this->parent = std::move(parent);
    this->member = member;
    this->meta = this->parent->meta;
}

AST MemberExprAST::getASTType() { return AST::MemberExprAST; }

FuxType MemberExprAST::getFuxType() { return FuxType::NO_TYPE; }

string MemberExprAST::getSymbol() { return parent->getSymbol()+"_"+member; }

/// UNARY EXPR ///

UnaryExprAST::UnaryExprAST(const Token &op, ExprAST::Ptr &expr, bool postOp) {
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

UnaryExprAST::UnaryExprAST(UnaryOp op, ExprAST::Ptr &expr) {
    this->op = op;
    this->expr = std::move(expr);
    this->meta = this->expr->meta;
}

AST UnaryExprAST::getASTType() { return AST::UnaryExprAST; }

// TODO: evaluate type
FuxType UnaryExprAST::getFuxType() { return FuxType::NO_TYPE; }

/// BINARY EXPR ///

BinaryExprAST::BinaryExprAST(BinaryOp op, ExprAST::Ptr &LHS, ExprAST::Ptr &RHS) {
    this->op = op;
    this->LHS = std::move(LHS);
    this->RHS = std::move(RHS);
    this->meta = this->LHS->meta;
    if (this->RHS)
        this->meta.copyEnd(this->RHS->meta);
}

AST BinaryExprAST::getASTType() { return AST::BinaryExprAST; }

// TODO: evaluate type
FuxType BinaryExprAST::getFuxType() { return FuxType::NO_TYPE; }

/// TYPE CAST ///

TypeCastExprAST::TypeCastExprAST(FuxType type, ExprAST::Ptr &expr) {
    this->type = type;
    this->expr = std::move(expr);
    this->meta = this->type.meta;
    this->meta.copyEnd(this->expr->meta);
}

AST TypeCastExprAST::getASTType() { return AST::TypeCastExprAST; }

FuxType TypeCastExprAST::getFuxType() { return type; }

/// TERNARY EXPR ///

TernaryExprAST::TernaryExprAST(ExprAST::Ptr &condition, 
    ExprAST::Ptr &thenExpr, ExprAST::Ptr &elseExpr) {
        this->condition = std::move(condition);
        this->thenExpr = std::move(thenExpr);
        this->elseExpr = std::move(elseExpr);
        this->meta = this->condition->meta;
        this->meta.copyEnd(this->elseExpr->meta);
}

AST TernaryExprAST::getASTType() { return AST::TernaryExprAST; }

FuxType TernaryExprAST::getFuxType() { return FuxType::BOOL; }

/// NO OPERATION ///

AST NoOperationAST::getASTType() { return AST::NoOperationAST; }

FuxType NoOperationAST::getFuxType() { return FuxType::NO_TYPE; }

/// VARIABLE DECLARATION ///

VariableDeclAST::~VariableDeclAST() { symbol.clear(); }

AST VariableDeclAST::getASTType() { return AST::VariableDeclAST; }

FuxType VariableDeclAST::getFuxType() { return type; }

string VariableDeclAST::getSymbol() { return symbol; }

FuxType &VariableDeclAST::getType() { return type; }

ExprAST::Ptr &VariableDeclAST::getValue() { return value; }

/// INBUILT CALL ///

AST InbuiltCallAST::getASTType() { return AST::InbuiltCallAST; }

FuxType InbuiltCallAST::getFuxType() { return FuxType::NO_TYPE; }

/// IF ELSE ///

AST IfElseAST::getASTType() { return AST::IfElseAST; }

FuxType IfElseAST::getFuxType() { return FuxType::NO_TYPE; }

/// CODE BLOCK ///

void CodeBlockAST::addSub(StmtAST::Ptr &sub) { body.push_back(std::move(sub)); }

AST CodeBlockAST::getASTType() { return AST::CodeBlockAST; }

FuxType CodeBlockAST::getFuxType() { return FuxType::NO_TYPE; }

/// WHILE LOOP ///

AST WhileLoopAST::getASTType() { return AST::WhileLoopAST; }

FuxType WhileLoopAST::getFuxType() { return FuxType::NO_TYPE; }

/// FOR LOOP ///

AST ForLoopAST::getASTType() { return AST::ForLoopAST; }

FuxType ForLoopAST::getFuxType() { return FuxType::NO_TYPE; }

/// PROTOTYPE ///

PrototypeAST::~PrototypeAST() { args.clear(); }

AST PrototypeAST::getASTType() { return AST::PrototypeAST; }

FuxType PrototypeAST::getFuxType() { return type; }

string PrototypeAST::getSymbol() { return symbol; }

StmtAST::Vec &PrototypeAST::getArgs() { return args; }

/// FUNCTION ///

AST FunctionAST::getASTType() { return AST::FunctionAST; }

FuxType FunctionAST::getFuxType() { return proto->getFuxType(); }

string FunctionAST::getSymbol() { return proto->getSymbol(); }

void FunctionAST::setBody(StmtAST::Ptr &body) { this->body = std::move(body); }

void FunctionAST::addLocal(StmtAST::Ptr &local) { locals.push_back(std::move(local)); }

/// ENUMERATION ///

AST EnumerationAST::getASTType() { return AST::EnumerationAST; }

FuxType EnumerationAST::getFuxType() { return FuxType::U64; }

string EnumerationAST::getSymbol() { return symbol; }

/// MACRO ///

MacroAST::Arg &MacroAST::Arg::operator=(const MacroAST::Arg &copy) {
    symbol = copy.symbol;
    type = copy.type;
    meta = copy.meta;
    return *this;
}

AST MacroAST::getASTType() { return AST::MacroAST; }

// impossible to evaluate due to multiple cases
FuxType MacroAST::getFuxType() { return FuxType::NO_TYPE; }

string MacroAST::getSymbol() { return symbol; }

/// ROOT ///

AST RootAST::getASTType() { return AST::RootAST; }

FuxType RootAST::getFuxType() { return FuxType::NO_TYPE; }

string RootAST::getSymbol() { return "root"; }

void RootAST::addSub(StmtAST::Ptr &sub) { program.push_back(std::move(sub)); }

_i64 RootAST::addSizeExpr(ExprAST::Ptr &sizeExpr) {
    arraySizeExprs.push_back(std::move(sizeExpr));
    return arraySizeExprs.size() - 1;
}

/// PARENTS ///

string StmtAST::getSymbol() { return string(); }

bool StmtAST::isExpr() { 
    return (this->getASTType() >= AST::NullExprAST 
        && this->getASTType() <= AST::TernaryExprAST);  
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