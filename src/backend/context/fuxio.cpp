/**
 * @file fuxio.cpp
 * @author fuechs
 * @brief fux io operations
 * @version 0.1
 * @date 2023-01-07
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#include "fuxio.hpp"

FuxIO::FuxIO(LLVMContext *context, Module *module, IRBuilder<> *builder, FuxMem *fuxMem, FuxStr *fuxStr) {
    { // declare i32 puts(i8* nocapture) nounwind
    FunctionType *FT = FunctionType::get(builder->getInt32Ty(), {builder->getInt8PtrTy()}, false);
    posixPuts = Function::Create(FT, Function::ExternalLinkage, "puts", *module);
    posixPuts->setDoesNotThrow();
    posixPuts->getArg(0)->addAttr(Attribute::NoCapture);
    llvm::verifyFunction(*posixPuts);
    }
    
    { // declare void putch(i8)
    FunctionType *FT = FunctionType::get(builder->getVoidTy(), {builder->getInt8Ty()}, false);
    putch = Function::Create(FT, Function::ExternalLinkage, "Fux_putch", *module);
    llvm::verifyFunction(*putch);
    } // end of putch

    { // define void Fux_puts(%str*)
    FunctionType *FT = FunctionType::get(builder->getVoidTy(), {fuxStr->ptr}, false);
    puts = Function::Create(FT, Function::ExternalLinkage, "Fux_puts", *module);
    BasicBlock *entry = BasicBlock::Create(*context, "entry", puts);

    builder->SetInsertPoint(entry);
    Value *buffer_ptr = builder->CreateGEP(fuxStr->str, puts->getArg(0), {builder->getInt64(0), builder->getInt32(0)}, "buffer_ptr");
    Value *buffer = builder->CreateLoad(builder->getInt8PtrTy(), buffer_ptr, "buffer");
    builder->CreateCall(posixPuts, {buffer});
    builder->CreateRetVoid();

    llvm::verifyFunction(*puts);
    } // end of puts

    { // declare void read(%str*);
    FunctionType *FT = FunctionType::get(builder->getVoidTy(), {fuxStr->ptr}, false);
    read = Function::Create(FT, Function::ExternalLinkage, "Fux_read", *module);
    llvm::verifyFunction(*read);
    } // end of read
}