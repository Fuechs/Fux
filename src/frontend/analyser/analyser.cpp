/**
 * @file analyser.cpp
 * @author fuechs
 * @brief fux ast analyser
 * @version 0.1
 * @date 2023-01-08
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "analyser.hpp"

StmtPtr Analyser::analyse() { return origin->analyse(); }

StmtPtr NullExprAST::analyse() { return nullptr; }

StmtPtr BoolExprAST::analyse() { return nullptr; }

StmtPtr NumberExprAST::analyse() { return nullptr; }

StmtPtr CharExprAST::analyse() { return nullptr; }

StmtPtr StringExprAST::analyse() { return nullptr; }

StmtPtr VariableExprAST::analyse() { return nullptr; }

StmtPtr MemberExprAST::analyse() { return nullptr; }

StmtPtr UnaryExprAST::analyse() { return nullptr; }

StmtPtr BinaryExprAST::analyse() { return nullptr; }

StmtPtr CallExprAST::analyse() { return nullptr; }

StmtPtr VariableDeclAST::analyse() { return nullptr; }

StmtPtr InbuiltCallAST::analyse() { return nullptr; }

StmtPtr IfElseAST::analyse() { return nullptr; }

StmtPtr CodeBlockAST::analyse() { return nullptr; }

StmtPtr PrototypeAST::analyse() { return nullptr; }

StmtPtr FunctionAST::analyse() { return nullptr; }

StmtPtr RootAST::analyse() { return nullptr; }