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
    if (type.kind == FuxType::STR)
        __str.clear();
    // FIXME: "malloc: ... pointer being freed was not allocated" why?
    // delete &type;
}

Value *ValueStruct::getLLVMValue(LLVMWrapper *fuxLLVM) {
    // TODO: how to implement pointer types?
    switch (type.kind) {
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
        case FuxType::STR:      {
            // TODO: this is just "dummy" code; don't know wether it actually works.
            // *object: str;
            Value *object = fuxLLVM->builder->CreateAlloca(fuxLLVM->fuxStr->str); 
            // *object = str();
            fuxLLVM->builder->CreateCall(fuxLLVM->fuxStr->str_create_default, {object});
            // *buffer: c8 = object.buffer;
            Value *buffer = fuxLLVM->builder->CreateCall(fuxLLVM->fuxStr->str_get_buffer, {object});
            // *char_array: c8 = this->__str.c_str();
            Value *char_array = fuxLLVM->builder->CreateGlobalString(__str); 
            // resize(object, size + 1);
            fuxLLVM->builder->CreateCall(fuxLLVM->fuxStr->str_resize,
                {object, fuxLLVM->builder->getInt64(__str.size() + 1)});
            // memcpy(buffer, char_array, size + 1);
            fuxLLVM->builder->CreateCall(fuxLLVM->fuxMem->memcpy, 
                {buffer, char_array, fuxLLVM->builder->getInt64(__str.size() + 1)});
            return object;
        }
        default:                return nullptr;
    }
}