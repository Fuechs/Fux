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
    Generator(RootAST *root) : root(root), context(), module(), builder() {}

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

    LLVMContext *getContext() { return context; }
    Module *getModule() { return module; }

private:
    RootAST *root;
    LLVMContext *context;
    Module *module;
    IRBuilder<> *builder;

    ValueMap namedValues;

    // initialize context, module & builder
    void initializeModule();
    void debugPrint(const string message);
};