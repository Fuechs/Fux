/**
 * @file fux.hpp
 * @author fuechs
 * @brief fux base header
 * @version 0.1
 * @date 2022-11-03
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

using std::cout, std::cerr, std::endl, std::exception, std::make_unique,
        std::map, std::pair, std::reference_wrapper, std::string, std::stringstream, 
        std::to_string, std::unique_ptr, std::vector;

// #define FUX_BACKEND

#ifdef FUX_BACKEND
    // do nothing
    #define FUX_BC(code) code
#else
    // remove redundant (backend) code
    #define FUX_BC(code)
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
struct FuxOptions {

    ~FuxOptions();

    string fileName; // file to compile (main)
    vector<string> fileLines; // lines of that file (main)
    string out              = "a.out"; // output binary file
    string version          = "0.1";
    vector<string> libraries = {
        #if defined(FUX_LINUX)
            "/usr/include/fux/"
        #elif defined(FUX_DARWIN)
            "/usr/local/include/fux/"
        #endif
    }; // library paths

    bool aggressiveErrors   = false;
    bool compileOnly        = false;
    bool warnings           = true;
    bool optimize           = false;
    bool debuggable         = true;
    bool strip              = false;
    bool werrors            = false;
    bool objDump            = false;
    bool unsafe             = false;
    bool threading          = true;
    // debug logs for development
    bool debugMode          = true; // ! change to false later
    
    size_t errorLimit     = 1000;
    string target         = ""; 
    // targeted architecture / platform
    // e.g. arm64-apple-darwin22.2.0
    //      x86_64-unknown-linux-gnu
};

// fux metadata for the compiler
struct FuxStruct {

    // enumeration of all supported fux versions; can be targeted through compiler flags
    enum version {

        // current version in development
        // ~2020-present
        ALPHA = 0,
    };
    const version current = ALPHA;
    const string latest = "Alpha (0)";

    FuxOptions options;  
};

extern FuxStruct fux;

// read flags and data from call arguments
int bootstrap(int argc, char **argv);
// print out the help
int printHelp();
// print out the version
int printVersion();
// convert a string to lowercase
// from https://stackoverflow.com/a/313990
string toLower(string data);
// run repl
int repl();
// clear the console
void clearConsole();

#define cmp(s) (strcmp(argv[i], s) == 0)