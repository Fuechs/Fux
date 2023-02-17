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

    FuxValue(Type *type = nullptr, Value *value = nullptr) 
    : type(type), value(value), literal(false) {}
    
    FuxValue &operator=(const FuxValue &copy);

    static FuxValue Literal(Value *value);

    Type *type;     // type that is pointed to
    Value *value;   // pointer (local variable)
    bool literal;   // is global constant literal
};

struct LLVMWrapper {
    LLVMWrapper(LLVMContext *context, Module *module, IRBuilder<> *builder);
    ~LLVMWrapper();

    Type *getTypeOf(Value *ptr);
    bool isLiteral(Value *ptr);
    Value *loadValue(Value *ptr);

    LLVMContext *context;
    Module *module;
    IRBuilder<> *builder;

    FuxValue::Map values;
    Function *posix_puts = nullptr; // temporary, will be replaced
};

#endif