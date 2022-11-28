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
    
    FunctionType *FT = FunctionType::get(Type::getInt32Ty(*context), false);
    Function *F = Function::Create(FT, Function::ExternalLinkage, "main", module);
    BasicBlock *BB = BasicBlock::Create(*context, "entry", F);
    builder->SetInsertPoint(BB);
    
    Value *retVal = readAST(root);
    
    builder->CreateRet(retVal);
    verifyFunction(*F);

    if (fux.options.debugMode)
        module->print(errs(), nullptr);
    
    delete this;
}

void Generator::initializeModule() {
    context = new LLVMContext();
    module = new Module("fux compiler", *context);
    builder = new IRBuilder<>(*context);
}

Value *Generator::readAST(AST *astPtr) {
    switch (astPtr->type) {
        case AST_ROOT:
            return readAST((*astPtr)[0]);
        case AST_BINARY_EXPR: {
            Value *arith = createArith((*astPtr)[0], (*astPtr)[1], (*astPtr)[2]);
            if (arith)
                arith->print(errs());
                cout << endl;
                return arith;
            return nullptr;
        }
        default:
            return nullptr;
    }
}

Value *Generator::codegen(AST *astPtr) {
    return ConstantFP::get(*context, APFloat(stod(astPtr->value)));
}

Value *Generator::createArith(AST *op, AST *LHS, AST *RHS) {
    Value *L = codegen(LHS);
    Value *R = codegen(RHS);
    if (!L || !R)
        return nullptr;
    
    if      (op->value == "+")   return builder->CreateFAdd(L, R, "addtmp");
    else if (op->value == "-")   return builder->CreateFSub(L, R, "subtmp");
    else if (op->value == "*")   return builder->CreateFMul(L, R, "multmp");
    else if (op->value == "/")   return builder->CreateFDiv(L, R, "divtmp");
    else                         return nullptr;
}