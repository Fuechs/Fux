/**
 * @file llvmheader.hpp
 * @author fuechs
 * @brief llvm header include file
 * @version 0.1
 * @date 2022-11-27
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

#include <llvm/ADT/SmallVector.h>

using namespace llvm;