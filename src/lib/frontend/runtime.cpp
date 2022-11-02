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
#include "parser/error.hpp"

int bootstrap(int argc, char **argv) {
    if (argc < 2)
        return help(); // return 1

    string fileName;

    initializeErrors();
    for (int i = 1; i < argc; ++i) {
        args_:
        if (opt("-a"))
            cOptions.aggressiveErrors = true;
        else if (opt("-c"))
            cOptions.compile = true;
        else if (opt("-o"))
            if ((i + 1) >= argc)
                rtError("output file required after option '-o'");
            else
                cOptions.out = string(argv[++i]);
        else if (opt("-V")) {
            printVersion();
            exit(0);
        } else if (opt("-O"))
            cOptions.optimize = true;
        else if (opt("-h") || opt("-?")) {
            help();
            exit(0);
        } else if (opt("-r") || opt("-release")) {
            cOptions.optimize = true;
            cOptions.debug = true;
            cOptions.strip = true;
        } else if (opt("-s"))
            cOptions.strip = true;
        else if (opt("-debug") || opt("-d"))
            cOptions.debugMode = true;
        else if (opt("-trarget")) {
            if ((i + 1) >= argc) 
                rtError("file version required after option '-target'");
            else {
                string x = string(argv[++i]);
                if (allIntegers(x))
                    cOptions.target = strtol(x.c_str(), NULL, 0);
                else
                    if (toLower(x) == "alpha")
                        cOptions.target = versions.ALPHA;
                    else
                        rtError("unknown target '"+x+"'");
            }
        } else if (opt("-w"))
            cOptions.warnings = false;
        else if (opt("-v")) {
            if ((i + 1) >= argc)
                rtError("file version required after option '-v'");
            else
                cOptions.version = string(argv[++i]);
        } else if (opt("-u") || opt("-unsafe"))
            cOptions.unsafe = true;
        else if (opt("-werror") || opt("-werr")) {
            cOptions.werrors = true;
            cOptions.warnings = true;
        } else if (opt("-errlmt")) { 
            string lmt = string(argv[++i]);
            if (allIntegers(lmt)) {
                cOptions.errorLimit = strtoul(lmt.c_str(), NULL, 0);

                if (cOptions.errorLimit > 100_000) 
                    rtError("cannot set the max errors allowed higher than 100,000");
                else if (cOptions.errorLimit == 0)
                    rtError("cannot have an error limit of 0");
            } else 
                rtError("invalid error limit set '"+lmt+"'");
        } else if (opt("-objdump")) 
            cOptions.objDump = true;
        else if (argv[i][0] == '-')
            rtError("invalid option '"+string(argv[i])+"', try 'fux -h'");
        else
            fileName = argv[i];
    }

    if (fileName.empty())
        return help();

    return 0;
}

int help() {
    cout 
        << "Usage: bootstrap (options) source file\n"
        << "Source file must have a .fux extension to be compiled.\n"
        << "[-options]\n\n"
        << "    -V                  print version and exit\n"
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
        << "    -werror -werr       treat warnings as errors\n"
        << "    -release -r         generate a release build\n"
        << "    -debug -d           turn debug mode on\n"
        << "    -h -?               show this message and exit"
    << endl;

    return 1;
}