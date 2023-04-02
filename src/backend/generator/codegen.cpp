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

#ifdef FUX_BACKEND

Eisdrache::Local nullLocal = Eisdrache::Local();

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
    // FIXME: can't get callee symbol as a local
    // Eisdrache::Local &calleeSymbol = callee->codegen(eisdrache);
    Eisdrache::Func &callee = *eisdrache->getFunc(callee->getSymbol());

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
        case CONASG: // TODO: analyser should check for constant locals
            eisdrache->storeValue(L, R.loadValue());
            return L;
        case ADDASG: 
            eisdrache->storeValue(L, eisdrache->binaryOp(Eisdrache::ADD, L, R, "addasg"));
            return L;
        case SUBASG: 
            eisdrache->storeValue(L, eisdrache->binaryOp(Eisdrache::SUB, L, R, "subasg"));
            return L;
        case MULASG: 
            eisdrache->storeValue(L, eisdrache->binaryOp(Eisdrache::MUL, L, R, "mulasg"));
            return L;
        case DIVASG: 
            eisdrache->storeValue(L, eisdrache->binaryOp(Eisdrache::DIV, L, R, "divasg"));
            return L;
        case POWASG: {
            Eisdrache::Func *pow = nullptr;

            if (!(pow = eisdrache->getFunc("pow"))) {
                Eisdrache::Func *prev = &eisdrache->getCurrentParent();
                pow = &eisdrache->declareFunction(eisdrache->getFloatTy(64), "pow",
                    {eisdrache->getFloatTy(64), eisdrache->getFloatTy(64)});
                (**pow)->setDoesNotThrow();
                (**pow)->setDoesNotAccessMemory();
                eisdrache->setParent(prev);
            }

            Eisdrache::Local &raw = pow->call({
                eisdrache->typeCast(L, eisdrache->getFloatTy(64)).getValuePtr(),
                R.loadValue().getValuePtr()}, "powasg");
            Eisdrache::Local &conv = eisdrache->typeCast(raw, (**L.getTy()), "powasg_conv");
            eisdrache->storeValue(L, conv);
            return L;
        }
        case MODASG: 
            eisdrache->storeValue(L, eisdrache->binaryOp(Eisdrache::MOD, L, R, "modasg"));
            return L;
        case BORASG: 
            eisdrache->storeValue(L, eisdrache->binaryOp(Eisdrache::OR, L, R, "borasg"));
            return L;
        case BANDASG:
            eisdrache->storeValue(L, eisdrache->binaryOp(Eisdrache::AND, L, R, "bandasg"));
            return L;
        case LSHASG:
            eisdrache->storeValue(L, eisdrache->binaryOp(Eisdrache::LSH, L, R, "lshasg"));
            return L;
        case RSHASG:
            eisdrache->storeValue(L, eisdrache->binaryOp(Eisdrache::RSH, L, R, "rshasg"));
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
        case POW: {
            Eisdrache::Func *pow = nullptr;

            if (!(pow = eisdrache->getFunc("pow"))) {
                Eisdrache::Func *prev = &eisdrache->getCurrentParent();
                pow = &eisdrache->declareFunction(eisdrache->getFloatTy(64), "pow",
                    {eisdrache->getFloatTy(64), eisdrache->getFloatTy(64)});
                (**pow)->setDoesNotThrow();
                (**pow)->setDoesNotAccessMemory();
                eisdrache->setParent(prev);
            }

            return pow->call({L.loadValue().getValuePtr(),
                R.loadValue().getValuePtr()}, "powtmp");
        }
        case MOD:           return eisdrache->binaryOp(Eisdrache::MOD, L, R);
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
    return eisdrache->typeCast(expr->codegen(eisdrache), Generator::getType(eisdrache, type), "typecast");
}

Eisdrache::Local &TernaryExprAST::codegen(Eisdrache *eisdrache) { return nullLocal; }

Eisdrache::Local &VariableDeclAST::codegen(Eisdrache *eisdrache) {
    return eisdrache->declareLocal(Generator::getType(eisdrache, type), symbol);
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
            Eisdrache::Func *cur = &eisdrache->getCurrentParent();
            Eisdrache::Func *puts = nullptr;

            if (!(puts = eisdrache->getFunc("puts"))) {
                puts = &eisdrache->declareFunction(eisdrache->getSizeTy(), "puts",
                    {eisdrache->getUnsignedPtrTy(8)});
                llvm::dyn_cast<llvm::Argument>(puts->arg(0).getValuePtr())->addAttr(llvm::Attribute::NoCapture);
                eisdrache->setParent(cur); 
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
    // figure out wether this is the entry block
    size_t blocks = (*eisdrache->getCurrentParent())->getBasicBlockList().size();
    llvm::BasicBlock *fst = &(*eisdrache->getCurrentParent())->getEntryBlock();
    bool isEntry = blocks == 0 || (blocks == 1 && fst->getName() == "locals");

    llvm::BasicBlock *that = eisdrache->createBlock(isEntry ? "entry" : string("block_")+to_string(blocks));
    
    if (blocks)
        eisdrache->jump(that);
    eisdrache->setBlock(that);
    
    for (StmtAST::Ptr &stmt : body)        
        stmt->codegen(eisdrache);

    if (!isEntry) { 
        that = eisdrache->createBlock(string("block_")+to_string(blocks)+"_leave"); 
        eisdrache->jump(that);
        eisdrache->setBlock(that);        
    }

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

    if (!locals.empty()) {
        eisdrache->createBlock("locals", true);
        for (StmtAST::Ptr &stmt : locals)
            stmt->codegen(eisdrache);
    }
    
    if (body->getASTType() != AST::CodeBlockAST) {
        llvm::BasicBlock *entry = eisdrache->createBlock("entry"); 
        if (!locals.empty())
            eisdrache->jump(entry);
        eisdrache->setBlock(entry);
    } 
    
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