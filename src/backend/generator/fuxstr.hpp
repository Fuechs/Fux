/**
 * @file fuxstr.hpp
 * @author fuechs
 * @brief fux string type header
 * @version 0.1
 * @date 2022-12-28
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "wrapper.hpp"
#include "../llvmheader.hpp"
#include "fuxmem.hpp"

/**
 * custom string type for Fux
 * (predefined LLVM IR)
 */
class FuxStr {
public:
    FuxStr(LLVMWrapper *fuxLLVM, FuxMem *fuxMem);

    // string type
    StructType *str = nullptr;
    // pointer to string type
    PointerType *ptr = nullptr;
    
    Function *createDefaultStr = nullptr;
    Function *deleteStr = nullptr;

private:
    LLVMContext *context = nullptr;
    Module *module = nullptr;
    IRBuilder<> *builder = nullptr;
};