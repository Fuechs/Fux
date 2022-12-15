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

#include "util/source.hpp"

#include "backend/generator/generator.hpp"

__fux_struct fux;

int main(int argc, char **argv) {
    int result = 0;

    // Generator *generator = new Generator(new AST(nullptr, AST_ROOT));
    // generator->generate();
    // Module *LLVMModule = generator->getModule();
    // delete generator;
    
    // return result;

    // result = bootstrap(argc, argv);
    // switch (result) {
    //     case -1:    return repl();
    //     case 0:     break;
    //     default:    return result;
    // }

    fux.options.fileName = "/Users/fuechs/Documents/GitHub/Fux/src/examples/test.fux"; // debugger

    SourceFile *mainFile = new SourceFile(fux.options.fileName, true);

    
    fux.options.libraries.push_back(mainFile->fileDir); // add src include path 

    // RootAST *root = mainFile->parse();
    // if (mainFile->error->hasErrors())
    //     goto end;

    RootAST *root = new RootAST();
    
    
    
    ArgMap eArgs;
    eArgs["a"] = fuxType::I32;
    // ExprList eBody;
    // ExprPtr eArg1 = make_unique<VariableExprAST>("a");
    // ExprPtr eArg2 = make_unique<NumberExprAST>(1.0);
    // ExprPtr binOp2 = make_unique<BinaryExprAST>('*', eArg1, eArg2);
    // eBody.push_back(binOp2);
    ExprPtr emptyF = make_unique<PrototypeAST>(fuxType::I32, "mod", eArgs);
    root->addSub(emptyF);

    ExprPtr arg1 = make_unique<VariableExprAST>("x");
    ExprPtr arg2 = make_unique<VariableExprAST>("y");
    ExprPtr binOp = make_unique<BinaryExprAST>('+', arg1, arg2);

    ExprList callArgs;
    ExprPtr num = make_unique<VariableExprAST>("addtmp");
    callArgs.push_back(move(num));
    ExprPtr eCall = make_unique<CallExprAST>("mod", callArgs);

    ExprList mBody;
    mBody.push_back(move(binOp));
    mBody.push_back(move(eCall));

    ArgMap args;
    args["x"] = fuxType::I32;
    args["y"] = fuxType::I32;
    
    ExprPtr mFunc = make_unique<FunctionAST>(fuxType::I32, "main", args, mBody);
    root->addSub(mFunc);

    { // own scope so it can be skipped by goto -- c++ calls desctructer at end of scope
        Generator *generator = new Generator(root);
        generator->generate();
        if (false) { // TODO: check for errors
            generator->forceDelete();
            goto end;
        }
    }

    end:
        // delete mainFile;
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
        else if (cmp("-repl"))                  return -1;
        else if (cmp("-h") || cmp("-help"))     return printHelp();

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
        << "    -repl               start repl\n"
        << "    -h -help            show this message and exit"
        << endl;

    return 1;
}

int printVersion() {
    cout
        << "Fux Version '" << fux.latest << "'\n";
    return 1;
}

string toLower(string data) {
    transform(data.begin(), data.end(), data.begin(), [](unsigned char c){ return std::tolower(c); });
    return data;
}

int repl() { 
    int result = 0;
    string input = "";
    for (;;) {
        cout << "> ";
        getline(cin, input);

        if (input.empty()) 
            continue;
        else if (input == "exit" || input == "exit;")
            break;

        SourceFile *that = new SourceFile("<stdin>", true);
        RootAST *root = that->parse();
        // TODO: generate and run ...
    }

    return result;
}