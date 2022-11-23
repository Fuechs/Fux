/**
 * @file generator.cpp
 * @author fuechs
 * @brief fux ir generator
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "generator.hpp"

void Generator::generate() {
    initializeModule();
    readAST(root);
    if (fux.options.debugMode)
        module->print(errs(), nullptr);
    delete this;
}

void Generator::initializeModule() {
    context = new LLVMContext();
    module = new Module("fux compiler", *context);
    builder = new IRBuilder<>(*context);
}

void Generator::readAST(AST *astPtr) {
    switch (astPtr->type) {
        case AST_ROOT:
            for (AST *sub : astPtr->body)
                readAST(sub);
            break;
        case AST_BINARY_EXPR:
            break;
    }
}