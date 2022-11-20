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



#include "../../fux.hpp"
#include "../../frontend/parser/ast.hpp"

using namespace llvm;

class Generator {
public:
    Generator(AST *root) { this->root = root; }

    void generate();

private:
    AST *root;
    LLVMContext *context;
    Module *module;
    IRBuilder<> *builder;

    void initializeModule();

    Function *createFunc(IRBuilder<> &builder, string name);
};