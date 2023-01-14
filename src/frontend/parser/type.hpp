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
#include "../lexer/token.hpp"
#include "expr.hpp"

class FuxType {
public:
    // Possible kinds of data types -- Mapped to respective keyword value
    enum Kind { 
        CUSTOM  = IDENTIFIER,   // user defined type
        VOID    = KEY_VOID,     // void
        BOOL    = KEY_BOOL,     // boolean
        I8      = KEY_I8,       // 8-bit signed integer
        U8      = KEY_U8,       // 8-bit unsigned integer
        C8      = KEY_C8,       // UTF-8 code block
        I16     = KEY_I16,      // 16-bit signed integer
        U16     = KEY_U16,      // 16-bit unsigned integer
        C16     = KEY_C16,      // UTF-16 code block
        I32     = KEY_I32,      // 32-bit signed integer
        U32     = KEY_U32,      // 32-bit unsigned integer
        F32     = KEY_F32,      // 32-bit floating point integer
        I64     = KEY_I64,      // 64-bit signed integer
        U64     = KEY_U64,      // 64-bit unsigned integer
        F64     = KEY_F64,      // 64-bit floating point integer
        STR     = KEY_STR,      // string ; array of c8
        VAR     = KEY_VAR,      // dynamic 
        AUTO,                   // automatic typing
        NO_TYPE,                // used to check for auto typing
    };

    enum Access {
        FIXED,      // does not modify the class it's defined within
        FINAL,      // will turn into a constant after initialisation
        CONSTANT,   // constant; value can't be changed
        SAFE,       // read-only outside of the class / module
        INTERN,     // only accessible from within the class / module
        PUBLIC,     // read and write access for everyone / everywhere & default (for values too)
    };

    typedef vector<Access> AccessList;

    FuxType(Kind kind = NO_TYPE, int64_t pointerDepth = 0, AccessList accessList = {PUBLIC}, bool array = false, ExprPtr &arraySize = nullExpr, string name = "")
    : kind(kind), pointerDepth(pointerDepth), access(accessList), array(array), arraySize(std::move(arraySize)), name(name) {}

    FuxType(const FuxType &copy) { operator=(copy); }
    
    ~FuxType() {
        access.clear();
        name.clear();
    }

    FuxType &operator=(FuxType copy) {
        this->kind = copy.kind;
        this->pointerDepth = copy.pointerDepth;
        this->access = copy.access;
        return *this;
    }

    bool operator!() { return kind == NO_TYPE; }

    Kind kind;
    /**
     * -1 --> Reference
     *  0 --> Value
     *  N --> Pointer with depth of N 
     */
    int64_t pointerDepth; 
    AccessList access;
    string name; // string value of the type; relevant for user defined types
    bool array; // is an array type
    ExprPtr arraySize; // relevant for array types; nullExpr -> no size

    // shorthand for normal types
    static FuxType createStd(Kind kind, int64_t pointerDepth, AccessList accessList, string name) {
        return FuxType(kind, pointerDepth, accessList, false, nullExpr, name);
    }

    // shorthand for reference types
    static FuxType createRef(Kind kind, AccessList accessList, string name) {
        return FuxType(kind, -1, accessList, false, nullExpr, name); 
    }

    // shorthand for array types
    static FuxType createArray(Kind kind, int64_t pointerDepth, AccessList accessList, string name, ExprPtr &arraySize = nullExpr) {
        return FuxType(kind, pointerDepth, accessList, true, arraySize, name);
    }
    
    // get -> | : + pointer depth for this type 
    string prefix() {
        switch (pointerDepth) {
            case -1:    return " -> ";
            case 0:     return ": ";
            default: {
                stringstream ss;
                ss << ": ";
                for (size_t pd = pointerDepth; pd --> 0;)
                    ss << "*";
                ss << " ";
                return ss.str();
            }
        }
    }

    // get typename + accesslist for this type 
    string suffix() {
        stringstream ss;

        if (kind == CUSTOM)
            ss << "'" << name << "'";
        else
            ss << KindString[kind];
        
        if (array) 
            ss << "[" << arraySize << "]";
        
        ss << " (";
        for (Access &a : access)
            ss << AccessString[a] << ",";
        ss << ") ";
        return ss.str();
    }

    // get prefix + suffix for this type
    string str() { return prefix() + suffix(); }

private:
    // string representations of enum elements
    const vector<string> KindString = {
        "", // custom, unreachable
        "void",
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
        "auto",
        "no_type"
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