/**
 * @file wrapper.hpp
 * @author fuechs
 * @brief fux llvm wrapper
 * @version 0.1
 * @date 2022-12-28
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "../llvmheader.hpp"

typedef map<string, Value *> ValueMap;

struct LLVMWrapper {
    LLVMWrapper(LLVMContext *context, Module *module, IRBuilder<> *builder)
    : context(context), module(module), builder(builder), namedValues({}) {}

    ~LLVMWrapper() {
        delete context;
        delete module;
        delete builder;
        namedValues.clear();
    }

    LLVMContext *context;
    Module *module;
    IRBuilder<> *builder;

    ValueMap namedValues;
};