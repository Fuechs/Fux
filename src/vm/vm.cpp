/**
 * @file vm.cpp
 * @author fuechs
 * @brief fux virtual machine
 * @version 0.1
 * @date 2022-10-16
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "vm.hpp"

namespace fux {

    void VM::run() {

            while (currentInstruction != nullptr) {
                switch (currentInstruction->opcode) {
                    
                    case EXIT: 
                        currentInstruction = nullptr;
                        break;

                    case ADD_I32: {
                        uint32_t leftHandSide = stack.back();
                        stack.pop_back();
                        uint32_t rightHandSide = stack.back();
                        stack.pop_back();
                        stack.push_back(leftHandSide+rightHandSide);
                        ++currentInstruction;
                        break;
                    }

                    case PUSH_I32: 
                        stack.push_back(currentInstruction->p2);
                        ++currentInstruction;
                        break;

                    case PRINT_I32: {
                        uint32_t number = stack.back();
                        stack.pop_back();
                        cout << "Number Printed: " << number << endl;
                        ++currentInstruction;
                        break;
                    }
                    
                }
            }

        }

}