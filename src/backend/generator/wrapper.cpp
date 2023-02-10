/**
 * @file wrapper.cpp
 * @author fuechs
 * @brief fux llvm wrapper
 * @version 0.1
 * @date 2023-02-10
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "wrapper.hpp"

FuxValue &FuxValue::operator=(const FuxValue &copy) { 
    type = copy.type; 
    value = copy.value; 
    return *this; 
}

Type *LLVMWrapper::getTypeOf(Value *ptr) {
    for (FuxValue::Map::value_type &x : values)
        if (x.second.value == ptr)
            return x.second.type;
    return nullptr;
}

void LLVMWrapper::loadValue(Value *&ptr) { 
    if (ptr->getType()->isPointerTy())
        ptr = builder->CreateLoad(getTypeOf(ptr), ptr); 
}