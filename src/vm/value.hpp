/**
 * @file value.hpp
 * @author fuechs
 * @brief fux value header
 * @version 0.1
 * @date 2022-10-10
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

enum class ValueType {
    I32,
};

struct Value {
    ValueType type;
    union {
        int _i32;
    };
};

// Constructors

#define I32(value) ((Value) {ValueType::I32, ._i32 = value})

// Accessors

#define AS_I32(value) ((int)(value)._i32)