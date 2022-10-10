/**
 * @file vm.cpp
 * @author fuechs
 * @brief fux virtual machine
 * @version 0.1
 * @date 2022-10-10
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "vm.hpp"

namespace fux {

    Value VM::exec(Chunk &chunk) {
        // set instruction pointer to the beginning
        ip = &chunk[0];

        for (;;) {
            debugInstruction(*ip);
            switch (READ_BYTE()) {

                case OP_HALT:   return pop();

                case OP_CONST:
                    auto constIndex = READ_BYTE();
                    auto constant = constants[constIndex];
                    push(GET_CONST());
                    break;

                default:        UnknownInstError(*--ip);            
            } 
        }
    }

    void VM::push(Value value) {
        if ((size_t)(sp - stack.begin()) == STACK_LIMIT)
            RuntimeError("push(): Stack overflow.");
        *sp = value;
        ++sp;
    }

    Value VM::pop() {
        if (sp == stack.begin()) {
            RuntimeError("pop(): empty stack.");
        }
        --sp;
        return *sp;
    }

    void debugInstruction(u8 code) {
        cout 
            << "|   "
            << OpCodeString[code] << " "
            << code
            << '\n';
    }

}