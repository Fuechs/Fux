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

    GlobalVariable *globVar = createGlobalI64("global", 16);
    
    Function *mainFunc = createFunc(fuxType::VOID, Function::ExternalLinkage, "main");
    // for (AST *sub : root->body)
    //     Value *V = readAST(sub);
    Constant *constVar = builder->getInt64(1);
    verifyFunction(*mainFunc);
    Value *arith = builder->CreateAdd(constVar, globVar, "addtmp");
    builder->Insert(arith);
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
            Value *arith = createArith(astPtr);
            if (arith)
                // arith->print(errs());
                // cout << endl;
                return arith;
            return nullptr;
        }
        default:
            return nullptr;
    }
}

Value *Generator::codegen(AST *astPtr) {
    if (astPtr->type == AST_BINARY_EXPR)
        return createArith(astPtr);
    return ConstantInt::get(*context, APInt(64, astPtr->value, true));
}

Value *Generator::createArith(AST *binaryExpr) {
    AST *op = (*binaryExpr)[0];
    AST *LHS = (*binaryExpr)[1];
    AST *RHS = (*binaryExpr)[2];

    Value *L = codegen(LHS);
    Value *R = codegen(RHS);
    if (!L || !R)
        return nullptr;
    
    if      (op->value == "+")   return builder->CreateAdd(L, R, "addtmp");
    else if (op->value == "-")   return builder->CreateSub(L, R, "subtmp");
    else if (op->value == "*")   return builder->CreateMul(L, R, "multmp");
    else if (op->value == "/")   return builder->CreateFDiv(L, R, "divtmp");
    else                         return nullptr;
}

GlobalVariable *Generator::createGlobalI64(const string name, const _i64 value, bool constant) {
    GlobalVariable *var = new GlobalVariable(
        *module, builder->getInt64Ty(), constant, 
        GlobalValue::CommonLinkage, builder->getInt64(value), name
    );
    var->setAlignment(MaybeAlign(4));
    return var;
}

// TODO: Arguments
Function *Generator::createProto(fuxType::Type type, Function::LinkageTypes linkage, const string name) {
    FunctionType *FT;
    switch (type) {
        case fuxType::VOID:     
            FT = FunctionType::get(Type::getVoidTy(*context), false);
            break;
        default:
            return nullptr;
    }

    Function *F = Function::Create(FT, linkage, name, module);
    return F;
}

Function *Generator::createFunc(Function *prototype) {
    BasicBlock *BB = BasicBlock::Create(*context, "entry", prototype);
    builder->SetInsertPoint(BB);
    return prototype;
}

Function *Generator::createFunc(fuxType::Type type, Function::LinkageTypes linkage, const string name) {
    return createFunc(createProto(type, linkage, name));
}