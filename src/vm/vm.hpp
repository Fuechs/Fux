/**
 * @file vm.hpp
 * @author fuechs 
 * @brief fux vm header
 * @version 0.1
 * @date 2022-10-13
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include <cstdint>
#include <vector>

#include "instruction.hpp"

namespace fux {

    using 
        std::vector
    ;

    class VM {
    public:
        vector<Instruction> code;
        vector<uint32_t> stack;
        Instruction *currentInstruction;

        void run() {

            while (currentInstruction != nullptr) {
                switch (currentInstruction->opcode) {
                    
                    case EXIT: 
                        currentInstruction = nullptr;
                        break;

                    case ADD_I32: 
                        stack.push_back(currentInstruction->p2);
                        break;
                    case PUSH_I32: break;
                    case PRINT_I32: break;
                    
                }
            }

        }
    };

}