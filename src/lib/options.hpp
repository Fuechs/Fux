/**
 * @file options.hpp
 * @author fuechs
 * @brief fux options header
 * @version 0.1
 * @date 2022-11-02
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "../include.hpp"
#include "frontend/list.hpp"

struct options {
    ~options() {
        out.clear();
        version.clear();
    }

    bool aggressiveErrors = false;
    bool compile = false;
    string out = "fux.out";
    string version = "0.1";
    bool warnings = true;
    bool optimize = false;
    bool debug = true; // make code debuggable
    bool strip = false;
    bool werrors = false;
    bool debugMode = false; // print information on what the compiler is doing
    bool objDump = false;
    unsigned long errorLimit = 1000;
    int target = versions.ALPHA;
    List<string> libraries; // user defined library paths
};

// obj dump switches
#define OBJ_DEMANGLE 0
#define OBJ_SYNTAX 1
#define OBJ_DEBUG_INFO 2

// warning switches
#define __WGENERAL 0
#define __WACCESS 1
#define __WAMBIG 2
#define __WDECL 3
#define __WMAIN 4
#define __WCAST 5
#define __WINIT 6

extern bool warningMap[];
extern bool objDumpMap[];