/**
 * @file fuxio.hpp
 * @author fuechs
 * @brief fux io operations header
 * @version 0.1
 * @date 2023-01-07
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "../llvmheader.hpp"    
#include "fuxmem.hpp"
#include "fuxstr.hpp"

class FuxIO {
public:
    FuxIO(LLVMContext *context, Module *module, IRBuilder<> *builder, FuxMem *fuxMem, FuxStr *fuxStr);

    Function *putch = nullptr;
    Function *puts = nullptr;
     
    Function *read = nullptr;
};