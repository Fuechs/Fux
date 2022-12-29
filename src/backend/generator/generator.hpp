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
#include "fuxstr.hpp"

class Generator {
public:
    Generator(RootAST *root, LLVMWrapper *fuxLLVM) 
    : root(root), fuxLLVM(fuxLLVM), fuxStr(FuxStr(fuxLLVM)) {
        fuxLLVM->builder = new IRBuilder<>(*fuxLLVM->context);
    }

    ~Generator() { delete root; }

    void generate();

private:
    RootAST *root;
    LLVMWrapper *fuxLLVM;
    FuxStr fuxStr;

    void debugPrint(const string message);

    Type *getType(fuxType::Type type, const bool pointer = false);
};