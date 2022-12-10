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

typedef signed long long genI64;

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

    // initialize context, module & builder
    void initializeModule();

    // read the AST and do different things depending on type
    Value *readAST(AST *astPtr);

    // put ASTs on the right path for code generation
    Value *codegen(AST *astPtr);
    // create arithmetic operation from binaryExprAST
    Value *createArith(AST *binaryExpr);

    // create global variables
    GlobalVariable *createGlobalI64(const string name, const genI64 value = 0);

    // create a function prototype
    Function *createProto(fuxType::Type type, Function::LinkageTypes linkage, const string name);
    // create a function from a prototype
    Function *createFunc(Function *prototype);
    // create a function
    Function *createFunc(fuxType::Type type, Function::LinkageTypes linkage, const string name);
};