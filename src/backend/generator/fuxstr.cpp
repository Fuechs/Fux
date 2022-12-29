/**
 * @file fuxstr.cpp
 * @author fuechs
 * @brief fux string type
 * @version 0.1
 * @date 2022-12-29
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "fuxstr.hpp"

FuxStr::FuxStr(LLVMWrapper *fuxLLVM) {
    context = fuxLLVM->context;
    module = fuxLLVM->module;
    builder = fuxLLVM->builder;

    // temporary variables
    FunctionType *FT = nullptr; 
    BasicBlock *BB = nullptr;

    // %str = type { i8*, i64, i64, i64 }
    str = StructType::create(*context, {
        fuxLLVM->builder->getInt8PtrTy(),
        fuxLLVM->builder->getInt64Ty(),
        fuxLLVM->builder->getInt64Ty(),
        fuxLLVM->builder->getInt64Ty(),
    }, "str", false);

    // %str* | ptr
    ptr = PointerType::get(str, 0);

    // define common fastcc void createDefaultStr(%str* %0)
    FT = FunctionType::get(builder->getVoidTy(), {ptr}, false);
    createDefaultStr = Function::Create(FT, Function::CommonLinkage, "createDefaultStr", *module);
    createDefaultStr->setCallingConv(CallingConv::Fast); // fastcc
    BB = BasicBlock::Create(*context, "entry", createDefaultStr);
    builder->SetInsertPoint(BB);
    // TODO: function body
    builder->CreateRetVoid();
    verifyFunction(*createDefaultStr); 
    // end of createDefaultStr

    // TODO: following functions
}