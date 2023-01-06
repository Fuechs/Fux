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

#include "wrapper.hpp"
#include "../llvmheader.hpp"

class FuxMem {
public:
    FuxMem(LLVMWrapper *fuxLLVM);

    Function *malloc = nullptr;
    Function *free = nullptr;
    Function *memcpy = nullptr;

private:
    LLVMContext *context = nullptr;
    Module *module = nullptr;
    IRBuilder<> *builder = nullptr;
};