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

Eisdrache::Ty *Generator::getType(Eisdrache *eisdrache, FuxType type) {
    Eisdrache::Ty *ret = nullptr;

    switch (type.kind) {
        case FuxType::VOID:     ret = eisdrache->getVoidTy(); break;
        case FuxType::BOOL:     ret = eisdrache->getBoolTy(); break;
        case FuxType::I8:       ret = eisdrache->getSignedTy(8); break;
        case FuxType::U8:       ret = eisdrache->getUnsignedTy(8); break;
        case FuxType::C8:       ret = eisdrache->getUnsignedTy(8); break;
        case FuxType::I16:      ret = eisdrache->getSignedTy(16); break;
        case FuxType::U16:      ret = eisdrache->getUnsignedTy(16); break;
        case FuxType::C16:      ret = eisdrache->getUnsignedTy(16); break;
        case FuxType::F16:      ret = eisdrache->getFloatTy(16); break;
        case FuxType::I32:      ret = eisdrache->getSignedTy(32); break;
        case FuxType::U32:      ret = eisdrache->getUnsignedTy(32); break;
        case FuxType::F32:      ret = eisdrache->getFloatTy(32); break;
        case FuxType::I64:      ret = eisdrache->getSignedTy(64); break;
        case FuxType::U64:      ret = eisdrache->getUnsignedTy(64); break;
        case FuxType::F64:      ret = eisdrache->getFloatTy(64); break;
        case FuxType::LIT:      ret = eisdrache->getUnsignedPtrTy(8); break;
        default:                ret = nullptr;
    }

    size_t pd = type.array ? type.pointerDepth + 1 : type.pointerDepth;
    while (pd --> 0) 
        ret = ret->getPtrTo();

    return ret;
}

#endif