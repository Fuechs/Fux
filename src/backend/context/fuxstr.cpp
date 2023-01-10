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
    // %str = type { i8*, i64, i64, i64 }
    str = StructType::create(*context, {
        builder->getInt8PtrTy(),    // char array (char *)
        builder->getInt64Ty(),      // length
        builder->getInt64Ty(),      // maxlen
        builder->getInt64Ty()       // factor
    }, "str", false);

    // %str* 
    ptr = str->getPointerTo();

    { // define fastcc void Fux_str_create_default(%str*) nounwind
    FunctionType *FT = FunctionType::get(builder->getVoidTy(), {ptr}, false);
    str_create_default = Function::Create(FT, Function::ExternalLinkage, "Fux_str_create_default", *module);
    str_create_default->setCallingConv(CallingConv::Fast); // fastcc
    str_create_default->setDoesNotThrow();
    BasicBlock *entry = BasicBlock::Create(*context, "entry", str_create_default);
    
    builder->SetInsertPoint(entry); 
    Value *gep0 = builder->CreateGEP(str, str_create_default->getArg(0), {builder->getInt64(0), builder->getInt32(0)});
    builder->CreateStore(ConstantPointerNull::get(builder->getInt8PtrTy()), gep0);
    Value *gep1 = builder->CreateGEP(str, str_create_default->getArg(0), {builder->getInt64(0), builder->getInt32(1)});
    builder->CreateStore(builder->getInt64(0), gep1);
    Value *gep2 = builder->CreateGEP(str, str_create_default->getArg(0), {builder->getInt64(0), builder->getInt32(2)});
    builder->CreateStore(builder->getInt64(0), gep2);
    Value *gep3 = builder->CreateGEP(str, str_create_default->getArg(0), {builder->getInt64(0), builder->getInt32(3)});
    builder->CreateStore(builder->getInt64(16), gep3);
    builder->CreateRetVoid();
    
    llvm::verifyFunction(*str_create_default);
    } // end of Fux_str_create_default

    { // define fastcc void Fux_str_delete(%str*) nounwind
    FunctionType *FT = FunctionType::get(builder->getVoidTy(), {ptr}, false);
    str_delete = Function::Create(FT, Function::ExternalLinkage, "Fux_str_delete", *module);
    str_delete->setCallingConv(CallingConv::Fast);
    str_delete->setDoesNotThrow();
    BasicBlock *entry = BasicBlock::Create(*context, "entry", str_delete);
    BasicBlock *freeBegin = BasicBlock::Create(*context, "free_begin", str_delete);
    BasicBlock *freeClose = BasicBlock::Create(*context, "free_close", str_delete);
    
    builder->SetInsertPoint(entry);
    Value *gep0 = builder->CreateGEP(str, str_delete->getArg(0), {builder->getInt64(0), builder->getInt32(0)});
    Value *load0 = builder->CreateLoad(builder->getInt8PtrTy(), gep0);
    Value *cmp0 = builder->CreateICmpNE(load0, ConstantPointerNull::get(builder->getInt8PtrTy()));
    builder->CreateCondBr(cmp0, freeBegin, freeClose);
    
    builder->SetInsertPoint(freeBegin);
    builder->CreateCall(fuxMem->free, {load0});
    builder->CreateBr(freeClose);
    
    builder->SetInsertPoint(freeClose);
    builder->CreateRetVoid();
    
    llvm::verifyFunction(*str_delete);
    } // end of Fux_str_delete

    { // define fastcc void Fux_str_resize(%str*, i64)
    FunctionType *FT = FunctionType::get(builder->getVoidTy(), {ptr, builder->getInt64Ty()}, false);
    str_resize = Function::Create(FT, Function::ExternalLinkage, "Fux_str_resize", *module);
    str_resize->setCallingConv(CallingConv::Fast);
    BasicBlock *entry = BasicBlock::Create(*context, "entry", str_resize);

    builder->SetInsertPoint(entry);
    Value *output = builder->CreateCall(fuxMem->malloc, {str_resize->getArg(1)}, "output");
    Value *gep0 = builder->CreateGEP(str, str_resize->getArg(0), {builder->getInt64(0), builder->getInt32(0)});
    Value *buffer = builder->CreateLoad(builder->getInt8PtrTy(), gep0, "buffer");
    Value *gep1 = builder->CreateGEP(str, str_resize->getArg(0), {builder->getInt64(0), builder->getInt32(1)});
    Value *len = builder->CreateLoad(builder->getInt64Ty(), gep1, "len");
    Value *call = builder->CreateCall(fuxMem->memcpy, {output, buffer, len});
    builder->CreateCall(fuxMem->free, {buffer});
    builder->CreateStore(output, gep0);
    Value *gep2 = builder->CreateGEP(str, str_resize->getArg(0), {builder->getInt64(0), builder->getInt32(2)});
    builder->CreateStore(str_resize->getArg(1), gep2);
    builder->CreateRetVoid();

    llvm::verifyFunction(*str_resize);
    } // end of Fux_str_resize

    { // define fastcc void Fux_str_add_char(%str*, i8)
    FunctionType *FT = FunctionType::get(builder->getVoidTy(), {ptr, builder->getInt8Ty()}, false);
    str_add_char = Function::Create(FT, Function::ExternalLinkage, "Fux_str_add_char", *module);
    str_add_char->setCallingConv(CallingConv::Fast);
    BasicBlock *entry = BasicBlock::Create(*context, "entry", str_add_char);
    BasicBlock *growBegin = BasicBlock::Create(*context, "grow_begin", str_add_char);
    BasicBlock *growClose = BasicBlock::Create(*context, "grow_close", str_add_char);
    
    builder->SetInsertPoint(entry);
    Value *len_gep = builder->CreateGEP(str, str_add_char->getArg(0), {builder->getInt64(0), builder->getInt32(1)});
    Value *len = builder->CreateLoad(builder->getInt64Ty(), len_gep, "len");
    Value *gep0 = builder->CreateGEP(str, str_add_char->getArg(0), {builder->getInt64(0), builder->getInt32(2)});
    Value *maxlen = builder->CreateLoad(builder->getInt64Ty(), gep0, "maxlen");
    Value *cmp0 = builder->CreateICmpEQ(len, maxlen);
    builder->CreateCondBr(cmp0, growBegin, growClose);
    
    builder->SetInsertPoint(growBegin);
    Value *gep1 = builder->CreateGEP(str, str_add_char->getArg(0), {builder->getInt64(0), builder->getInt32(3)});
    Value *factor = builder->CreateLoad(builder->getInt64Ty(), gep1, "factor");
    Value *add0 = builder->CreateAdd(maxlen, factor);
    builder->CreateCall(str_resize, {str_add_char->getArg(0), add0});
    builder->CreateBr(growClose);
    
    builder->SetInsertPoint(growClose);
    Value *gep2 = builder->CreateGEP(str, str_add_char->getArg(0), {builder->getInt64(0), builder->getInt32(0)});
    Value *buffer = builder->CreateLoad(builder->getInt8PtrTy(), gep2, "buffer");
    Value *gep3 = builder->CreateGEP(builder->getInt8PtrTy(), buffer, len);
    builder->CreateStore(str_add_char->getArg(1), gep3);
    Value *add1 = builder->CreateAdd(len, builder->getInt64(1));
    builder->CreateStore(add1, len_gep);
    builder->CreateRetVoid();
    
    llvm::verifyFunction(*str_add_char);
    } // end of Fux_str_add_char
}