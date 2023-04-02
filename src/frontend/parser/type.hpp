/**
 * @file type.hpp
 * @author fuechs
 * @brief fux type header
 * @version 0.1
 * @date 2022-11-09
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../lexer/token.hpp"
#include "../../backend/llvmheader.hpp"
#include "../metadata.hpp"

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
        F16     = KEY_F16,      // 16-bit floating pointer integer
        I32     = KEY_I32,      // 32-bit signed integer
        U32     = KEY_U32,      // 32-bit unsigned integer
        F32     = KEY_F32,      // 32-bit floating point integer
        I64     = KEY_I64,      // 64-bit signed integer
        U64     = KEY_U64,      // 64-bit unsigned integer
        F64     = KEY_F64,      // 64-bit floating point integer
        LIT,                    // string ; array of c8
        VAR     = KEY_VAR,      // dynamic 
        AUTO,                   // automatic typing
        NO_TYPE,                
    };

    enum Access {
        SAFE        = KEY_SAFE,     // read-only outside of the class / module
        INTERN      = KEY_INTERN,   // only accessible from within the class / module
        FINAL       = KEY_FINAL,    // will turn into a constant after initialisation
        FIXED       = KEY_FIXED,    // does not modify the class it's defined within
        ASYNC       = KEY_ASYNC,    // async function that interacts with APIs / a network
        CONSTANT,                   // constant; value can't be changed
        PUBLIC,                     // read and write access for everyone / everywhere & default (for values too)
    };

    using AccessList = vector<Access>;
    using Vec = vector<FuxType>;

    FuxType(Kind kind = NO_TYPE, size_t pointerDepth = 0, bool reference = false, AccessList accessList = {}, bool array = false, _i64 sizeID = -1, string name = "")
    : kind(kind), pointerDepth(pointerDepth), reference(reference), access(accessList), array(array), sizeID(sizeID), name(name) {}
    ~FuxType();

    FuxType &operator=(const FuxType &copy);
    bool operator==(const FuxType &comp) const;
    bool operator!();

    // shorthand for normal types
    static FuxType createStd(Kind kind, size_t pointerDepth = 0, bool reference = false, AccessList accessList = {}, string name = "");
    // shorthand for reference types
    static FuxType createRef(Kind kind, size_t pointerDepth = 0, AccessList accessList = {}, string name = "");
    // shorthand for array types
    static FuxType createArray(Kind kind, size_t pointerDepth = 0, bool reference = false, AccessList accessList = {}, string name = "", _i64 sizeID = -1);    
    // shorthand for primitive types (e.g. for type casts)
    static FuxType createPrimitive(Kind kind, size_t pointerDepth = 0, bool array = false, string name = "");

    // return FuxType::AccessList as string
    string accessAsString(char delim = ' ');
    // return FuxType::Kind as string
    string kindAsString();
    // return this type as a mangled string
    string mangledString();
    // output string representation of type
    void debugPrint(bool primitive = false);

    // check wether type is valid
    bool valid();
    
    Kind kind;
    
    //  0 --> Value
    //  N --> Pointer with depth of N 
    size_t pointerDepth; 
    bool reference;
    // access modifiers
    AccessList access;
    // string value of the type
    // relevant for user defined types and debugPrint()
    string name;
    // is an array type
    bool array; 
    // relevant for array types
    // -1 -> no size
    //  N -> ID of array size expr stored in RootAST
    _i64 sizeID;
    Metadata meta; // position where type was parsed 
};