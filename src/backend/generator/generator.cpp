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
    initializeModule();

    root->codegen(builder, module, namedValues);

    if (fux.options.debugMode)
        module->print(errs(), nullptr);
    
    delete this;
}

void Generator::initializeModule() {
    delete context; 
    delete module;
    delete builder;
    context = new LLVMContext();
    module = new Module("fux compiler", *context);
    builder = new IRBuilder<>(*context);
}

Value *RootAST::codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) {
    for (ExprPtr &sub : program)
        sub->codegen(builder, module, namedValues);
    return nullptr;
}

Value *NumberExprAST::codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) {
    return ConstantFP::get(builder->getContext(), APFloat(value));
}

Value *VariableExprAST::codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) {
    Value *V = namedValues[name];
    if (!V)
        return nullptr;
    return V;
}

Value *BinaryExprAST::codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) {
    Value *L = LHS->codegen(builder, module, namedValues);
    Value *R = RHS->codegen(builder, module, namedValues);
    if (!L || !R)
        return nullptr;

    switch (op) {
        case '+':   return builder->CreateFAdd(L, R, "addtmp");
        case '-':   return builder->CreateFSub(L, R, "subtmp");
        case '*':   return builder->CreateFMul(L, R, "multmp");
        case '/':   return builder->CreateFDiv(L, R, "divtmp");
        default:    return nullptr;
    }
}

Value *CallExprAST::codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) {
    Function *calleeFunc = module->getFunction(callee);
    if (!calleeFunc)
        return nullptr;
    
    if (calleeFunc->arg_size() != args.size())
        return nullptr;

    ValueList argList;
    for (size_t i = 0, e = args.size(); i != e; ++i) {
        argList.push_back(args[i]->codegen(builder, module, namedValues));
        if (!argList.back())    
            return nullptr;
    }

    return builder->CreateCall(calleeFunc, argList, "calltmp");
} 

Function *PrototypeAST::codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) {
    TypeList doubles(args.size(), builder->getDoubleTy());
    FunctionType *funcType = FunctionType::get(builder->getDoubleTy(), doubles, false);
    Function *func = Function::Create(funcType, Function::ExternalLinkage, name, *module);
    
    size_t idx = 0;
    for (auto &arg : func->args())
        arg.setName(args[idx++]);
    
    return func;
}

Function *FunctionAST::codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) {
    Function *func = module->getFunction(proto->getName());  
    if (!func)  func = proto->codegen(builder, module, namedValues);
    if (!func)  return nullptr; 
    
    if (!func->empty()) 
        return nullptr; // cannot be redefined

    BasicBlock *BB = BasicBlock::Create(builder->getContext(), "entry", func);
    builder->SetInsertPoint(BB);

    namedValues.clear();
    for (auto &arg : func->args())
        namedValues[arg.getName().str()] = &arg;

    if (Value *retVal = body->codegen(builder, module, namedValues)) {
        builder->CreateRet(retVal);
        verifyFunction(*func);
        return func;
    }

    // error reading body
    func->eraseFromParent();
    return nullptr;
}