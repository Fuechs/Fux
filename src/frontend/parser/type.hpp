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

namespace fuxType {

enum Type {
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
    "C8",
    "I16",
    "U16",
    "C16",
    "I32",
    "U32",
    "F32",
    "I64",
    "U64",
    "F64",
    "STR",
    "VAR",
    "ENUM",
    "STRUCT",
    "ARRAY", // []
    "CONSTANT", // ===
    "NO_TYPE",
};

}