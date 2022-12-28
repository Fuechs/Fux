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
    this->mlirContext = new mlir::MLIRContext();
    this->passManager = new mlir::PassManager(mlirContext, "fux pass manager");
    this->target = fux.options.target;
    this->phase = FuxPhase::FLIR;

    this->root = root;
    this->generator = new Generator(root, nullptr, nullptr);
    this->compiler = nullptr; // will be required after generation
}

FuxContext::~FuxContext() {
    delete llvmContext;
    delete root;
    delete compiler;
}