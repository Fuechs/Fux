/**
 * @file context.cpp
 * @author Fyronix
 * @brief fux context
 * @version 0.1
 * @date 2023-03-02
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "context.hpp"

#ifdef FUX_BACKEND

FuxContext::FuxContext(RootAST::Ptr &root) {
    llvm::InitializeNativeTarget(); // initialize the LLVM native target
    LLVMContext *llvmContext = new LLVMContext(); 
    this->fuxLLVM = new LLVMWrapper(
        llvmContext, 
        new Module("fux compiler", *llvmContext), // create a new LLVM module with the name "fux compiler"
        new IRBuilder<>(*llvmContext) // create a new IR builder with the LLVM context
    );
    this->target = fux.options.target; 

    this->root = std::move(root);
    this->generator = nullptr; 
    this->compiler = nullptr; // initialize compiler to null, will be required after generation
}

FuxContext::~FuxContext() {
    delete fuxLLVM; // delete the LLVM wrapper
    target.clear(); 
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
    generator = new Generator(root, fuxLLVM); // create a new generator with the root AST and LLVM wrapper
    generator->generate(); // generate LLVM IR
}

void FuxContext::optimize() { return; } // no optimization implemented

void FuxContext::compile() {
    delete generator; // delete the generator
    compiler = new Compiler("<placeholder>", module); // create a new compiler with a placeholder filename and the LLVM module
}

#endif
