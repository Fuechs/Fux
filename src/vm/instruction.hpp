/**
 * @file instruction.hpp
 * @author fuechs
 * @brief fux instruction header
 * @version 0.1
 * @date 2022-10-13
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include <cstdint>

namespace fux {

    typedef enum OpCode_ENUM: uint8_t {
        EXIT,
        ADD_I32,
        PUSH_I32,
        PRINT_I32,
    } OpCode;

    class Instruction {
    public:
        OpCode opcode;
        uint32_t p1;
        uint32_t p2;  
    };

}