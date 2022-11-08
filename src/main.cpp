/**
 * @file main.cpp
 * @author fuechs
 * @brief main file of fux
 * @version 0.1
 * @date 2022-09-26
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "fux.hpp"
#include "frontend/error/error.hpp"
#include "frontend/parser/parser.hpp"
#include "frontend/parser/ast.hpp"

__fux_struct fux;

int main(int argc, char **argv) {
    
    int result = 0;

    // int result = bootstrap(argc, argv);
    // if (result != 0) 
    //     return result;
    
    // setting file manually for debugging reasons
    fux.options.fileName = "src/examples/test.fux"; 

    fux.options.libraries.push_back(getDirectory(fux.options.fileName)); // add src include path 

    // just repl for testing
    return repl();

    ErrorManager *error = new ErrorManager(fux.options.fileName, vector<string>());
    
    {   // own scope so it can be skipped by goto
        Parser *parser = new Parser(error, fux.options.fileName, readFile(fux.options.fileName), true);
        auto root = parser->parse();

        root->debugPrint();
        
        delete parser; 

        if (error->hasErrors())
            goto end;
    }

    end: 
        error->panic();

        return result;
}

int bootstrap(int argc, char **argv) {
    if (argc < 2)   
        return printHelp();
    
    for (int i = 1; i < argc; ++i) {

        if      (cmp("-V"))     return printVersion();
        else if (cmp("-o")) {
            if ((i + 1) >= argc)
                cerr << "output file required after option '-o'\n"; // TODO: add errors
            else
                fux.options.out = string(argv[++i]);
        }
        else if (cmp("-c"))     fux.options.compileOnly = true;
        else if (cmp("-a"))     fux.options.aggressiveErrors = true;
        else if (cmp("-s"))     fux.options.strip = true;
        else if (cmp("-O"))     fux.options.optimize = true;
        else if (cmp("-L")) {
            if ((i + 1) >= argc)
                cerr << "path required after option '-L'\n";
            else 
                fux.options.libraries.push_back(string(argv[++i]));
        } 
        else if (cmp("-w"))     fux.options.warnings = true;
        else if (cmp("-errlmt")) {
            if ((i + 1) >= argc)
                cerr << "count required after option '-errlmt'\n";
            else {
                try {
                    fux.options.errorLimit = (uint64_t) atoll(argv[++i]);
                } catch (exception e) {
                    cerr << "invalid error limit count '" << string(argv[i]) << "'\n";
                }
            }
        }
        else if (cmp("-v")) {
            if ((i + 1) >= argc)
                cerr << "file version required after option '-v'\n";
            else
                fux.options.version = string(argv[++i]);
        }
        else if (cmp("-unsafe") || cmp("-u"))   fux.options.unsafe = true;
        else if (cmp("-objdump"))   fux.options.objDump = true;
        else if (cmp("-target")) {
            if ((i + 1) >= argc) 
                cerr << "file version required after option '-target'\n";
            else {
                string x = string(argv[++i]);
                if (toLower(x) == "alpha")
                    fux.options.target = fux.ALPHA;
                else
                    cerr << "unknown target '"+x+"'\n";
            }
        }
        else if (cmp("-werror") || cmp("-werr"))    fux.options.werrors = true;
        else if (cmp("-release") || cmp("-r")) {
            fux.options.optimize = true;
            fux.options.debuggable = true;
            fux.options.strip = true;
        }
        else if (cmp("-debug") || cmp("-d"))    fux.options.userDebug = true;
        else if (cmp("-h") || cmp("-help"))        return printHelp();

        else if (argv[i][0] == '-')             cerr << "invalid option '"+string(argv[i])+"'\n";
        else                                    fux.options.fileName = argv[i];
    }

    if (fux.options.fileName.empty()) {   
        cerr << "source file missing\n";
        return printHelp();
    }

    return 0;
}

int printHelp() {
    cout 
        << "Usage: fux [options] <source file>\n"
        << "[options]\n\n"
        << "    -V                  print version and exit\n"
        << "    -o <file>           set output object file name\n"
        << "    -c                  compile only\n"
        << "    -a                  aggressive errors\n"
        << "    -s                  strip debugging info\n"
        << "    -O                  optimize executable\n"
        << "    -L <path>           add library path\n"
        << "    -w                  disable all warnings\n"
        << "    -errlmt <count>     set an error limit for the compiler\n"
        << "    -v <version>        set application version\n"      
        << "    -unsafe -u          compile unsafe code\n"
        << "    -objdump            create dump object dump file\n"
        << "    -target <target>    specify targeted platform\n"
        << "    -werror -werr       treat warnings as errors\n"
        << "    -release -r         generate a release build\n"
        << "    -debug -d           turn debug mode on\n"
        << "    -h -help            show this message and exit"
    << endl;

    return 1;
}

int printVersion() {
    cout
        << "Fux Version '" << fux.latest << "'\n";
    return 1;
}

// from https://stackoverflow.com/a/313990
string toLower(string data) {
    transform(data.begin(), data.end(), data.begin(), [](unsigned char c){ return std::tolower(c); });
    return data;
}

int repl() { 
    string input = "";
    for (;;) {
        cout << "> ";
        getline(cin, input);
        if (input.empty()) 
            continue;
        else if (input == "exit" || input == "exit;")
            break;
        ErrorManager *error = new ErrorManager("<stdin>", {});
        Parser *parser = new Parser(error, "<stdin>", input, true);
        AST *root = parser->parse();
        delete parser;
        root->debugPrint();
        delete root;
        error->panic();
    }
    return 1;
}