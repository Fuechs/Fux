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

StmtAST::Ptr Analyser::analyse() { return origin->analyse(this); }

Table &Analyser::getTable() { return table; }

StmtAST::Ptr NoOperationAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr NullExprAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr BoolExprAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr NumberExprAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr CharExprAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr StringExprAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr RangeExprAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr ArrayExprAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr VariableExprAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr MemberExprAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr CallExprAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr UnaryExprAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr BinaryExprAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr TypeCastExprAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr TernaryExprAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr VariableDeclAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr InbuiltCallAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr IfElseAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr CodeBlockAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr WhileLoopAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr ForLoopAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr PrototypeAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr FunctionAST::analyse(Analyser *analyser) {
    proto = (PrototypeAST::Ptr) dynamic_cast<PrototypeAST *>(&*proto->analyse(analyser));
    body = body->analyse(analyser);
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr EnumerationAST::analyse(Analyser *analyser) { return nullptr; }

StmtAST::Ptr RootAST::analyse(Analyser *analyser) {
    RootAST::Ptr mod = make_unique<RootAST>();
    StmtAST::Ptr modStmt = nullptr;
    for (StmtAST::Ptr &stmt : program) 
        mod->addSub((modStmt = stmt->analyse(analyser)));  
    return mod;
}