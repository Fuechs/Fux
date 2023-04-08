/**
 * @file compiler.cpp
 * @author fuechs
 * @brief fux compiler
 * @version 0.1
 * @date 2022-12-20
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "compiler.hpp"

#ifdef FUX_BACKEND

Compiler::Compiler(const string &fileName, llvm::Module *module) 
: fileName(fileName), module(module) {}

Compiler::~Compiler() {
    fileName.clear();
    delete module;
}

void Compiler::compile() {
    return;
}

#endif