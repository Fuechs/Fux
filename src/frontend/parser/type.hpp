/**
 * @file type.hpp
 * @author fuechs
 * @brief fux type header
 * @version 0.1
 * @date 2022-11-09
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

enum Type {
    VOID,
    BOOL,
    I8,
    U8,
    CHAR_TYPE,
    I16,
    U16,
    WCHAR,
    I32,
    U32,
    FLOAT_TYPE,
    I64,
    U64,
    DOUBLE,
    INT,
    UINT,
    STR,
    VAR,
    ENUM,
    STRUCT,
    ARRAY, // []
    CONSTANT, // ===
    NO_TYPE,
};

static const char *TypeString[] = {
    "VOID",
    "BOOL",
    "I8",
    "U8",
    "CHAR_TYPE",
    "I16",
    "U16",
    "WCHAR",
    "I32",
    "U32",
    "FLOAT_TYPE",
    "I64",
    "U64",
    "DOUBLE",
    "INT",
    "UINT",
    "STR",
    "VAR",
    "ENUM",
    "STRUCT",
    "ARRAY", // []
    "CONSTANT", // ===
    "NO_TYPE",
};