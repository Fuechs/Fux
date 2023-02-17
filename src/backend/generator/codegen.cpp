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

Value *NullExprAST::codegen(LLVMWrapper *fuxLLVM) { return nullptr; }

Value *BoolExprAST::codegen(LLVMWrapper *fuxLLVM) { return value->getLLVMValue(fuxLLVM); }

Value *NumberExprAST::codegen(LLVMWrapper *fuxLLVM) { return value->getLLVMValue(fuxLLVM); }

Value *CharExprAST::codegen(LLVMWrapper *fuxLLVM) { return value->getLLVMValue(fuxLLVM); }

Value *StringExprAST::codegen(LLVMWrapper *fuxLLVM) { return value->getLLVMValue(fuxLLVM); }

Value *ArrayExprAST::codegen(LLVMWrapper *fuxLLVM) { return nullptr; }

Value *VariableExprAST::codegen(LLVMWrapper *fuxLLVM) { 
    // FuxValue &found = fuxLLVM->values[name];
    // Value *ret = fuxLLVM->builder->CreateLoad(found.type, found.value, name+"_LOAD");
    return fuxLLVM->values[name].value;
}

Value *MemberExprAST::codegen(LLVMWrapper *fuxLLVM) { return nullptr; }

Value *CallExprAST::codegen(LLVMWrapper *fuxLLVM) {
    // Function *calleeFunc = callee->codegen(fuxLLVM);
    // if (!calleeFunc)
        // return nullptr;
    
    // if (calleeFunc->arg_size() != args.size())
        // return nullptr;

    // ValueList argList;
    // for (size_t i = 0, e = args.size(); i != e; ++i) {
    //     argList.push_back(args[i]->codegen(fuxLLVM));
    //     if (!argList.back())    
    //         return nullptr;
    // }

    // return fuxLLVM->builder->CreateCall(calleeFunc, argList, "calltmp");
    return nullptr;
} 

Value *RangeExprAST::codegen(LLVMWrapper *fuxLLVM) { return nullptr; }

Value *UnaryExprAST::codegen(LLVMWrapper *fuxLLVM) { return nullptr; }

Value *BinaryExprAST::codegen(LLVMWrapper *fuxLLVM) {
    Value *L = LHS->codegen(fuxLLVM);
    Value *R = RHS->codegen(fuxLLVM);
    if (!L || !R)
        return nullptr;

    switch (op) {
        using enum BinaryOp;
        case ASG:   return fuxLLVM->builder->CreateStore(R, L);
        case SWAPASG: {
            Value *lVal = fuxLLVM->loadValue(L);
            Value *rVal = fuxLLVM->loadValue(R);
            fuxLLVM->builder->CreateStore(rVal, L);
            fuxLLVM->builder->CreateStore(lVal, R);
            return L;
        }
        case ADD: 
            L = fuxLLVM->loadValue(L);
            R = fuxLLVM->loadValue(R);
            return fuxLLVM->builder->CreateAdd(L, R, "addtmp");
        case SUB:   
            L = fuxLLVM->loadValue(L);
            R = fuxLLVM->loadValue(R);
            return fuxLLVM->builder->CreateSub(L, R, "subtmp");
        case MUL:  
            L = fuxLLVM->loadValue(L);
            R = fuxLLVM->loadValue(R);
            return fuxLLVM->builder->CreateMul(L, R, "multmp");
        case DIV:   
            L = fuxLLVM->loadValue(L);
            R = fuxLLVM->loadValue(R);
            return fuxLLVM->builder->CreateFDiv(L, R, "divtmp");
        default:    return nullptr;
    }
}

Value *TypeCastExprAST::codegen(LLVMWrapper *fuxLLVM) {
    Value *valToCast = expr->codegen(fuxLLVM);
    return fuxLLVM->builder->CreateZExt(valToCast, Generator::getType(fuxLLVM, type));
}

Value *TernaryExprAST::codegen(LLVMWrapper *fuxLLVM) { return nullptr; }

Value *VariableDeclAST::codegen(LLVMWrapper *fuxLLVM) {
    Type *llvmType = Generator::getType(fuxLLVM, type);
    Value *that = fuxLLVM->builder->CreateAlloca(llvmType, 0, symbol);
    if (value) 
        fuxLLVM->builder->CreateStore(value->codegen(fuxLLVM), that);
    fuxLLVM->values[symbol] = FuxValue(llvmType, that);
    return that;
}

Value *InbuiltCallAST::codegen(LLVMWrapper *fuxLLVM) {
    switch (callee) {
        using enum Inbuilts;
        case RETURN: {
            Value *ret = arguments.at(0)->codegen(fuxLLVM);
            ret = fuxLLVM->loadValue(ret);
            return fuxLLVM->builder->CreateRet(ret);
        }
        case PUTS: {
            Value *literal = arguments.at(0)->codegen(fuxLLVM);
            literal = fuxLLVM->loadValue(literal);
            return fuxLLVM->builder->CreateCall(fuxLLVM->posix_puts, {literal});
        }
        default:        return nullptr;
    }
}

Value *IfElseAST::codegen(LLVMWrapper *fuxLLVM) {
    return nullptr;
}

Value *CodeBlockAST::codegen(LLVMWrapper *fuxLLVM) {
    Value *ret = nullptr;
    for (StmtAST::Ptr &stmt : body)
        ret = stmt->codegen(fuxLLVM);
    return ret;
}

Value *WhileLoopAST::codegen(LLVMWrapper *fuxLLVM) { return nullptr; }

Value *ForLoopAST::codegen(LLVMWrapper *fuxLLVM) { return nullptr; }

Function *PrototypeAST::codegen(LLVMWrapper *fuxLLVM) {
    TypeList paramTypes = TypeList();
    for (StmtAST::Ptr &param : args) 
        paramTypes.push_back(Generator::getType(fuxLLVM, param->getFuxType()));
    FunctionType *funcType = FunctionType::get(Generator::getType(fuxLLVM, type), paramTypes, false);
    Function *func = Function::Create(funcType, Function::ExternalLinkage, symbol == "main" ? "main" : "Usr_"+symbol, *fuxLLVM->module);
    return func;
}

Function *FunctionAST::codegen(LLVMWrapper *fuxLLVM) {
    Function *func = fuxLLVM->module->getFunction(proto->getSymbol());  
    if (!func)  func = proto->codegen(fuxLLVM);
    if (!func)  return nullptr; 
    
    if (!func->empty()) 
        return nullptr; // cannot be redefined

    BasicBlock *BB = BasicBlock::Create(*fuxLLVM->context, "entry", func);
    fuxLLVM->builder->SetInsertPoint(BB);

    fuxLLVM->values.clear();
    for (auto &arg : func->args())
        fuxLLVM->values[arg.getName().str()] = FuxValue(arg.getType(), &arg);

    Value *retVal = body->codegen(fuxLLVM);

    if (!retVal) { // error reading body
        func->eraseFromParent();
        return nullptr;
    }
    
    if (func->getReturnType()->isVoidTy()) 
        fuxLLVM->builder->CreateRetVoid();
    
    verifyFunction(*func);
    return func;
}

Value *RootAST::codegen(LLVMWrapper *fuxLLVM) {
    for (StmtAST::Ptr &sub : program)
        sub->codegen(fuxLLVM);
    return nullptr;
}

#endif