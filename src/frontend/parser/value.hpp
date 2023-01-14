/**
 * @file value.hpp
 * @author fuechs
 * @brief value struct header
 * @version 0.1
 * @date 2023-01-14
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#pragma once

#include "../../backend/llvmheader.hpp"
#include "../../backend/generator/wrapper.hpp"
#include "type.hpp"

// used to represent values within a union
struct ValueStruct {
    // TODO: constructor for every type
    ValueStruct(_i64 value) : type(FuxType(FuxType::I64)), __i64(value) {}
    ValueStruct(_f64 value) : type(FuxType(FuxType::F64)), __f64(value) {}
    
    ~ValueStruct();

    Value *getLLVMValue(LLVMWrapper* fuxLLVM);

    FuxType type;
    union {
        bool    __bool;
        _i8     __i8;
        _u8     __u8;
        _c8     __c8;
        _i16    __i16;
        _u16    __u16;
        _c16    __c16;
        _i32    __i32;
        _u32    __u32;
        _f32    __f32;
        _i64    __i64;
        _u64    __u64;
        _f64    __f64;
        string  __str;
    };
};