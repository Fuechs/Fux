/**
 * @file genarray.hpp
 * @author fuechs
 * @brief fux array type
 * @version 0.1
 * @date 2023-01-09
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#pragma once

#include "wrapper.hpp"
#include "../llvmheader.hpp"

class FuxArray {
public:
    FuxArray(LLVMWrapper *fuxLLVM, Type *type, const bool userDef = false) : elementType(type) {
        LLVMContext *context = fuxLLVM->context;
        Module *module = fuxLLVM->module;
        IRBuilder<> *builder =fuxLLVM->builder;

        // create array type
        arrayType = StructType::create({
            elementType->getPointerTo(),    // buffer
            builder->getInt64Ty(),          // length
            builder->getInt64Ty(),          // maxlength
            builder->getInt64Ty(),          // factor
        });
        arrayPtr = arrayType->getPointerTo();
        
        llvm::raw_string_ostream rso(prefix);
        type->print(rso);
        if (userDef)
            prefix = "Usr_arr_"+rso.str()+"_";
        else
            prefix = "Fux_arr_"+rso.str()+"_";
    }

    Function *arr_create_default;
    Function *arr_delete;

    Type *arrayType;
    Type *arrayPtr;
    Type *elementType;

private:
    string prefix;
};
