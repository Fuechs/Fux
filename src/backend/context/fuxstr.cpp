/**
 * @file fuxstr.cpp
 * @author fuechs
 * @brief fux string type
 * @version 0.1
 * @date 2022-12-29
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs. All rights reserved.
 * 
 */

#include "fuxstr.hpp"

FuxStr::FuxStr(LLVMContext *context, Module *module, IRBuilder<> *builder, FuxMem *fuxMem) {

    // temporary variables
    FunctionType *FT = nullptr; 
    BasicBlock *BB = nullptr;
    Value *GEP = nullptr;
    vector<Value *> args = {};
    GlobalVariable *GV = nullptr;

    // %str = type { i8*, i64, i64, i64 }
    str = StructType::create(*context, {
        builder->getInt8PtrTy(),    // char array (char *)
        builder->getInt64Ty(),      // length
        builder->getInt64Ty(),      // maxlen
        builder->getInt64Ty()       // factor
    }, "str", false);

    // %str* | ptr
    ptr = PointerType::get(str, 0);

    // example variable
    module->getOrInsertGlobal("example", str);
    GV = module->getNamedGlobal("example");
    GV->setLinkage(GlobalValue::CommonLinkage);

    // define common fastcc void createDefaultStr(%str* %0)
    FT = FunctionType::get(builder->getVoidTy(), {ptr}, false);
    createDefaultStr = Function::Create(FT, Function::CommonLinkage, "createDefaultStr", *module);
    createDefaultStr->setCallingConv(CallingConv::Fast); // fastcc
    args.push_back(createDefaultStr->args().begin());
    BB = BasicBlock::Create(*context, "entry", createDefaultStr);
    builder->SetInsertPoint(BB);
    
    GEP = builder->CreateGEP(ptr, args[0], {builder->getInt64(0)});
    builder->CreateStore(ConstantPointerNull::get(builder->getInt8PtrTy()), GEP);
    
    builder->CreateRetVoid();
    
    llvm::verifyFunction(*createDefaultStr);
    // end of createDefaultStr

    // TODO: following functions
}