/**
 * @file fuxmem.hpp
 * @author fuechs
 * @brief fux memory operations header
 * @version 0.1
 * @date 2023-01-06
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../llvmheader.hpp"

#ifdef FUX_BACKEND

class FuxMem {
public:
    FuxMem(LLVMContext *context, Module *module, IRBuilder<> *builder);

    Function *malloc = nullptr;
    Function *free = nullptr;
    Function *memcpy = nullptr;
};

#endif