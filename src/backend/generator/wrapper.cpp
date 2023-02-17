/**
 * @file wrapper.cpp
 * @author fuechs
 * @brief fux llvm wrapper
 * @version 0.1
 * @date 2023-02-10
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "wrapper.hpp"

FuxValue &FuxValue::operator=(const FuxValue &copy) { 
    type = copy.type; 
    value = copy.value; 
    literal = copy.literal;
    return *this; 
}

FuxValue FuxValue::Literal(Value *value) {
    FuxValue fv = FuxValue(Type::getInt8PtrTy(value->getContext()), value);
    fv.literal = true;
    return fv;
}


LLVMWrapper::LLVMWrapper(LLVMContext *context, Module *module, IRBuilder<> *builder)
: context(context), module(module), builder(builder), values(FuxValue::Map()) {
    FunctionType *FT = FunctionType::get(builder->getInt64Ty(), {builder->getInt8PtrTy()}, false);
    posix_puts = Function::Create(FT, Function::ExternalLinkage, "puts", *module);
    posix_puts->getArg(0)->addAttr(Attribute::NoCapture);
    llvm::verifyFunction(*posix_puts);
}

LLVMWrapper::~LLVMWrapper() {
    delete context;
    delete module;
    delete builder;
    values.clear();
}

Type *LLVMWrapper::getTypeOf(Value *ptr) {
    for (FuxValue::Map::value_type &x : values)
        if (x.second.value == ptr)
            return x.second.type;
    return nullptr;
}

bool LLVMWrapper::isLiteral(Value *ptr) {
    for (FuxValue::Map::value_type &x : values)
        if (x.second.value == ptr)
            return x.second.literal;
    return false;
}

Value *LLVMWrapper::loadValue(Value *ptr) { 
    if (ptr->getType()->isPointerTy() && !isLiteral(ptr))
        return builder->CreateLoad(getTypeOf(ptr), ptr, ptr->getName()+"_LOAD_"); 
    return ptr;
}