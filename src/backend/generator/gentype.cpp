/**
 * @file gentype.cpp
 * @author fuechs
 * @brief generator types
 * @version 0.1
 * @date 2022-12-26
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "generator.hpp"

Type *Generator::getType(LLVMWrapper *fuxLLVM, FuxType type) {
    IRBuilder<> *builder = fuxLLVM->builder;
    Type* ret;

    switch (type.kind) {
        case FuxType::VOID:     ret = builder->getVoidTy();
        case FuxType::BOOL:     ret = builder->getInt1Ty();
        case FuxType::I8:       ret = builder->getInt8Ty();
        case FuxType::U8:       ret = builder->getInt8Ty();
        case FuxType::C8:       ret = builder->getInt8Ty();
        case FuxType::I16:      ret = builder->getInt16Ty();
        case FuxType::U16:      ret = builder->getInt16Ty();
        case FuxType::C16:      ret = builder->getInt16Ty();
        case FuxType::I32:      ret = builder->getInt32Ty();
        case FuxType::U32:      ret = builder->getInt32Ty();
        case FuxType::F32:      ret = builder->getFloatTy();
        case FuxType::I64:      ret = builder->getInt64Ty();
        case FuxType::U64:      ret = builder->getInt64Ty();
        case FuxType::F64:      ret = builder->getDoubleTy();
        case FuxType::STR:      ret = fuxLLVM->fuxStr->str;
        default:                ret = nullptr;
    }

    size_t pd = type.pointerDepth;
    while (pd --> 0) 
        ret = ret->getPointerTo();

    return ret;
}

FuxArray *Generator::createArrayType(LLVMWrapper *fuxLLVM, FuxType type) { 
    return new FuxArray(fuxLLVM, getType(fuxLLVM, type)); 
}