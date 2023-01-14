/**
 * @file value.cpp
 * @author fuechs
 * @brief value struct
 * @version 0.1
 * @date 2023-01-14
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#include "value.hpp"

ValueStruct::~ValueStruct() {
    if (type.kind == FuxType::STR)
        __str.clear();
    delete &type;
}

Value *ValueStruct::getLLVMValue(LLVMWrapper *fuxLLVM) {
    // TODO: add all types
    switch (type.kind) {
        case FuxType::I64:      return fuxLLVM->builder->getInt64(__i64);
        case FuxType::F64:      return ConstantFP::get(fuxLLVM->builder->getDoubleTy(), __f64);
        default:                return nullptr;
    }
}