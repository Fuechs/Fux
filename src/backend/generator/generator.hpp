/**
 * @file generator.hpp
 * @author fuechs
 * @brief fux ir generator header
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../llvmheader.hpp"
#include "../../frontend/ast/ast.hpp"
#include "../context/fuxmem.hpp"
#include "../context/fuxstr.hpp"
#include "genarray.hpp"

class Generator {
public:
    Generator(ExprPtr &root, LLVMWrapper *fuxLLVM) 
    : root(std::move(root)), fuxLLVM(fuxLLVM) {}

    void generate();

    static Type *getType(LLVMWrapper *fuxLLVM, FuxType type);
    static FuxArray *createArrayType(LLVMWrapper *fuxLLVM, FuxType type);

private:
    ExprPtr root;
    LLVMWrapper *fuxLLVM;

    void debugPrint(const string message);
};