/**
 * @file fuxstr.hpp
 * @author fuechs
 * @brief fux string type header
 * @version 0.1
 * @date 2022-12-28
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../llvmheader.hpp"
#include "fuxmem.hpp"

/**
 * custom string type for Fux
 * (predefined LLVM IR)
 */
class FuxStr {
public:
    FuxStr(LLVMContext *context, Module *module, IRBuilder<> *builder, FuxMem *fuxMem);

    // string type
    StructType *str = nullptr;
    // pointer to string type
    PointerType *ptr = nullptr;
    
    Function *str_create_default = nullptr;
    Function *str_delete = nullptr;
    Function *str_resize = nullptr;
    Function *str_add_char = nullptr;
    Function *str_get_buffer = nullptr;
};