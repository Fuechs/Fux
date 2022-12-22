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

void Generator::debugPrint(const string message) {
    cout << getDebugText() << "Generator";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

Value *RootAST::codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) {
    for (ExprPtr &sub : program)
        sub->codegen(builder, module, namedValues);
    return nullptr;
}

Value *NumberExprAST::codegen(IRBuilder<> *builder, Module *module, ValueMap &namedValues) {
    return builder->getInt32((_i32) value);
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
        case '+':   return builder->CreateAdd(L, R, "addtmp");
        case '-':   return builder->CreateSub(L, R, "subtmp");
        case '*':   return builder->CreateMul(L, R, "multmp");
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
    TypeList argTypes(args.size(), builder->getInt32Ty());
    FunctionType *funcType;
    switch (type) {
        case fuxType::VOID: funcType = FunctionType::get(builder->getVoidTy(), argTypes, false); break;
        case fuxType::I32:  funcType = FunctionType::get(builder->getInt32Ty(), argTypes, false); break;
        case fuxType::F64:  funcType = FunctionType::get(builder->getDoubleTy(), argTypes, false); break;
        default:            return nullptr;
    }
    Function *func = Function::Create(funcType, Function::ExternalLinkage, name, *module);
    
    auto it = args.begin();
    for (auto &arg : func->args())
        arg.setName(it++->first);
    
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

    Value *retVal;
    for (ExprPtr &expr : body)
        retVal = expr->codegen(builder, module, namedValues);
    
    if (func->getReturnType()->isVoidTy()) {
        builder->CreateRetVoid();
        return func;
    }
    
    if (retVal) {
        builder->CreateRet(retVal);
        verifyFunction(*func);
        return func;
    }

    // error reading body
    func->eraseFromParent();
    return nullptr;
}