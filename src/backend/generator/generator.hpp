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
    Generator(AST *root) { this->root = root; }

    void generate();

private:
    AST *root;
    LLVMContext *context;
    Module *module;
    IRBuilder<> *builder;
    vector<string> funcArgs;

    void initializeModule();

    Function *createFunc(IRBuilder<> &builder, string name);
    BasicBlock *createBB(Function *func, string name);
    void setFuncArgs(Function *func, vector<string> funcArgs);

    GlobalVariable *createGlob(IRBuilder<> &builder, string name);

    Value *createArith(IRBuilder<> &builder, Value *L, Value *R);
    Value *createIfElse(IRBuilder<> &builder, BBList list, ValList VL);
};