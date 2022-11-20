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
    GlobalVariable *gVar = createGlob(*builder, "x");
    
    Function *fooFunc = createFunc(*builder, "foo");
    
    funcArgs.push_back("a"); 
    funcArgs.push_back("b");
    setFuncArgs(fooFunc, funcArgs);
    
    BasicBlock *entry = createBB(fooFunc, "entry");
    builder->SetInsertPoint(entry);

    Function::arg_iterator args = fooFunc->arg_begin();
    Value *arg1 = &*args++;
    Value *constant = builder->getInt32(16);
    Value *val = createArith(*builder, arg1, constant);
    
    Value *val2 = builder->getInt32(100);
    Value *compare = builder->CreateICmpULT(val, val2, "cmptmp");

    ValList VL;
    VL.push_back(compare);
    VL.push_back(arg1);

    BasicBlock *thenBB = createBB(fooFunc, "then");
    BasicBlock *elseBB = createBB(fooFunc, "else");
    BasicBlock *mergeBB = createBB(fooFunc, "ifcont");
    BBList list;
    list.push_back(thenBB);
    list.push_back(elseBB);
    list.push_back(mergeBB);

    Value *v = createIfElse(*builder, list, VL);
    
    builder->CreateRet(v);
    verifyFunction(*fooFunc);
    module->print(errs(), nullptr);
}

void Generator::initializeModule() {
    this->context = new LLVMContext();
    this->module = new Module("fux compiler", *context);
    this->builder = new IRBuilder<>(*context);
}

Function *Generator::createFunc(IRBuilder<> &builder, string name) {
    vector<llvm::Type *> integers(funcArgs.size(), llvm::Type::getInt32Ty(*context));
    FunctionType *funcType = FunctionType::get(builder.getInt32Ty(), integers, false);
    Function *thisFunc = Function::Create(funcType, llvm::Function::ExternalLinkage, name, module);
    return thisFunc;
}

BasicBlock *Generator::createBB(Function *func, string name) {
    return BasicBlock::Create(*context, name, func);
}

void Generator::setFuncArgs(Function *func, vector<string> funcArgs) {
    size_t idx = 0;
    Function::arg_iterator AI, AE;
    for (AI = func->arg_begin(), AE = func->arg_end(); AI != AE; ++AI, ++idx) 
        AI->setName(funcArgs[idx]);
}

GlobalVariable *Generator::createGlob(IRBuilder<> &builder, string name) {
    module->getOrInsertGlobal(name, builder.getInt32Ty());
    GlobalVariable *gVar = module->getNamedGlobal(name);
    gVar->setLinkage(GlobalValue::CommonLinkage);
    gVar->setAlignment(MaybeAlign(4));
    return gVar;
}

Value *Generator::createArith(IRBuilder<> &builder, Value *L, Value *R) {
    return builder.CreateMul(L, R, "multmp");
}


Value *Generator::createIfElse(IRBuilder<> &builder, BBList list, ValList VL) {
    Value *condtn = VL[0];
    Value *arg1 = VL[1];
    BasicBlock *thenBB = list[0];
    BasicBlock *elseBB = list[1];
    BasicBlock *mergeBB = list[2];
    builder.CreateCondBr(condtn, thenBB, elseBB);

    builder.SetInsertPoint(thenBB);
    Value *thenVal = builder.CreateAdd(arg1, builder.getInt32(1), "thenaddtmp");
    builder.CreateBr(mergeBB);

    builder.SetInsertPoint(elseBB);
    Value *elseVal = builder.CreateAdd(arg1, builder.getInt32(2), "elseaddtmp");
    builder.CreateBr(mergeBB);

    size_t phiBBSize = list.size() - 1;
    builder.SetInsertPoint(mergeBB);
    PHINode *phi = builder.CreatePHI(llvm::Type::getInt32Ty(*(new LLVMContext())), phiBBSize, "iftmp");
    phi->addIncoming(thenVal, thenBB);
    phi->addIncoming(elseVal, elseBB);

    return phi;
}