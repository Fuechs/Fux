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

typedef vector<Value *> ValueList;
typedef vector<BasicBlock *> BlockList;

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
    
    Value *readAST(AST *astPtr);

    Value *codegen(AST *astPtr);
    Value *createArith(AST *binaryExpr);
    
    Value *defineGlobal();

    Function *createProto(fuxType::Type type, Function::LinkageTypes linkage, const string name);
    Function *createFunc(Function *prototype);
    Function *createFunc(fuxType::Type type, Function::LinkageTypes linkage, const string name);
};