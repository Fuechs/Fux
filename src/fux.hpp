/**
 * @file fux.hpp
 * @author fuechs
 * @brief 
 * @version 0.1
 * @date 2022-11-03
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <memory>

// #include <list>
// #include <regex>

#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdlib>

#include "util/color.hpp"
#include "util/io.hpp"

// #include "util/keypair.hpp"
// #include "util/list.hpp"
// #include "util/time.hpp"

#define FUX_DEV_DEBUG

using namespace std;

// compiler options / flags
struct __options_struct {

    ~__options_struct() { out.clear(); }

    string fileName; // file to compile (main)
    vector<string> fileLines; // lines of that file (main)
    string out              = "a.out"; // output binary file
    string version          = "0.1";
    vector<string> libraries = {"/usr/local/include/fux/"}; // library paths

    bool aggressiveErrors   = false;
    bool compileOnly        = false;
    bool warnings           = true;
    bool optimize           = false;
    bool debuggable         = true;
    bool strip              = false;
    bool werrors            = false;
    bool userDebug          = false;
    bool objDump            = false;
    bool unsafe             = false;

    // debug logs for development
    #ifdef FUX_DEV_DEBUG
    bool debugMode          = true;
    #else
    bool debugMode          = false; 
    #endif
    
    uint64_t errorLimit     = 1000;
    int target              = 0; // targeted fux version 
};

// fux metadata for the compiler
struct __fux_struct {

    // enumeration of all supported fux versions; can be targeted through compiler flags
    enum version {

        // current version in development
        // ~2020-present
        ALPHA = 0,
    };
     
    __options_struct options;  

    string latest = "Alpha ("+to_string(this->ALPHA)+")";
};

extern __fux_struct fux;

int bootstrap(int argc, char **argv);
int printHelp();
int printVersion();
string toLower(string data);
int repl();

#define cmp(s) (strcmp(argv[i], s) == 0)