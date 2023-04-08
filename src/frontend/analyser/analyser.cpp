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

#ifdef FUX_ANALYSER

Stmt::Ptr Analyser::analyse() { 
    current = new Scope();
    return origin->analyse(this); 
}

void Analyser::enter() { current = current->enter(); }

void Analyser::leave() { current = current->leave(); }

void Analyser::insert(Symbol *symbol) { current->insert(symbol); }

Symbol *Analyser::contains(string symbol) { return current->contains(symbol); }

template<typename To, typename From>
typename To::Ptr Analyser::cast(From &ptr) {
    return (typename To::Ptr) dynamic_cast<To *>(ptr.get());
}

template<typename To, typename From>
typename To::Ptr Analyser::process(From &ptr) { 
    Stmt::Ptr tmp = ptr->analyse(this);
    return cast<To>(tmp); 
}

string Analyser::mangleSymbol(Stmt::Ptr &link) {
    if (link->isExpr())
        assert(!"Analyser::mangleSymbol(): Linked AST is not a statement, but an expression.");
    
    string ret = "";

    switch (link->getASTType()) {
        case AST::PrototypeStmt:
            ret += link->getSymbol() + "_" + link->getFuxType().mangledString();
            for (Stmt::Ptr &param : dynamic_cast<PrototypeStmt *>(link.get())->getArgs())
                ret += "_" + param->getFuxType().mangledString();
            break;
        default: 
            assert(!"Analyser::mangleSymbol(): Type of linked AST is not implemented.");
    }
    
    return ret;
}

Stmt::Ptr VariadicExpr::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr NullExpr::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr BoolExpr::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr NumberExpr::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr CharExpr::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr StringExpr::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr RangeExpr::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr ArrayExpr::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr SymbolExpr::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr MemberExpr::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr CallExpr::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr UnaryExpr::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr BinaryExpr::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr TypeCastExpr::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr TernaryExpr::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr NoOperationStmt::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr VariableStmt::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr InbuiltCallStmt::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr IfElseStmt::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr BlockStmt::analyse(Analyser *analyser) {
    analyser->enter();

    Stmt::Vec body = {};
    for (Stmt::Ptr &stmt : body)
        body.push_back(stmt->analyse(analyser));
    
    analyser->leave();
    return make_unique<BlockStmt>(body);
}

Stmt::Ptr WhileStmt::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr ForStmt::analyse(Analyser *analyser) {
    return (Stmt::Ptr) this;
}

Stmt::Ptr PrototypeStmt::analyse(Analyser *analyser) {
    Stmt::Ptr mod = (Stmt::Ptr) this;
    const string sym = analyser->mangleSymbol(mod);

    if (analyser->contains(sym))
        // TODO: duplicate declaration error...
        return mod;
    
    FuxType::Vec types = {};
    for (Stmt::Ptr &arg : args) {
        VariableStmt::Ptr mod_arg = analyser->process<VariableStmt>(arg);
        if (mod_arg)
            types.push_back(mod_arg->getFuxType());
    }
    
    // TODO: parent
    analyser->insert(new Function(type, sym, types, meta));
    return mod;
}

Stmt::Ptr FunctionStmt::analyse(Analyser *analyser) {
    proto = analyser->process<PrototypeStmt>(proto);
    body = body->analyse(analyser);
    return (Stmt::Ptr) this;
}

Stmt::Ptr EnumStmt::analyse(Analyser *analyser) { 
    return (Stmt::Ptr) this; 
}

Stmt::Ptr MacroStmt::analyse(Analyser *analyser) { 
    return (Stmt::Ptr) this; 
}

Stmt::Ptr Root::analyse(Analyser *analyser) {
    Root::Ptr mod = make_unique<Root>();
    Stmt::Ptr modStmt = nullptr;
    for (Stmt::Ptr &stmt : program) 
        mod->addSub((modStmt = stmt->analyse(analyser)));  
    return mod;
}

#endif