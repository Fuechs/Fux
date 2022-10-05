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

#include "../common.hpp"

namespace fux {

    using 
        std::string,
        std::cout
    ;

    typedef enum OpCode_ENUM {
        OP_RETURN,
    } OpCode;

    class Chunk {
    public:
        Chunk();

        std::vector<u8> code = {};

        void writeCode(u8 byte);
        void freeChunk();
        void disassembleChunk(string name);
        int disassembleInstruction(int offset);

    };
    
}

