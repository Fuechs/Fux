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

    // %str* | ptr
    ptr = PointerType::get(str, 0);

    { // define common fastcc void createDefaultStr(%str*) nounwind
    FunctionType *FT = FunctionType::get(builder->getVoidTy(), {ptr}, false);
    createDefaultStr = Function::Create(FT, Function::CommonLinkage, "createDefaultStr", *module);
    createDefaultStr->setCallingConv(CallingConv::Fast); // fastcc
    createDefaultStr->setDoesNotThrow();
    ArrayRef<Value *> args = {createDefaultStr->args().begin()};
    BasicBlock *entry = BasicBlock::Create(*context, "entry", createDefaultStr);
    
    builder->SetInsertPoint(entry); 
    Value *gep0 = builder->CreateGEP(ptr, args[0], {builder->getInt64(0)});
    builder->CreateStore(ConstantPointerNull::get(builder->getInt8PtrTy()), gep0);
    Value *gep1 = builder->CreateGEP(ptr, args[0], {builder->getInt64(1)});
    builder->CreateStore(builder->getInt64(0), gep1);
    Value *gep2 = builder->CreateGEP(ptr, args[0], {builder->getInt64(2)});
    builder->CreateStore(builder->getInt64(0), gep2);
    Value *gep3 = builder->CreateGEP(ptr, args[0], {builder->getInt64(3)});
    builder->CreateStore(builder->getInt64(16), gep3);
    builder->CreateRetVoid();
    
    llvm::verifyFunction(*createDefaultStr);
    } // end of createDefaultStr

    { // define common fastcc void deleteStr(%str*) nounwind
    FunctionType *FT = FunctionType::get(builder->getVoidTy(), {ptr}, false);
    deleteStr = Function::Create(FT, Function::CommonLinkage, "deleteStr", *module);
    deleteStr->setCallingConv(CallingConv::Fast);
    deleteStr->setDoesNotThrow();
    ArrayRef<Value *> args = {deleteStr->args().begin()};
    BasicBlock *entry = BasicBlock::Create(*context, "entry", deleteStr);
    BasicBlock *freeBegin = BasicBlock::Create(*context, "free_begin", deleteStr);
    BasicBlock *freeClose = BasicBlock::Create(*context, "free_close", deleteStr);
    
    builder->SetInsertPoint(entry);
    Value *gep0 = builder->CreateGEP(ptr, args[0], {builder->getInt64(0)});
    Value *load0 = builder->CreateLoad(builder->getInt8PtrTy(), gep0);
    Value *cmp0 = builder->CreateICmpNE(load0, ConstantPointerNull::get(builder->getInt8PtrTy()));
    builder->CreateCondBr(cmp0, freeBegin, freeClose);
    
    builder->SetInsertPoint(freeBegin);
    builder->CreateCall(fuxMem->free, {load0});
    builder->CreateBr(freeClose);
    
    builder->SetInsertPoint(freeClose);
    builder->CreateRetVoid();
    
    llvm::verifyFunction(*deleteStr);
    } // end of deleteStr

    { // declare common fastcc void resizeStr(%str*, i64)
    FunctionType *FT = FunctionType::get(builder->getVoidTy(), {ptr, builder->getInt64Ty()}, false);
    resizeStr = Function::Create(FT, Function::CommonLinkage, "resizeStr", *module);
    resizeStr->setCallingConv(CallingConv::Fast);
    ArrayRef<Value *> args = {resizeStr->getArg(0), resizeStr->getArg(1)};
    BasicBlock *entry = BasicBlock::Create(*context, "entry", resizeStr);

    builder->SetInsertPoint(entry);
    Value *output = builder->CreateCall(fuxMem->malloc, {args[1]}, "output");
    Value *gep0 = builder->CreateGEP(ptr, args[0], {builder->getInt64(0)});
    Value *buffer = builder->CreateLoad(builder->getInt8PtrTy(), gep0, "buffer");
    Value *gep1 = builder->CreateGEP(ptr, args[0], {builder->getInt64(1)});
    Value *len = builder->CreateLoad(builder->getInt64Ty(), gep1, "len");
    Value *call = builder->CreateCall(fuxMem->memcpy, {output, buffer, len});
    builder->CreateCall(fuxMem->free, {buffer});
    builder->CreateStore(output, gep0);
    Value *gep2 = builder->CreateGEP(ptr, args[0], {builder->getInt64(2)});
    builder->CreateStore(args[1], gep2);
    builder->CreateRetVoid();

    llvm::verifyFunction(*resizeStr);
    } // end of resizeStr

    { // define common fastcc void addChar(%str*, i8)
    FunctionType *FT = FunctionType::get(builder->getVoidTy(), {ptr, builder->getInt8Ty()}, false);
    addChar = Function::Create(FT, Function::CommonLinkage, "addChar", *module);
    addChar->setCallingConv(CallingConv::Fast);
    ArrayRef<Value *> args = {addChar->getArg(0), addChar->getArg(1)};
    BasicBlock *entry = BasicBlock::Create(*context, "entry", addChar);
    BasicBlock *growBegin = BasicBlock::Create(*context, "grow_begin", addChar);
    BasicBlock *growClose = BasicBlock::Create(*context, "grow_close", addChar);
    
    builder->SetInsertPoint(entry);
    Value *len_gep = builder->CreateGEP(ptr, args[0], {builder->getInt64(1)});
    Value *len = builder->CreateLoad(builder->getInt64Ty(), len_gep, "len");
    Value *gep0 = builder->CreateGEP(ptr, args[0], {builder->getInt64(2)});
    Value *maxlen = builder->CreateLoad(builder->getInt64Ty(), gep0, "maxlen");
    Value *cmp0 = builder->CreateICmpEQ(len, maxlen);
    builder->CreateCondBr(cmp0, growBegin, growClose);
    
    builder->SetInsertPoint(growBegin);
    Value *gep1 = builder->CreateGEP(ptr, args[0], {builder->getInt64(3)});
    Value *factor = builder->CreateLoad(builder->getInt64Ty(), gep1, "factor");
    Value *add0 = builder->CreateAdd(maxlen, factor);
    builder->CreateCall(resizeStr, {args[0], add0});
    builder->CreateBr(growClose);
    
    builder->SetInsertPoint(growClose);
    Value *gep2 = builder->CreateGEP(ptr, args[0], {builder->getInt64(0)});
    Value *buffer = builder->CreateLoad(builder->getInt8PtrTy(), gep2, "buffer");
    Value *gep3 = builder->CreateGEP(builder->getInt8PtrTy(), buffer, len);
    builder->CreateStore(args[1], gep3);
    Value *add1 = builder->CreateAdd(len, builder->getInt64(1));
    builder->CreateStore(add1, len_gep);
    builder->CreateRetVoid();
    
    llvm::verifyFunction(*addChar);
    } // end of addChar
}