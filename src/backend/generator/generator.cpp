/**
 * @file generator.cpp
 * @author fuechs
 * @brief fux ir generator
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "generator.hpp"

#ifdef FUX_BACKEND

void Generator::generate() {
    root->codegen(fuxLLVM);

    if (fux.options.debugMode) {
        std::error_code EC;
        llvm::raw_fd_ostream output("src/output.ll", EC);
        fuxLLVM->module->print(output, nullptr);
        output.close();
    }
}

#endif