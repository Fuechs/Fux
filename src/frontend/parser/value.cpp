/**
 * @file value.cpp
 * @author fuechs
 * @brief value struct
 * @version 0.1
 * @date 2023-01-14
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "value.hpp"

ValueStruct::~ValueStruct() {
    if (type.kind == FuxType::LIT)
        __lit.clear();
}

#ifdef FUX_BACKEND
Value *ValueStruct::getLLVMValue(LLVMWrapper *fuxLLVM) {
    switch (type.kind) {
        case FuxType::BOOL:     return fuxLLVM->builder->getInt1(__bool);
        case FuxType::I8:       return fuxLLVM->builder->getInt8(__i8);
        case FuxType::U8:       return fuxLLVM->builder->getInt8(__u8);
        case FuxType::C8:       return fuxLLVM->builder->getInt8(__c8);
        case FuxType::I16:      return fuxLLVM->builder->getInt16(__i16);
        case FuxType::U16:      return fuxLLVM->builder->getInt16(__u16);
        case FuxType::C16:      return fuxLLVM->builder->getInt16(__c16);
        case FuxType::I32:      return fuxLLVM->builder->getInt32(__i32);
        case FuxType::U32:      return fuxLLVM->builder->getInt32(__u32);
        case FuxType::F32:      return ConstantFP::get(fuxLLVM->builder->getFloatTy(), __f32);
        case FuxType::I64:      return fuxLLVM->builder->getInt64(__i64);
        case FuxType::U64:      return fuxLLVM->builder->getInt64(__u64);
        case FuxType::F64:      return ConstantFP::get(fuxLLVM->builder->getDoubleTy(), __f64);
        case FuxType::LIT:      {
            Value *globalLiteral = fuxLLVM->builder->CreateGlobalStringPtr(__lit, "literal_");
            fuxLLVM->values[globalLiteral->getName().str()] = FuxValue::Literal(globalLiteral);
            return globalLiteral;
        }
        default:                return nullptr;
    }
}
#endif