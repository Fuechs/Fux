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

#include <cstdint>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

#include <llvm/ADT/SmallVector.h>

using namespace llvm;

typedef vector<Value *> ValueList;
typedef vector<BasicBlock *> BlockList;

// type definitions for generating objects
typedef int8_t      _i8;
typedef uint8_t     _u8;
typedef char        _c8;
typedef int16_t     _i16;
typedef uint16_t    _u16;
typedef wchar_t     _c16;
typedef int32_t     _i32;
typedef uint32_t    _u32;
typedef float       _f32;
typedef int64_t     _i64;
typedef uint64_t    _u64;
typedef double      _f64;