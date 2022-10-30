/**
 * @file runtime.cpp
 * @author fuechs
 * @brief fux runtime
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "runtime.hpp"

int bootstrap(int argc, char **argv) {
    if (argc < 2)
        return help(); // return 1

    return 0;
}

int help() {
    cout 
        << "Usage: bootstrap (options) source file\n"
        << "Source file must have a .fux extension to be compiled.\n"
        << "[-options]\n\n"
        << "    -V                  show version\n"
        << "    -o <file>           set output object file\n"
        << "    -c                  compile only\n"
        << "    -a                  aggresive errors\n"
        << "    -s                  strip debugging info\n"
        << "    -O                  optimize executable\n"
        << "    -w                  disable all warnings\n"
        << "    -v <version>        set application version\n"      
        << "    -unsafe -u          compile unsage code\n"
        << "    -objdump            create dump file for asm\n"
        << "    -target             specify targeted platform\n"
        << "    -werror             treat warnings as errors\n"
        << "    -release -r         generate a release build\n"
        << "    -h                  show this message"
    << endl;

    return 1;
}