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

#include "../fux.hpp"
#include "../lexer/token.hpp"
#include "../llvmheader.hpp"
#include "../util/metadata.hpp"

class Expr;

namespace Fux {

class Pointer;
class Reference; 
class Array;

class Type {
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
        PTR,                    // pointer
        REF,                    // reference
        ARR,                    // array
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
    using Vec = vector<Type>;

    Type(Kind kind = NO_TYPE, AccessList accessList = {}, string name = "");
    ~Type();

    Type &operator=(const Type &copy);
    bool operator==(const Type &comp) const;
    bool operator!() const;

    Pointer getPointerTo();

    // return FuxType::AccessList as string
    string accessAsString(char delim = ' ');
    // return FuxType::Kind as string
    string kindAsString();
    // return this type as a mangled string
    virtual string mangled();
    // output string representation of type
    virtual void debugPrint(bool primitive = false);

    // check wether type is valid
    bool valid();
    
    Kind kind;

    // access modifiers
    AccessList access;
    
    // string value of the type
    // relevant for user defined types and debugPrint()
    string name;

    // position where type was parsed 
    Metadata meta;
};

class Pointer : public Type {
public:
    Pointer(Type &pointee) : pointee(pointee) {}

    string mangled() override;
    void debugPrint(bool primitive = false) override;

    Type pointee;
};

class Reference : public Type {
public:
    string mangled() override;
    void debugPrint(bool primitive = false) override;

    Type reference;
};

class Array : public Type {
public:
    Array(Type member = NO_TYPE, shared_ptr<Expr> size = nullptr);

    string mangled() override;
    void debugPrint(bool primitive = false) override;

    Type member;
    shared_ptr<Expr> size;
};

}