// Created by Fuechs on 2022-09-26

#pragma once

#include <string>
#include <vector>

namespace fux {

    enum BUILTIN_TYPE {
        VOID,       // void, nothing, nada
        BOOL,       // boolean
        I8,         // 8-bit integer
        U8,         // unsigned 8-bit integer
        I16,        // 16-bit integer
        U16,        // unsigned 16-bit integer
        I32,        // 32-bit integer
        U32,        // unsigned 32-bit integer
        FLOAT,      // 32-bit floating point integer
        I64,        // 64-bit integer
        U64,        // unsigned 64-bit integer
        DOUBLE,     // 64-bit floating point integer
        STR,        // string
        STRUCT,     // struct
        ENUM,       // TODO: enum
    };

    /**
     * @brief Individual type in fux 
     * ~ Examples: i32, bool, str
     */
    class Type {
    public:
        Type(const std::string name = "", enum BUILTIN_TYPE type = VOID) 
        : mName(name), mType(type) {}

        std::string mName;
        enum BUILTIN_TYPE mType;
        std::vector<Type> mFields; // for STRUCT only
    };

}