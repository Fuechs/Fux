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

    Function *fooFunc = createFunc(*builder, "foo");
    verifyFunction(*fooFunc);
    module->print(errs(), nullptr);
}

void Generator::initializeModule() {
    this->context = new LLVMContext();
    this->module = new Module("fux compiler", *context);
    this->builder = new IRBuilder<>(*context);
}

Function *Generator::createFunc(IRBuilder<> &builder, string name) {
    FunctionType *funcType = FunctionType::get(builder.getInt32Ty(), false);
    Function *thisFunc = Function::Create(funcType, llvm::Function::ExternalLinkage, name, module);
    return thisFunc;
}