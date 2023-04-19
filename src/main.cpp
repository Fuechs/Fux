/**
 * @file main.cpp
 * @author fuechs
 * @brief entry point
 * @version 0.1
 * @date 2023-04-07
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "fux.hpp"
#include "util/source.hpp"

FuxStruct fux;

#ifndef ERROR_TEST

int parseArguments(int argc, char **argv);
int printVersion();
int printHelp();

int main(int argc, char **argv) {
    int result = 1;

    #if defined(FUX_WIN_INVALID)
        cout << "[ERROR]: Unsupported 'windows' architecture: Fux requires 64-bit.\n";
        return result;
    #elif defined(FUX_UNKNOWN_PLATFORM)
        cout << "[WARNING]: Unknown platform. Compiled version may be broken.\n";
    #endif

    if ((result = parseArguments(argc, argv)) != 0)
        return result;

    if (fux.debug) {
        std::cout << debugText << "This version of Fux was compiled for ";
        #if defined(FUX_WIN)
            std::cout << "Windows";
        #elif defined(FUX_LINUX)
            std::cout << "Linux";
        #elif defined(FUX_UNIX)
            std::cout << "Unix";
        #elif defined(FUX_DARWIN)
            std::cout << "Darwin (MacOS)";
        #endif
        
        #ifndef FUX_BACKEND
            std::cout << " without the backend";
        #endif

        #ifndef FUX_ANALYSER
            #ifndef FUX_BACKEND
                std::cout << " and";
            #endif
            std::cout << " without the analyser";
        #endif

        std::cout << ".\n";
    }

    Source *main = new Source(fux.mainFile, true);

    main->parse();
    main->root->debugPrint();

    return result;
}

// parse compiler flags and source file
int parseArguments(int argc, char **argv) { 
    #define cmp(s) strcmp(argv[i], s) == 0

    if (argc < 2)
        return printHelp();

    for (int i = 1; i < argc; i++) {

        if (cmp("-v"))
            return printVersion();
        
        else if (cmp("-c")) 
            fux.compileOnly = true;

        else if (cmp("-o")) {
            if ((i + 1) >= argc)
                std::cerr << "output file required after option '-o'\n";
            else
                fux.out = std::string(argv[++i]);
        }

        else if (cmp("-a"))
            fux.aggressive = true;
        
        else if (cmp("-O0"))
            fux.optimize = false;

        else if (cmp("-O1"))
            fux.optimize = true;

        else if (cmp("-L")) {
            if ((i + 1) >= argc)
                std::cerr << "path required after option '-L'\n";
            else 
                fux.libraries.push_back(std::string(argv[++i]));
        } 

        else if (cmp("-w"))
            fux.warnings = false;
        
        else if (cmp("-errlimit")) {
            if ((i + 1) >= argc)
                std::cerr << "count required after option '-errlmt'\n";
            else 
                try {
                    fux.errorLimit = (size_t) atoll(argv[++i]);
                } catch (std::exception e) {
                    std::cerr << "invalid error limit count '" << std::string(argv[i]) << "'\n";
                }
        }

        else if (cmp("-werr")) 
            fux.werrors = true;
        
        else if (cmp("-debug") || cmp("-d"))
            fux.debug = true;
        
        else if (cmp("-nothread"))
            fux.threading = false;

        else if (cmp("-help") || cmp("-h"))
            return printHelp();

        else if (argv[i][0] == '-') {
            std::cerr << "Invalid option: '"+std::string(argv[i])+"'\n";
            return printHelp();
        }

        else
            fux.mainFile = argv[i];
    }

    if (fux.mainFile.empty()) {
        std::cerr << "No source file provided\n";
        return printHelp();
    }

    return 0; 

    #undef cmp
}

// print fux version
int printVersion() { 
    std::cout << "Fux v" << fux.version << "\n";
    return 1; 
}

// print help message
int printHelp() { 
    std::cout 
        << "Usage: fux [options] <source file>\n"
        << "options:\n\n"
        << "    -v                  print version and exit\n"
        << "    -c                  compile only\n"
        << "    -o <file>           set output binary file\n"
        << "    -a                  enable aggressive errors\n"
        << "    -O<0|1>             turn optimization off (0) or on (1)\n"
        << "    -L <path>           add library path\n"
        << "    -w                  disable all warnings\n"
        << "    -errlimit <count>   set the error limit\n"
        << "    -werr               treat warnings as errors\n"
        << "    -debug -d           print out debug logs\n"
        << "    -nothread           turn off multithreading\n"
        << "    -help -h            show this message and exit\n";
    return 1; 
}

#else

int main(void) {
    string file = "main.fux";
    StringVec content = {
        "main(void): u64", 
        "   return \"Hello World!\\n\";"};
    Source dummy(file, content);

    fux.aggressive = true;
    Error::Ptr error = make_shared<Error>(Error::UNEXPECTED_TYPE, "This is an error",
        make_shared<Subject>(Metadata(file, 1, 2, 1, content.back().size()), 
            // (Marking::Vec) {Marking(Marking::DASH_UL, "Declaration of `main`", 1, 1, 13, 15), 
            // Marking(Marking::POINTER, "Declared with type `u64`", 1, 0, 13),
            // Marking(Marking::DASH_UL, "Expected an expression of type `u64` here", 2, 11, 4, 26),
            // Marking(Marking::POINTER, "Trying to return a value of type `*c8` here", 2, 0, 4)}
            (Marking::Vec) { make_shared<Underline>(1, 1, 16, 13, "Declaration of `main`") }
    ));
    error->report();
    return 0;
}

#endif