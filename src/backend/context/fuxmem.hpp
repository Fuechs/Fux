/**
 * @file fuxmem.hpp
 * @author fuechs
 * @brief fux memory operations header
 * @version 0.1
 * @date 2023-01-06
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "../llvmheader.hpp"

class FuxMem {
public:
    FuxMem(LLVMContext *context, Module *module, IRBuilder<> *builder);

    Function *malloc = nullptr;
    Function *free = nullptr;
    Function *memcpy = nullptr;
};