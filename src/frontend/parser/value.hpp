/**
 * @file value.hpp
 * @author fuechs
 * @brief value struct header
 * @version 0.1
 * @date 2023-01-14
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../backend/llvmheader.hpp"
#include "type.hpp"

// used to represent values within a union
struct ValueStruct {
    ValueStruct(bool value) :   type(FuxType(FuxType::BOOL)), __bool(value) {}
    ValueStruct(_c8 value) :    type(FuxType(FuxType::C8)),  __c8(value) {}
    ValueStruct(_c16 value) :   type(FuxType(FuxType::C16)), __c16(value) {}
    ValueStruct(_i64 value) :   type(FuxType(FuxType::I64)), __i64(value) {}
    ValueStruct(_u64 value) :   type(FuxType(FuxType::U64)), __u64(value) {}
    ValueStruct(_f64 value) :   type(FuxType(FuxType::F64)), __f64(value) {}
    ValueStruct(string value) : type(FuxType(FuxType::LIT)), __lit(value) {}
    
    ~ValueStruct();

    #ifdef FUX_BACKEND
    Eisdrache::Local &getLLVMValue(Eisdrache* eisdrache);
    #endif

    // output saved value
    void debugPrint();

    FuxType type;
    union {
        bool    __bool;
        _c8     __c8;
        _c16    __c16;
        _i64    __i64;
        _u64    __u64;
        _f64    __f64;
        string  __lit;
    };
};