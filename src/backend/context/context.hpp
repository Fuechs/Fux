/**
 * @file context.hpp
 * @author fuechs
 * @brief fux context header
 * @version 0.1
 * @date 2022-12-27
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "../llvmheader.hpp"
#include "../generator/generator.hpp"
#include "../compiler/compiler.hpp"
#include "../generator/wrapper.hpp"

/**
 * Manages...
 * ...Generator
 * ...Optimizer
 * ...Compiler
 * ...LLVMContext
 * 
 */
class FuxContext {
public:
    FuxContext(ExprPtr &root);
    ~FuxContext();

    LLVMWrapper *fuxLLVM;
    Module *module;
    string target;

    void run();

private:
    ExprPtr root;
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