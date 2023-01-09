/**
 * @file genarray.hpp
 * @author fuechs
 * @brief fux array type
 * @version 0.1
 * @date 2023-01-09
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "wrapper.hpp"
#include "../llvmheader.hpp"

class FuxArray {
public:
    FuxArray(LLVMWrapper *fuxLLVM, Type *type) : elementType(type) {
        LLVMContext *context = fuxLLVM->context;
        Module *module = fuxLLVM->module;
        IRBuilder<> *builder =fuxLLVM->builder;

        // create array type
        arrayType = StructType::create({
            elementType->getPointerTo(),
            builder->getInt64Ty(),
            builder->getInt64Ty(),
            builder->getInt64Ty(),
        });
        arrayPtr = arrayType->getPointerTo();
    }

    Function *arr_create_default;
    Function *arr_delete;

    Type *arrayType;
    Type *arrayPtr;
    Type *elementType;
};
