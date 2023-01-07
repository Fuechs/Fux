/**
 * @file fuxio.cpp
 * @author fuechs
 * @brief fux io operations
 * @version 0.1
 * @date 2023-01-07
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs. All rights reserved.
 * 
 */

#include "fuxio.hpp"

FuxIO::FuxIO(LLVMContext *context, Module *module, IRBuilder<> *builder, FuxMem *fuxMem, FuxStr *fuxStr) {
    // temporary
    FunctionType *FT;

    // declare void putch(i8)
    FT = FunctionType::get(builder->getVoidTy(), {builder->getInt8Ty()}, false);
    putch = Function::Create(FT, Function::CommonLinkage, "putch", *module);
    llvm::verifyFunction(*putch);

    // declare void puts(%str)
    FT = FunctionType::get(builder->getVoidTy(), {fuxStr->str}, false);
    puts = Function::Create(FT, Function::CommonLinkage, "puts", *module);
    llvm::verifyFunction(*puts);

    // declare %str read(void);
    FT = FunctionType::get(fuxStr->str, false);
    read = Function::Create(FT, Function::CommonLinkage, "read", *module);
    llvm::verifyFunction(*read);
} 