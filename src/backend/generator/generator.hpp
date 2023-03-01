/**
 * @file generator.hpp
 * @author fuechs
 * @brief fux ir generator header
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"

#ifdef FUX_BACKEND
#include "../llvmheader.hpp"
#include "../../frontend/ast/ast.hpp"

class Generator {
public:
    Generator(RootAST::Ptr &root, LLVMWrapper *fuxLLVM) 
    : root(std::move(root)), fuxLLVM(fuxLLVM) {}

    void generate();

    static Type *getType(LLVMWrapper *fuxLLVM, FuxType type);

private:
    RootAST::Ptr root;
    LLVMWrapper *fuxLLVM;

    void debugPrint(const string message);
};

#endif