/**
 * @file context.cpp
 * @author fuechs
 * @brief fux context
 * @version 0.1
 * @date 2022-12-27
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "context.hpp"

FuxContext::FuxContext(RootAST *root) {
    this->llvmContext = new LLVMContext();
    this->module = new Module("fux compiler", *llvmContext);
    this->target = fux.options.target;

    this->root = root;
    this->generator = nullptr;
    this->compiler = nullptr; // will be required after generation
}

FuxContext::~FuxContext() {
    delete llvmContext;
    delete module;
    target.clear();
    delete compiler;
}

void FuxContext::run() {
    debugPrint("Generating.");
    generate();
    debugPrint("Optimizing.");
    optimize();
    debugPrint("Compiling.");
    compile();
}

void FuxContext::generate() {
    generator = new Generator(root, module);
}

void FuxContext::optimize() { return; }

void FuxContext::compile() {
    delete generator;
    compiler = new Compiler("<placeholder>", module);

}