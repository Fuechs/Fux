/**
 * @file type.hpp
 * @author fuechs
 * @brief fux type header
 * @version 0.1
 * @date 2022-11-09
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs. All rights reserved.
 * 
 */

#pragma once




class FuxType {
public:
    enum Kind {
        VOID,
        BOOL,
        I8,
        U8,
        C8,
        I16,
        U16,
        C16,
        I32,
        U32,
        F32,
        I64,
        U64,
        F64,
        STR,
        VAR,
        ARRAY, // []
        NO_TYPE,
    };

    enum Access {
        FIXED,
        CONSTANT,
        SAFE,
        INTERN,
        PUBLIC,
    };
    
    FuxType(Kind kind = NO_TYPE, size_t pointerDepth = 0, Access access = PUBLIC)
    : kind(kind), pointerDepth(pointerDepth), access(access) {}

    Kind kind;
    size_t pointerDepth;
    Access access;
};