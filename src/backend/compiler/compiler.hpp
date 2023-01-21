/**
 * @file compiler.hpp
 * @author fuechs
 * @brief fux compiler header
 * @version 0.1
 * @date 2022-11-27
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../llvmheader.hpp"

#ifdef FUX_BACKEND

class Compiler {
public:
    Compiler(const string &fileName, Module *module);
    ~Compiler();

    void compile();

private:
    string fileName;
    Module *module;

    void debugPrint(const string message);
};

#endif