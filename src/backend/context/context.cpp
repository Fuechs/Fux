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
    this->builder = new IRBuilder<>(*llvmContext);
    // this->targetTripe = ...;
    this->phase = FuxPhase::FLIR;

    this->root = root;
    this->generator = new Generator(root, module, builder);
    this->compiler = nullptr; // will be required after generation
}

FuxContext::~FuxContext() {
    delete llvmContext;
    delete module;
    delete builder;
    delete root;
    delete compiler;
}

