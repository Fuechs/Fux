/**
 * @file fuxmem.cpp
 * @author fuechs
 * @brief fux memory operations 
 * @version 0.1
 * @date 2023-01-06
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs. All rights reserved.
 * 
 */

#include "fuxmem.hpp"

FuxMem::FuxMem(LLVMWrapper *fuxLLVM) {
    context = fuxLLVM->context;
    module = fuxLLVM->module;
    builder = fuxLLVM->builder;

    // temporary
    FunctionType *FT = nullptr;

    // declare i8* @malloc(i64)
    FT = FunctionType::get(builder->getInt8PtrTy(), {builder->getInt64Ty()}, false);
    malloc = Function::Create(FT, Function::CommonLinkage, "malloc", *module);
    llvm::verifyFunction(*malloc);

    // declare void @free(i8*)
    FT = FunctionType::get(builder->getVoidTy(), {builder->getInt8PtrTy()}, false);
    free = Function::Create(FT, Function::CommonLinkage, "free", *module);
    llvm::verifyFunction(*free);

    // declare i8* memcpy(i8*, i8*, i64)
    FT = FunctionType::get(builder->getInt8PtrTy(), {
        builder->getInt8PtrTy(),
        builder->getInt8PtrTy(),
        builder->getInt64Ty()
    }, false);
    memcpy = Function::Create(FT, Function::CommonLinkage, "memcpy", *module);
    llvm::verifyFunction(*memcpy);
}