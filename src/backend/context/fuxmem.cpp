/**
 * @file fuxmem.cpp
 * @author fuechs
 * @brief fux memory operations 
 * @version 0.1
 * @date 2023-01-06
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "fuxmem.hpp"

#ifdef FUX_BACKEND

FuxMem::FuxMem(LLVMContext *context, Module *module, IRBuilder<> *builder) {
    // temporary   
    FunctionType *FT = nullptr;

    // declare i8* malloc(i64)
    FT = FunctionType::get(builder->getInt8PtrTy(), {builder->getInt64Ty()}, false);
    malloc = Function::Create(FT, Function::ExternalLinkage, "malloc", *module);
    llvm::verifyFunction(*malloc);

    // declare void free(i8*)
    FT = FunctionType::get(builder->getVoidTy(), {builder->getInt8PtrTy()}, false);
    free = Function::Create(FT, Function::ExternalLinkage, "free", *module);
    llvm::verifyFunction(*free);

    // declare i8* memcpy(i8*, i8*, i64)
    FT = FunctionType::get(builder->getInt8PtrTy(), {
        builder->getInt8PtrTy(),
        builder->getInt8PtrTy(),
        builder->getInt64Ty()
    }, false);
    memcpy = Function::Create(FT, Function::ExternalLinkage, "memcpy", *module);
    llvm::verifyFunction(*memcpy);
}

#endif