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
    { // declare void putch(i8)
    FunctionType *FT = FunctionType::get(builder->getVoidTy(), {builder->getInt8Ty()}, false);
    putch = Function::Create(FT, Function::CommonLinkage, "Fux_putch", *module);
    llvm::verifyFunction(*putch);
    } // end of putch

    { // declare void puts(%str)
    FunctionType *FT = FunctionType::get(builder->getVoidTy(), {fuxStr->str}, false);
    puts = Function::Create(FT, Function::CommonLinkage, "Fux_puts", *module);
    llvm::verifyFunction(*puts);
    } // end of puts

    { // declare void read(%str*);
    FunctionType *FT = FunctionType::get(builder->getVoidTy(), {fuxStr->ptr}, false);
    read = Function::Create(FT, Function::CommonLinkage, "Fux_read", *module);
    llvm::verifyFunction(*read);
    } // end of read
}