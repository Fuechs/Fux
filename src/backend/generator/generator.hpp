/**
 * @file generator.hpp
 * @author fuechs
 * @brief fux ir generator header
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

#include <llvm/ADT/SmallVector.h>

#include "../../fux.hpp"
#include "../../frontend/parser/ast.hpp"

using namespace llvm;

typedef SmallVector<BasicBlock *, 16> BBList;
typedef SmallVector<Value *, 16> ValList;

class Generator {
public:
    Generator(AST *root) : root(root), context(), module(), builder() {}

    ~Generator() { 
        delete root;
        delete builder;
    }

    void generate();
    Module *getModule() { return module; }

private:
    AST *root;
    LLVMContext *context;
    Module *module;
    IRBuilder<> *builder;

    void initializeModule();
    void readAST(AST *astPtr);
    void createArith();
};