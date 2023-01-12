/**
 * @file type.hpp
 * @author fuechs
 * @brief fux type header
 * @version 0.1
 * @date 2022-11-09
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"

class FuxType {
public:
    enum Kind { 
        VOID,   // void
        BOOL,   // boolean
        I8,     // 8-bit signed integer
        U8,     // 8-bit unsigned integer
        C8,     // UTF-8 code block
        I16,    // 16-bit signed integer
        U16,    // 16-bit unsigned integer
        C16,    // UTF-16 code block
        I32,    // 32-bit signed integer
        U32,    // 32-bit unsigned integer
        F32,    // 32-bit floating point integer
        I64,    // 64-bit signed integer
        U64,    // 64-bit unsigned integer
        F64,    // 64-bit floating point integer
        STR,    // string ; array of c8
        VAR,    // dynamic 
        CUSTOM, // user defined type
        NO_TYPE,
    };

    enum Access {
        FIXED,      // does not modify the class it's defined within
        FINAL,      // will turn into a constant after initialisation
        CONSTANT,   // constant; value can't be changed
        SAFE,       // read-only outside of the class / module
        INTERN,     // only accessible from within the class / module
        PUBLIC,     // read and write access for everyone / everywhere
    };
    
    FuxType(Kind kind = NO_TYPE, size_t pointerDepth = 0, Access access = PUBLIC, string customName = "")
    : kind(kind), pointerDepth(pointerDepth), access(access), customName(customName) {}

    FuxType &operator=(FuxType copy) {
        this->kind = copy.kind;
        this->pointerDepth = copy.pointerDepth;
        this->access = copy.access;
        return *this;
    }

    Kind kind;
    size_t pointerDepth;
    Access access;
    string customName; // name of the user defined type; only used if kind = CUSTOM
    
    // return string representation of this type (for debug purposes)
    string str() {
        stringstream ss;
        for (size_t pd = pointerDepth; pd --> 0;)
            ss << "*";
        ss << KindString[kind] << " (" << AccessString[access] << ")"; 
        if (!customName.empty())
            ss << " ['" << customName << "']";
        return ss.str();
    }

private:
    // string representations of enum elements
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
        "custom",
        "no_type",
    };

    const vector<string> AccessString = {
        "fixed",
        "final",
        "constant",
        "safe",
        "intern",
        "public",
    };
};