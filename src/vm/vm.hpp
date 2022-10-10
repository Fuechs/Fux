/**
 * @file vm.hpp
 * @author fuechs
 * @brief fux virtual machine header
 * @version 0.1
 * @date 2022-10-10
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <array>

#include "chunk.hpp"
#include "value.hpp"
    
#define READ_BYTE() *ip++
#define GET_CONST() constants[READ_BYTE()]
#define STACK_LIMIT 512

namespace fux {


    using
        std::string,
        std::vector,
        std::cerr,
        std::array
    ;

    class RuntimeError {
    public:
        RuntimeError(string message) {
            cerr 
                << "[RuntimeError occurred at Line ?:?]: " 
                << message
                << '\n';
            exit(1);
        }
    };

    class UnknownInstError {
    public:
        UnknownInstError(u8 instruction) {
            cerr
                << "[UnknownInstError]: "
                << std::hex 
                << instruction 
                << '\n';
            exit(1);
        }
    };

    class VM {
    public:
        VM() {}

        /**
         * @brief instruction pointer
         * 
         */
        u8* ip;

        /**
         * @brief stack pointer
         * 
         */
        Value* sp;

        /**
         * @brief operands stack
         * 
         */
        array<Value, STACK_LIMIT> stack;


        vector<Value> constants;

        /**
         * @brief executes a program
         * 
         * @param chunk : bytecodes 
         */
        Value exec(Chunk &chunk);

        void push(Value value);
        
        Value pop();

    };

    void debugInstruction(u8 code);
    
}