/**
 * @file context.hpp
 * @author fuechs
 * @brief fux context header
 * @version 0.1
 * @date 2022-12-27
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../llvmheader.hpp"

#ifdef FUX_BACKEND
#include "../generator/generator.hpp"
#include "../compiler/compiler.hpp"

// Manages whole backend of the compiler
class FuxContext {
public:
    FuxContext(Root::Ptr &root);
    ~FuxContext();

    Eisdrache *eisdrache;

    void run();

private:
    Root::Ptr root;
    Generator *generator;
    Compiler *compiler;

    /* generate IR */
    void generate();
    /* optimize IR */
    void optimize();
    /* compile to assembly */
    void compile();

    void debugPrint(const string message);
};
#endif