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
 * Manages...
 * ...Generator
 * ...Compiler
 * ...LLVMContext
 * ...MLIRContext
 * 
 */
class FuxContext {
public:
    LLVMContext *llvmContext;
    mlir::MLIRContext *mlirContext;
    
    FuxContext(RootAST *root);
    ~FuxContext();

    mlir::PassManager *passManager;
    string target;
    FuxPhase phase;

private:
    RootAST *root;
    Generator *generator;
    Compiler *compiler;
};