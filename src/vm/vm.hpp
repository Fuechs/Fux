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
#include <iostream>

#include "instruction.hpp"

namespace fux {

    using 
        std::vector,
        std::cout,
        std::endl
    ;

    class VM {
    public:
        vector<Instruction> code;
        vector<uint32_t> stack;
        Instruction *currentInstruction;

        void run();
    };

}