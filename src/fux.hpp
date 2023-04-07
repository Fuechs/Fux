/**
 * @file fux.hpp
 * @author fuechs
 * @brief fux base header
 * @version 0.1
 * @date 2023-04-07
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include <cassert>
#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "util/color.hpp"
#include "util/io.hpp"

using StringVec = std::vector<std::string>;

// #define FUX_BACKEND

#ifdef FUX_BACKEND
    // do nothing
    #define FUX_BC(code) code
#else
    // remove redundant (backend) code
    #define FUX_BC(code)
#endif

// #define FUX_ANALYSER

#ifdef FUX_ANALYSER
    // do nothing
    #define FUX_AC(code) code
#else
    // remove redundant (analyser) code
    #define FUX_AC(code)
#endif

#if defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && defined(_WIN32))
    #ifndef _WIN64
        #define FUX_WIN_INVALID
    #else
        #define FUX_WIN
        #include <windows.h>
    #endif
#elif defined(__linux__) 
    #define FUX_LINUX 
#elif defined(__unix__) || !defined(__APPLE__) && defined(__MACH__)
    #define FUX_UNIX
#elif defined(__APPLE__) && defined(__MACH__) 
    #define FUX_DARWIN
#else
    #define FUX_UNKNOWN_PLATFORM
#endif

// compiler options / flags
struct FuxStruct {
    std::string mainFile = "";              // main file containing main() function
    std::string version = "0.1-dev";        // targeted version (standard is latest)
    std::string out = "a.out";              // name of output binary file (standard is 'a.out')
    StringVec libraries = {                 // library paths
        #if defined(FUX_LINUX)
            "/usr/include/fux/"
        #elif defined(FUX_DARWIN)
            "/usr/local/include/fux/"
        #endif
    }; 

    bool aggressive     = false; // aggressive errors
    bool compileOnly    = false; // compile only to llvm ir
    bool warnings       = true;  // throw warnings
    bool optimize       = true;  // optimize generated code
    bool werrors        = true;  // treat warnings as errors
    bool threading      = true;  // use threading 
    bool debug          = true;  // debug logs 
    size_t errorLimit   = 1000;  // amount of errors until stop
    std::string target  = "";    // targeted architecture / platform
                                 // e.g. arm64-apple-darwin22.2.0
                                 //      x86_64-unknown-linux-gnu
};

extern FuxStruct fux;