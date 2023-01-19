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

StmtPtr Analyser::analyse() { return std::move(origin->analyse()); }

StmtPtr NumberExprAST::analyse() { return std::move(nullStmt); }

StmtPtr StringExprAST::analyse() { return std::move(nullStmt); }

StmtPtr VariableExprAST::analyse() { return std::move(nullStmt); }

StmtPtr MemberExprAST::analyse() { return std::move(nullStmt); } 

StmtPtr BinaryExprAST::analyse() { return std::move(nullStmt); }

StmtPtr ComparisonExprAST::analyse() { return std::move(nullStmt); }

StmtPtr LogicalExprAST::analyse() { return std::move(nullStmt); }

StmtPtr CallExprAST::analyse() { return std::move(nullStmt); }

StmtPtr AssignmentExprAST::analyse() { return std::move(nullStmt); }

StmtPtr VariableDeclAST::analyse() { return std::move(nullStmt); }

StmtPtr PutsCallAST::analyse() { return std::move(nullStmt); }

StmtPtr ReturnCallAST::analyse() { return std::move(nullStmt); }

StmtPtr IfElseAST::analyse() { return std::move(nullStmt); }

StmtPtr CodeBlockAST::analyse() { return std::move(nullStmt); }

StmtPtr PrototypeAST::analyse() { return std::move(nullStmt); }

StmtPtr FunctionAST::analyse() { return std::move(nullStmt); }

StmtPtr RootAST::analyse() { return std::move(nullStmt); }