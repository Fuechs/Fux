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

#ifdef FUX_BACKEND

Type *Generator::getType(LLVMWrapper *fuxLLVM, FuxType type) {
    IRBuilder<> *builder = fuxLLVM->builder;
    Type* ret;

    switch (type.kind) {
        case FuxType::VOID:     ret = builder->getVoidTy(); break;
        case FuxType::BOOL:     ret = builder->getInt1Ty(); break;
        case FuxType::I8:       ret = builder->getInt8Ty(); break;
        case FuxType::U8:       ret = builder->getInt8Ty(); break;
        case FuxType::C8:       ret = builder->getInt8Ty(); break;
        case FuxType::I16:      ret = builder->getInt16Ty(); break;
        case FuxType::U16:      ret = builder->getInt16Ty(); break;
        case FuxType::C16:      ret = builder->getInt16Ty(); break;
        case FuxType::I32:      ret = builder->getInt32Ty(); break;
        case FuxType::U32:      ret = builder->getInt32Ty(); break;
        case FuxType::F32:      ret = builder->getFloatTy(); break;
        case FuxType::I64:      ret = builder->getInt64Ty(); break;
        case FuxType::U64:      ret = builder->getInt64Ty(); break;
        case FuxType::F64:      ret = builder->getDoubleTy(); break;
        case FuxType::LIT:      ret = builder->getInt8PtrTy(); break;
        default:                ret = nullptr;
    }

    size_t pd = type.array ? type.pointerDepth + 1 : type.pointerDepth;
    while (pd --> 0) 
        ret = ret->getPointerTo();

    return ret;
}

#endif