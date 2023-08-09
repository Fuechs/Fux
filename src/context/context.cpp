/**
 * @file context.cpp
 * @author fuechs
 * @brief fux context
 * @version 0.1
 * @date 2023-03-02
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "context.hpp"

#ifdef FUX_BACKEND

FuxContext::FuxContext(Root::Ptr &root) {
    Eisdrache::initialize();  
    this->eisdrache = Eisdrache::create("fux compiler");
    this->root = root;
    this->generator = nullptr; 
    this->compiler = nullptr; // initialize compiler to null, will be required after generation
}

FuxContext::~FuxContext() {
    delete eisdrache; // delete the LLVM wrapper
    delete compiler; 
}

void FuxContext::run() {
    debugPrint("Generating."); // print debug message
    generate(); // generate LLVM IR
    debugPrint("Optimizing."); // print debug message
    optimize(); // optimize the LLVM IR
    debugPrint("Compiling."); // print debug message
    compile(); // compile the LLVM IR
}

void FuxContext::generate() {
    generator = new Generator(root, eisdrache); // create a new generator with the root AST and Eisdrache wrapper
    generator->generate(); // generate LLVM IR
}

void FuxContext::optimize() { return; } // no optimization implemented

void FuxContext::compile() {
    delete generator; // delete the generator
    compiler = new Compiler("<placeholder>", eisdrache->getModule()); // create a new compiler with a placeholder filename and the LLVM module
}

#endif
