/**
 * @file generator.cpp
 * @author fuechs
 * @brief fux ir generator
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "generator.hpp"


void Generator::generate() {
    root->codegen(fuxLLVM);

    if (fux.options.debugMode)
        fuxLLVM->module->print(errs(), nullptr);
    
    delete this;
}

// *::codegen()

Value *RootAST::codegen(LLVMWrapper *fuxLLVM) {
    for (ExprPtr &sub : program)
        sub->codegen(fuxLLVM);
    return nullptr;
}

Value *NumberExprAST::codegen(LLVMWrapper *fuxLLVM) {
    return fuxLLVM->builder->getInt32((_i32) value);
}

Value *VariableExprAST::codegen(LLVMWrapper *fuxLLVM) {
    Value *V = fuxLLVM->namedValues[name];
    if (!V)
        return nullptr;
    return V;
}

Value *BinaryExprAST::codegen(LLVMWrapper *fuxLLVM) {
    Value *L = LHS->codegen(fuxLLVM);
    Value *R = RHS->codegen(fuxLLVM);
    if (!L || !R)
        return nullptr;

    switch (op) {
        case '+':   return fuxLLVM->builder->CreateAdd(L, R, "addtmp");
        case '-':   return fuxLLVM->builder->CreateSub(L, R, "subtmp");
        case '*':   return fuxLLVM->builder->CreateMul(L, R, "multmp");
        case '/':   return fuxLLVM->builder->CreateFDiv(L, R, "divtmp");
        default:    return nullptr;
    }
}

Value *ComparisonExprAST::codegen(LLVMWrapper *fuxLLVM) { return nullptr; }
Value *LogicalExprAST::codegen(LLVMWrapper *fuxLLVM) { return nullptr; }

Value *CallExprAST::codegen(LLVMWrapper *fuxLLVM) {
    Function *calleeFunc = fuxLLVM->module->getFunction(callee);
    if (!calleeFunc)
        return nullptr;
    
    if (calleeFunc->arg_size() != args.size())
        return nullptr;

    ValueList argList;
    for (size_t i = 0, e = args.size(); i != e; ++i) {
        argList.push_back(args[i]->codegen(fuxLLVM));
        if (!argList.back())    
            return nullptr;
    }

    return fuxLLVM->builder->CreateCall(calleeFunc, argList, "calltmp");
} 

Value *VariableDeclAST::codegen(LLVMWrapper *fuxLLVM) { return nullptr; }

Function *PrototypeAST::codegen(LLVMWrapper *fuxLLVM) {
    TypeList argTypes(args.size(), fuxLLVM->builder->getInt32Ty());
    FunctionType *funcType;
    switch (type) {
        case fuxType::VOID: funcType = FunctionType::get(fuxLLVM->builder->getVoidTy(), argTypes, false); break;
        case fuxType::I32:  funcType = FunctionType::get(fuxLLVM->builder->getInt32Ty(), argTypes, false); break;
        case fuxType::F64:  funcType = FunctionType::get(fuxLLVM->builder->getDoubleTy(), argTypes, false); break;
        default:            return nullptr;
    }
    Function *func = Function::Create(funcType, Function::ExternalLinkage, name, *fuxLLVM->module);
    
    auto it = args.begin();
    for (auto &arg : func->args())
        arg.setName(it++->first);
    
    return func;
}

Function *FunctionAST::codegen(LLVMWrapper *fuxLLVM) {
    Function *func = fuxLLVM->module->getFunction(proto->getName());  
    if (!func)  func = proto->codegen(fuxLLVM);
    if (!func)  return nullptr; 
    
    if (!func->empty()) 
        return nullptr; // cannot be redefined

    BasicBlock *BB = BasicBlock::Create(fuxLLVM->builder->getContext(), "entry", func);
    fuxLLVM->builder->SetInsertPoint(BB);

    fuxLLVM->namedValues.clear();
    for (auto &arg : func->args())
        fuxLLVM->namedValues[arg.getName().str()] = &arg;

    Value *retVal;
    for (ExprPtr &expr : body)
        retVal = expr->codegen(fuxLLVM);
    
    if (func->getReturnType()->isVoidTy()) {
        fuxLLVM->builder->CreateRetVoid();
        return func;
    }
    
    if (retVal) {
        fuxLLVM->builder->CreateRet(retVal);
        verifyFunction(*func);
        return func;
    }

    // error reading body
    func->eraseFromParent();
    return nullptr;
}