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
#include <list>

#include <cstdio>
#include <cstring>
#include <cstdint>

// #include "util/list.hpp"
// #include "util/keypair.hpp"
// #include "util/time.hpp"

#define FUX_DEV_DEBUG

using namespace std;

/**
 * @brief compiler options / flags
 * 
 */
struct __options_struct {

    ~__options_struct() { out.clear(); }

    string fileName; // file to compile
    string out              = "a.out"; // output binary file
    string version          = "0.1";
    // vector<string> libraries; // user defined library paths

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

/**
 * @brief Fux metadata for the compiler
 * 
 */
struct __fux_struct {

    /**
     * @brief enumeration of all supported fux version; can be targeted through compiler flags
     * 
     */
    static enum version_enum {

        /*  
            -+- ALPHA -+-
            id: 0
            time: ~2020-present 
            still in development
        */
        ALPHA = 0,
    } version;
     
    __options_struct options;  

    string latest = "Alpha ("+to_string(this->ALPHA)+")";
};

extern __fux_struct fux;

int bootstrap(int argc, char **argv);
int printHelp();
int printVersion();
string toLower(string data);

#define cmp(s) (strcmp(argv[i], s) == 0)