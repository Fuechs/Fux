/**
 * @file wrapper.hpp
 * @author fuechs
 * @brief fux llvm wrapper header
 * @version 0.1
 * @date 2022-12-28
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../llvmheader.hpp"

#ifdef FUX_BACKEND

struct FuxValue {
    typedef map<string, FuxValue> Map;

    FuxValue(Type *type = nullptr, Value *value = nullptr) : type(type), value(value) {}
    FuxValue &operator=(const FuxValue &copy);

    Type *type;     // type that is pointed to
    Value *value;   // pointer (local variable)
};

struct LLVMWrapper {
    LLVMWrapper(LLVMContext *context, Module *module, IRBuilder<> *builder)
    : context(context), module(module), builder(builder), values(FuxValue::Map()) {}

    ~LLVMWrapper() {
        delete context;
        delete module;
        delete builder;
        values.clear();
    }

    Type *getTypeOf(Value *ptr);
    void loadValue(Value *&ptr);

    LLVMContext *context;
    Module *module;
    IRBuilder<> *builder;

    // FuxMem *fuxMem;
    // FuxStr *fuxStr;
    // FuxIO *fuxIO;

    FuxValue::Map values;
    Type *ctxReturnType = nullptr;
};

#endif