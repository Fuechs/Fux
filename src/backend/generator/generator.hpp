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

typedef SmallVector<BasicBlock *, 16> BBList;
typedef SmallVector<Value *, 16> ValList;

class Generator {
public:
    Generator(AST *root) : root(root), context(), module(), builder() {}

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