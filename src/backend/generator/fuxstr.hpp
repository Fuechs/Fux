/**
 * @file fuxstr.hpp
 * @author fuechs
 * @brief fux string type header
 * @version 0.1
 * @date 2022-12-28
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "wrapper.hpp"
#include "../llvmheader.hpp"

/**
 * custom string type for Fux
 * (predefined LLVM IR)
 */
struct FuxStr {
public:
    FuxStr(LLVMWrapper *fuxLLVM);

    // string type
    StructType *str;
    // pointer to string type
    PointerType *ptr;
    
    Function *createDefaultStr;
    Function *deleteStr;

private:
    LLVMContext *context;
    Module *module;
    IRBuilder<> *builder;
};