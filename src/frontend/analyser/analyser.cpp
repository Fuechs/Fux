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

StmtAST::Ptr Analyser::analyse() { return origin->analyse(Expectation(table)); }

StmtAST::Ptr NullExprAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr BoolExprAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr NumberExprAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr CharExprAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr StringExprAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr ArrayExprAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr VariableExprAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr MemberExprAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr CallExprAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr UnaryExprAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr BinaryExprAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr TypeCastExprAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr TernaryExprAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr VariableDeclAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr InbuiltCallAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr IfElseAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr CodeBlockAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr WhileLoopAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr ForLoopAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr PrototypeAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr FunctionAST::analyse(Expectation exp) { return nullptr; }

StmtAST::Ptr RootAST::analyse(Expectation exp) {
    RootAST::Ptr mod = make_unique<RootAST>();
    StmtAST::Ptr modStmt = nullptr;
    for (StmtAST::Ptr &stmt : program) 
        mod->addSub((modStmt = stmt->analyse(exp)));  
    return mod;
}