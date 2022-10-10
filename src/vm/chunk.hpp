/**
 * @file chunk.hpp
 * @author fuechs
 * @brief fux chunk header
 * @version 0.1
 * @date 2022-10-05
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include <vector>
#include <string>
#include <iostream>

namespace fux {

    using 
        std::string,
        std::cout,
        std::vector
    ;

    typedef uint8_t u8;

    typedef enum OpCode_ENUM {
        OP_HALT,            // stops the program
        OP_CONST,           // pushes const onto the stack
        OP_ADD,
        OP_SUB,
        OP_MUL,
        OP_DIV,
    } OpCode;

    static const char *OpCodeString[] = {
        "OP_HALT",
        "OP_CONST",
        "OP_ADD",
        "OP_SUB",
        "OP_MUL",
        "OP_DIV",
    };

    typedef vector<u8> Chunk;
    
}

