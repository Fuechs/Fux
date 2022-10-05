/**
 * @file chunk.cpp
 * @author fuechs 
 * @brief fux chunk
 * @version 0.1
 * @date 2022-10-05
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "chunk.hpp"

namespace fux {

    void Chunk::writeCode(u8 byte) {
        code.push_back(byte);
    }

    void Chunk::freeChunk() {
        code = {};
    }

    void Chunk::disassembleChunk(string name) {
        cout << "== " << name << " ==\n";
        for (int offset = 0; offset < code.size();)
            offset = disassembleInstruction(offset);
    }

    int Chunk::disassembleInstruction(int offset) {
        cout << std::format("{:04}", offset) << '\n';
    }

}