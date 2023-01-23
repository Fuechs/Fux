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

#ifdef FUX_BACKEND

Value *NullExprAST::codegen(LLVMWrapper *fuxLLVM) { return value->getLLVMValue(fuxLLVM); }

Value *BoolExprAST::codegen(LLVMWrapper *fuxLLVM) { return value->getLLVMValue(fuxLLVM); }

Value *NumberExprAST::codegen(LLVMWrapper *fuxLLVM) { return value->getLLVMValue(fuxLLVM); }

Value *CharExprAST::codegen(LLVMWrapper *fuxLLVM) { return value->getLLVMValue(fuxLLVM); }

Value *StringExprAST::codegen(LLVMWrapper *fuxLLVM) { return value->getLLVMValue(fuxLLVM); }

Value *VariableExprAST::codegen(LLVMWrapper *fuxLLVM) {
    Value *V = fuxLLVM->namedValues[name];
    if (!V)
        return nullptr;
    return V;
}

Value *MemberExprAST::codegen(LLVMWrapper *fuxLLVM) { return nullptr; }

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

Value *AssignmentExprAST::codegen(LLVMWrapper *fuxLLVM) { return nullptr; }

Value *VariableDeclAST::codegen(LLVMWrapper *fuxLLVM) { return nullptr; }

Value *PutsCallAST::codegen(LLVMWrapper *fuxLLVM) {
    Value *arg = argument->codegen(fuxLLVM);
    // if (arg->getType() != fuxLLVM->fuxStr->str)
        return nullptr;
    // return fuxLLVM->builder->CreateCall(fuxLLVM->fuxIO->puts, {arg});
}

Value *ReturnCallAST::codegen(LLVMWrapper *fuxLLVM) { return nullptr; }

Value *IfElseAST::codegen(LLVMWrapper *fuxLLVM) {
    return nullptr;
}

Value *CodeBlockAST::codegen(LLVMWrapper *fuxLLVM) { return nullptr; }

Function *PrototypeAST::codegen(LLVMWrapper *fuxLLVM) {
    TypeList argTypes(args.size(), fuxLLVM->builder->getInt32Ty());
    FunctionType *funcType /*= FunctionType::get(Generator::getType(fuxLLVM, type), argTypes, false)*/ ;
    Function *func = Function::Create(funcType, Function::ExternalLinkage, "Usr_"+name, *fuxLLVM->module);
    
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

    // FIXME: This does not work as expected, 
    // body of the function is missing in the IR
    Value *retVal = body->codegen(fuxLLVM);
    
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

Value *RootAST::codegen(LLVMWrapper *fuxLLVM) {
    for (StmtPtr &sub : program)
        sub->codegen(fuxLLVM);
    return nullptr;
}

#endif