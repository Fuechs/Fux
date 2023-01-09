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
#include "../context/fuxmem.hpp"
#include "../context/fuxstr.hpp"
#include "../context/fuxio.hpp"

typedef map<string, Value *> ValueMap;

struct LLVMWrapper {
    LLVMWrapper(LLVMContext *context, Module *module, IRBuilder<> *builder)
    : context(context), module(module), builder(builder), namedValues({}) {
        fuxMem = new FuxMem(context, module, builder);
        fuxStr = new FuxStr(context, module, builder, fuxMem);
        fuxIO = new FuxIO(context, module, builder, fuxMem, fuxStr);
    }

    ~LLVMWrapper() {
        delete context;
        delete module;
        delete builder;
        namedValues.clear();
    }

    LLVMContext *context;
    Module *module;
    IRBuilder<> *builder;

    FuxMem *fuxMem;
    FuxStr *fuxStr;
    FuxIO *fuxIO;

    ValueMap namedValues;
};