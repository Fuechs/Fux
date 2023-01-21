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
#include "../context/fuxmem.hpp"
#include "../context/fuxstr.hpp"
#include "../context/fuxio.hpp"
#endif

struct LLVMWrapper {
    LLVMWrapper(LLVMContext *context, Module *module, IRBuilder<> *builder)
    : context(context), module(module), builder(builder), namedValues({}) {
        #ifdef FUX_BACKEND
        fuxMem = new FuxMem(context, module, builder);
        fuxStr = new FuxStr(context, module, builder, fuxMem);
        fuxIO = new FuxIO(context, module, builder, fuxMem, fuxStr);
        #endif
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

    #ifdef FUX_BACKEND
    FuxMem *fuxMem;
    FuxStr *fuxStr;
    FuxIO *fuxIO;
    #endif

    ValueMap namedValues;
};