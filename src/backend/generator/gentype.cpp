/**
 * @file gentype.cpp
 * @author fuechs
 * @brief generator types
 * @version 0.1
 * @date 2022-12-26
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "generator.hpp"

Type *Generator::getType(fuxType::Type type, const bool pointer) {
    // TODO: how the fuck do I get unsigned / signed integer types?

    if (pointer)
        switch (type) {
            case fuxType::VOID:     return fuxLLVM->builder->getPtrTy();
            case fuxType::BOOL:     return Type::getInt1PtrTy(*fuxLLVM->context);
            case fuxType::I8:       return fuxLLVM->builder->getInt8PtrTy();
            case fuxType::U8:       return fuxLLVM->builder->getInt8PtrTy();
            case fuxType::C8:       return fuxLLVM->builder->getInt8PtrTy();
            case fuxType::I16:      return Type::getInt16PtrTy(*fuxLLVM->context);
            case fuxType::U16:      return Type::getInt16PtrTy(*fuxLLVM->context);
            case fuxType::C16:      return Type::getInt16PtrTy(*fuxLLVM->context);
            case fuxType::I32:      return Type::getInt32PtrTy(*fuxLLVM->context);
            case fuxType::U32:      return Type::getInt32PtrTy(*fuxLLVM->context);
            case fuxType::F32:      return Type::getFloatPtrTy(*fuxLLVM->context);
            case fuxType::I64:      return Type::getInt64PtrTy(*fuxLLVM->context);
            case fuxType::U64:      return Type::getInt64PtrTy(*fuxLLVM->context);
            case fuxType::F64:      return Type::getDoublePtrTy(*fuxLLVM->context);
            case fuxType::STR:      return fuxLLVM->builder->getPtrTy();
            default:                return nullptr;
        }

    switch (type) {
        case fuxType::VOID:     return fuxLLVM->builder->getVoidTy();
        case fuxType::BOOL:     return fuxLLVM->builder->getInt1Ty();
        case fuxType::I8:       return fuxLLVM->builder->getInt8Ty();
        case fuxType::U8:       return fuxLLVM->builder->getInt8Ty();
        case fuxType::C8:       return fuxLLVM->builder->getInt8Ty();
        case fuxType::I16:      return fuxLLVM->builder->getInt16Ty();
        case fuxType::U16:      return fuxLLVM->builder->getInt16Ty();
        case fuxType::C16:      return fuxLLVM->builder->getInt16Ty();
        case fuxType::I32:      return fuxLLVM->builder->getInt32Ty();
        case fuxType::U32:      return fuxLLVM->builder->getInt32Ty();
        case fuxType::F32:      return fuxLLVM->builder->getFloatTy();
        case fuxType::I64:      return fuxLLVM->builder->getInt64Ty();
        case fuxType::U64:      return fuxLLVM->builder->getInt64Ty();
        case fuxType::F64:      return fuxLLVM->builder->getDoubleTy();
        case fuxType::STR:      return fuxLLVM->builder->getInt8PtrTy(); // char* for now
        default:                return nullptr;
    }
}