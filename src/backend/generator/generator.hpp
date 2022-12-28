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
#include "../llvmheader.hpp"
#include "../../frontend/parser/ast.hpp"

typedef map<string, Value *> ValueMap;

class Generator {
public:
    Generator(RootAST *root, Module *module) 
    : root(root), context(&module->getContext()), 
    module(module), builder(new IRBuilder<>(*context)) {}

    ~Generator() { 
        delete root;
        delete builder;
    }

    void forceDelete() {
        delete builder;
        delete module;
        delete context;
        delete this;
    }

    void generate();

private:
    RootAST *root;
    LLVMContext *context;
    Module *module;
    IRBuilder<> *builder;

    ValueMap namedValues;

    // initialize context, module & builder
    void initializeModule();
    void debugPrint(const string message);

    Type *getType(fuxType::Type type, const bool pointer = false);
};