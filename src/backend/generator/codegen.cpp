/**
 * @file codegen.cpp
 * @author fuechs
 * @brief fux ast codegen functions
 * @version 0.1
 * @date 2023-01-19
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "../../frontend/ast/ast.hpp"
#include "generator.hpp"

Eisdrache::Local nullLocal = Eisdrache::Local();

#ifdef FUX_BACKEND

Eisdrache::Local &NoOperationAST::codegen(Eisdrache *eisdrache) { return nullLocal; }

Eisdrache::Local &NullExprAST::codegen(Eisdrache *eisdrache) { return  nullLocal; }

Eisdrache::Local &BoolExprAST::codegen(Eisdrache *eisdrache) { return value->getLLVMValue(eisdrache); }

Eisdrache::Local &NumberExprAST::codegen(Eisdrache *eisdrache) { return value->getLLVMValue(eisdrache); }

Eisdrache::Local &CharExprAST::codegen(Eisdrache *eisdrache) { return value->getLLVMValue(eisdrache); }

Eisdrache::Local &StringExprAST::codegen(Eisdrache *eisdrache) { return value->getLLVMValue(eisdrache); }

Eisdrache::Local &ArrayExprAST::codegen(Eisdrache *eisdrache) { return nullLocal; }

Eisdrache::Local &VariableExprAST::codegen(Eisdrache *eisdrache) {
    return eisdrache->getCurrentParent()[name];
}

Eisdrache::Local &MemberExprAST::codegen(Eisdrache *eisdrache) { return nullLocal; }

Eisdrache::Local &CallExprAST::codegen(Eisdrache *eisdrache) {
    Eisdrache::Local &calleeSymbol = callee->codegen(eisdrache);
    Eisdrache::Func &callee = *eisdrache->getFunc(calleeSymbol.getName());

    if ((*callee)->arg_size() != args.size())
        return nullLocal;

    Eisdrache::ValueVec arguments = {};
    for (ExprAST::Ptr &arg : args)
        arguments.push_back(arg->codegen(eisdrache).getValuePtr());
    
    return callee.call(arguments);
}

Eisdrache::Local &RangeExprAST::codegen(Eisdrache *eisdrache) { return nullLocal; }

Eisdrache::Local &UnaryExprAST::codegen(Eisdrache *eisdrache) { return nullLocal; }

Eisdrache::Local &BinaryExprAST::codegen(Eisdrache *eisdrache) {
    Eisdrache::Local &L = LHS->codegen(eisdrache);
    Eisdrache::Local &R = RHS->codegen(eisdrache);

    switch (op) {
        using enum BinaryOp;
        case ASG:   
            eisdrache->storeValue(L, R.loadValue());
            return L;
        case SWAPASG: {
            Eisdrache::Local &lVal = L.loadValue();
            Eisdrache::Local &rVal = R.loadValue();
            eisdrache->storeValue(L, rVal);
            eisdrache->storeValue(R, lVal);
            return L;
        }
        case ADD:           return eisdrache->binaryOp(Eisdrache::ADD, L, R);
        case SUB:           return eisdrache->binaryOp(Eisdrache::SUB, L, R);
        case MUL:           return eisdrache->binaryOp(Eisdrache::MUL, L, R);
        case DIV:           return eisdrache->binaryOp(Eisdrache::DIV, L, R);
        case BOR:           return eisdrache->binaryOp(Eisdrache::OR, L, R);
        case BXOR:          return eisdrache->binaryOp(Eisdrache::XOR, L, R);
        case BAND:          return eisdrache->binaryOp(Eisdrache::AND, L, R);
        case LSH:           return eisdrache->binaryOp(Eisdrache::LSH, L, R);
        case RSH:           return eisdrache->binaryOp(Eisdrache::RSH, L, R);
        case EQUAL:         return eisdrache->binaryOp(Eisdrache::EQU, L, R);
        case UNEQUAL:       return eisdrache->binaryOp(Eisdrache::NEQ, L, R);
        case LESST:         return eisdrache->binaryOp(Eisdrache::LES, L, R);
        case LTE:           return eisdrache->binaryOp(Eisdrache::LTE, L, R);
        case GREATERT:      return eisdrache->binaryOp(Eisdrache::GRE, L, R);
        case GTE:           return eisdrache->binaryOp(Eisdrache::GTE, L, R);
        default:            return nullLocal;
    }
}

Eisdrache::Local &TypeCastExprAST::codegen(Eisdrache *eisdrache) {
    return eisdrache->bitCast(expr->codegen(eisdrache), Generator::getType(eisdrache, type));
}

Eisdrache::Local &TernaryExprAST::codegen(Eisdrache *eisdrache) { return nullLocal; }

Eisdrache::Local &VariableDeclAST::codegen(Eisdrache *eisdrache) {
    Eisdrache::Ty *declTy = Generator::getType(eisdrache, type);
    Eisdrache::Local &decl = eisdrache->declareLocal(declTy, symbol);
    if (value)
        decl.setFuture(value->codegen(eisdrache).getValuePtr());
    return decl;
}

Eisdrache::Local &InbuiltCallAST::codegen(Eisdrache *eisdrache) {
    switch (callee) {
        using enum Inbuilts;
        case RETURN: {
            if (arguments.empty())
                eisdrache->createRet();
            else
                eisdrache->createRet(arguments.at(0)->codegen(eisdrache));
            return nullLocal;
        }
        case PUTS: {
            Eisdrache::Local &literal = arguments.at(0)->codegen(eisdrache);
            Eisdrache::Func *puts = nullptr;
            if (!(puts = eisdrache->getFunc("puts"))) {
                puts = &eisdrache->declareFunction(eisdrache->getSizeTy(), "puts",
                    {eisdrache->getUnsignedPtrTy(8)});
                llvm::dyn_cast<llvm::Argument>(puts->arg(0).getValuePtr())->addAttr(llvm::Attribute::NoCapture); 
            }
            return puts->call({literal.getValuePtr()});
        }
        default:        return nullLocal;
    }
}

Eisdrache::Local &IfElseAST::codegen(Eisdrache *eisdrache) { 
    Eisdrache::Local &cond = condition->codegen(eisdrache);
    llvm::BasicBlock *then = eisdrache->createBlock("then");
    llvm::BasicBlock *else_ = nullptr;
    llvm::BasicBlock *cond_leave = eisdrache->createBlock("cond_leave");
    if (elseBody)
        else_ = eisdrache->createBlock("else");
    eisdrache->jump(cond, then, else_ ? else_ : cond_leave);

    eisdrache->setBlock(then);
    thenBody->codegen(eisdrache);
    eisdrache->jump(cond_leave);

    if (else_) {
        eisdrache->setBlock(else_);
        elseBody->codegen(eisdrache);
        eisdrache->jump(cond_leave);
    }

    eisdrache->setBlock(cond_leave);
    return nullLocal;
}

Eisdrache::Local &CodeBlockAST::codegen(Eisdrache *eisdrache) {
    bool isEntry = (*eisdrache->getCurrentParent())->getBasicBlockList().empty();
    eisdrache->createBlock(isEntry ? "entry" : "", true);
    for (StmtAST::Ptr &stmt : body)
        stmt->codegen(eisdrache);
    if (!isEntry)
        eisdrache->createBlock("block_leave", true);
    return nullLocal;
}

Eisdrache::Local &WhileLoopAST::codegen(Eisdrache *eisdrache) { return nullLocal; }

Eisdrache::Local &ForLoopAST::codegen(Eisdrache *eisdrache) { return nullLocal; }

Eisdrache::Local &PrototypeAST::codegen(Eisdrache *eisdrache) {
    Eisdrache::Ty::Map params = {};
    for (StmtAST::Ptr &arg : args) {
        VariableDeclAST *arg_decl = dynamic_cast<VariableDeclAST *>(&*arg);
        params.push_back({arg_decl->getSymbol(), Generator::getType(eisdrache, arg_decl->getType())});
    }
    eisdrache->declareFunction(Generator::getType(eisdrache, type), symbol, params);
    return nullLocal;
}


Eisdrache::Local &FunctionAST::codegen(Eisdrache *eisdrache) {
    Eisdrache::Func *func = eisdrache->getFunc(proto->getSymbol());
    
    if (!func) {
        proto->codegen(eisdrache);
        func = eisdrache->getFunc(proto->getSymbol());
    }
    
    if (!func)  
        return nullLocal;

    eisdrache->setParent(func);
    body->codegen(eisdrache);
    eisdrache->verifyFunc(*func);

    return nullLocal;
}

Eisdrache::Local &RootAST::codegen(Eisdrache *eisdrache) {
    for (StmtAST::Ptr &stmt : program)
        stmt->codegen(eisdrache);
    return nullLocal;
}

#endif