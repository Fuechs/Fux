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

StmtAST::Ptr Analyser::analyse() { 
    current = new Scope();
    return origin->analyse(this); 
}

void Analyser::enter() { current = current->enter(); }

void Analyser::leave() { current = current->leave(); }

void Analyser::insert(Symbol *symbol) { current->insert(symbol); }

Symbol *Analyser::contains(string symbol) { return current->contains(symbol); }

template<typename Ast>
typename Ast::Ptr Analyser::process(StmtAST::Ptr &ptr) { 
    return (typename Ast::Ptr) dynamic_cast<Ast *>(&*ptr->analyse(this)); 
}

template<typename Ast>
typename Ast::Ptr Analyser::process(typename Ast::Ptr &ptr) {
    StmtAST::Ptr tmp = (StmtAST::Ptr) &*ptr;
    return process<Ast>(tmp);
}

string Analyser::mangleSymbol(const string &original, StmtAST::Ptr &link) {
    string ret = "";

    switch (link->getASTType()) {
        default: 
            assert(!"Analyser::mangleSymbol(): AST type not implemented.");
    }
    
    return ret;
}

StmtAST::Ptr NoOperationAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr NullExprAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr BoolExprAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr NumberExprAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr CharExprAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr StringExprAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr RangeExprAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr ArrayExprAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr VariableExprAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr MemberExprAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr CallExprAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr UnaryExprAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr BinaryExprAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr TypeCastExprAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr TernaryExprAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr VariableDeclAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr InbuiltCallAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr IfElseAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr CodeBlockAST::analyse(Analyser *analyser) {
    analyser->enter();

    StmtAST::Vec body = {};
    for (StmtAST::Ptr &stmt : body)
        body.push_back(stmt->analyse(analyser));
    
    analyser->leave();
    return make_unique<CodeBlockAST>(body);
}

StmtAST::Ptr WhileLoopAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr ForLoopAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr PrototypeAST::analyse(Analyser *analyser) {
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr FunctionAST::analyse(Analyser *analyser) {
    proto = analyser->process<PrototypeAST>(proto);
    body = body->analyse(analyser);
    return (StmtAST::Ptr) this;
}

StmtAST::Ptr EnumerationAST::analyse(Analyser *analyser) { 
    return (StmtAST::Ptr) this; 
}

StmtAST::Ptr MacroAST::analyse(Analyser *analyser) { 
    return (StmtAST::Ptr) this; 
}

StmtAST::Ptr RootAST::analyse(Analyser *analyser) {
    RootAST::Ptr mod = make_unique<RootAST>();
    StmtAST::Ptr modStmt = nullptr;
    for (StmtAST::Ptr &stmt : program) 
        mod->addSub((modStmt = stmt->analyse(analyser)));  
    return mod;
}