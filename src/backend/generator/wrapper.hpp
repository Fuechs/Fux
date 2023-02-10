/**
 * @file wrapper.hpp
 * @author fuechs
 * @brief fux llvm wrapper
 * @version 0.1
 * @date 2022-12-28
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../llvmheader.hpp"

#ifdef FUX_BACKEND
// #include "../context/fuxmem.hpp"
// #include "../context/fuxstr.hpp"
// #include "../context/fuxio.hpp"

struct FuxValue {
    typedef map<string, FuxValue> Map;

    FuxValue(Type *type = nullptr, Value *value = nullptr) : type(type), value(value) {}
    
    FuxValue &operator=(const FuxValue &copy) { 
        type = copy.type; 
        value = copy.value; 
        return *this; 
    }

    Type *type;     // type that is pointed to
    Value *value;   // pointer (local variable)
};

struct LLVMWrapper {
    LLVMWrapper(LLVMContext *context, Module *module, IRBuilder<> *builder)
    : context(context), module(module), builder(builder), values(FuxValue::Map()) {
        // fuxMem = new FuxMem(context, module, builder);
        // fuxStr = new FuxStr(context, module, builder, fuxMem);
        // fuxIO = new FuxIO(context, module, builder, fuxMem, fuxStr);
    }

    ~LLVMWrapper() {
        delete context;
        delete module;
        delete builder;
        values.clear();
    }

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