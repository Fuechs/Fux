/**
 * @file string.hpp
 * @author fuechs
 * @brief fux string type
 * @version 0.1
 * @date 2022-12-28
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "generator.hpp"

void Generator::defineStringType() {
    LLVMContext *context = fuxLLVM->context;
    Module *module = fuxLLVM->module;
    IRBuilder<> *builder = fuxLLVM->builder;

    // %str = type { i8*, i64, i64, i64 }
    StructType *str = StructType::create(*context, {
        builder->getInt8PtrTy(),
        builder->getInt64Ty(),
        builder->getInt64Ty(),
        builder->getInt64Ty()
    }, "str", false);
}