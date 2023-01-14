/**
 * @file llvmheader.hpp
 * @author fuechs
 * @brief llvm header include file
 * @version 0.1
 * @date 2022-11-27
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#pragma once

#include "../fux.hpp"

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include <llvm/MC/TargetRegistry.h>

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

using namespace llvm;

typedef vector<Value *> ValueList;
typedef vector<BasicBlock *> BlockList;
typedef vector<Type *> TypeList;
typedef map<string, Value *> ValueMap;

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