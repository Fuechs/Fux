/**
 * @file context.hpp
 * @author fuechs
 * @brief fux context header
 * @version 0.1
 * @date 2022-12-27
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "../llvmheader.hpp"
#include "../generator/generator.hpp"
#include "../compiler/compiler.hpp"

/**
 * FLIR:    ...    
 * MLIR:    ...Lowered FLIR to MLIR
 * LIR:     ...Lowered MLIR to LIR
 * IR:      ...Lowered LIR to LLVM IR
 * NOOPT:   ...No optimization
 * OPTN:    ...Nth optimization
 */
enum class FuxPhase {
    FLIR,
    MLIR,
    LIR,
    IR,
    NOOPT,
    OPT1, 
    OPT2, 
    OPT3, 
};

/**
 * imagine description here.
 * 
 */
class FuxContext {
public:
    FuxContext(RootAST *root);
    ~FuxContext();

    LLVMContext *llvmContext;
    Module *module;
    IRBuilder<> *builder;
    // MLIRContext *mlirContext;
    // PassManager *passManager;
    string targetTripe;
    FuxPhase phase;

private:
    RootAST *root;
    Generator *generator;
    Compiler *compiler;
};