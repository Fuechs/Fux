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
    
    string str() {
        stringstream ss;
        for (size_t pd = pointerDepth; pd --> 0;)
            ss << "*";
        ss << KindString[kind] << " (" << AccessString[access] << ")"; 
        return ss.str();
    }

private:
    const vector<string> KindString = {
        "void",
        "bool",
        "i8",
        "u8",
        "c8",
        "i16",
        "u16",
        "c16",
        "i32",
        "u32",
        "f32",
        "i64",
        "u64",
        "f64",
        "str",
        "var",
        "no_type",
    };

    const vector<string> AccessString = {
        "fixed",
        "constant",
        "safe",
        "intern",
        "public",
    };
};