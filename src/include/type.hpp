/**
 * @file type.hpp
 * @author your name (you@domain.com)
 * @brief fux type header
 * @version 0.1
 * @date 2022-09-27
 * 
 * @copyright Copyright (c) 2022, Fuechs.
 *            All rights reserved.
 *            BSD 3-Clause License
 */

#pragma once

#include <string>
#include <map>

namespace fux {

    using std::string, std::map;

    typedef enum BuiltInType_ENUM {
        VOID,
        I8,
        U8,
        I16,
        U16,
        I32,
        U32,
        FLOAT,
        I64,
        U64,
        DOUBLE,
    } BuiltInType;

    class Type {
    public:
        Type(BuiltInType type = VOID, string value = "void") : type(type), value(value) {}

        BuiltInType type;
        string value;
    };

    typedef map<string, Type> TypeMap;

}