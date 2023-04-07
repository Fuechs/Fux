/**
 * @file type.hpp
 * @author fuechs
 * @brief fux type representation
 * @version 0.1
 * @date 2023-04-07
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../fux.hpp"
#include "token.hpp"

class Expr { public: using Ptr = std::unique_ptr<Expr>; };
class Sym { public: using Ptr = std::shared_ptr<Sym>; };
class Context { public: using Ptr = std::shared_ptr<Context>; };

/**
 * @brief Representation of a Type in an AST and in the LLVM IR
 */
class Type {
public:
    using Ptr = std::shared_ptr<Type>;
    using Vec = std::vector<Ptr>;

    enum Access {
        PUB,
        SAFE,
        INTERN,
        FIXED,
        FINAL,
        CONST,
        NONE,
    };

    using AccessVec = std::vector<Access>;

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
        LIT,                    // string literal ; array of c8
        VAR     = KEY_VAR,      // dynamic (used for variadic parameters)
        AUTO,                   // automatic typing
        NO_TYPE,                
    };

    Type(Kind kind = NO_TYPE, AccessVec access = {}, size_t ptr = 0, bool ref = false, bool array = false, 
        Expr::Ptr size = nullptr, Sym::Ptr structure = nullptr, Expr::Ptr custom = nullptr, Context::Ptr ctx = nullptr);
    ~Type();
    
    // using this may make the original (RHS) type invalid
    // since the array size and custom expr will be moved
    Type &operator=(Type &copy);
    bool operator==(const Type &comp) const;
    
    // this type with ptr + 1
    Type::Ptr getPointerTo();
    // this type with ptr - 1
    Type::Ptr getPointee();
    // this type with ref = true
    Type::Ptr getReferenceTo();
    // array of this type
    Type::Ptr getArrayOf(Expr::Ptr size = nullptr);

    // get the name of this type
    std::string getName();
    // get the mangled name of this type (used to avoid duplicate symbols)
    std::string getMangledName();

    static Type::Ptr getNormal(Kind kind, AccessVec access = {}, size_t ptr = 0, bool ref = false, Expr::Ptr custom = nullptr, Context::Ptr ctx = nullptr);
    static Type::Ptr getArray(Type::Ptr elementTy, Expr::Ptr size = nullptr, Context::Ptr ctx = nullptr);

    Kind kind;          // kind of type
    AccessVec access;   // access of type
    size_t ptr;         // pointer depth
    bool ref;           // wether this is a reference
    bool array;         // wether this is an array type
    Expr::Ptr size;     // optional array size
    Sym::Ptr structure; // optional structure type
    Expr::Ptr custom;   // provided symbol
    Context::Ptr ctx;   // context
};