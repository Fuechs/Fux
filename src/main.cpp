/**
 * @file main.cpp
 * @author fuechs
 * @brief main file of fux
 * @version 0.1
 * @date 2022-09-26
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "fux.hpp"
#include "util/source.hpp"

#ifdef FUX_BACKEND
#include "util/threading.hpp"
#include "backend/context/context.hpp"

RootAST::Ptr createTestAST();
#endif

FuxStruct fux;

int main(int argc, char **argv) {
    int result = 0;

    #if defined(FUX_WIN_INVALID)
        cout << "[ERROR]: Unsupported 'windows' architecture: Fux requires 64-bit.\n";
        return result;
    #elif defined(FUX_UNKNOWN_PLATFORM)
        cout << "[WARNING]: Unknown platform. Compiled version may be broken.\n";
    #endif

    if (fux.options.debugMode) {
        cout << debugText << "This version of Fux was compiled for ";
        #if defined(FUX_WIN)
            cout << "Windows";
        #elif defined(FUX_LINUX)
            cout << "Linux";
        #elif defined(FUX_UNIX)
            cout << "Unix";
        #elif defined(FUX_DARWIN)
            cout << "Darwin (MacOS)";
        #endif
        #ifndef FUX_BACKEND
            cout << " without the backend";
        #endif
        cout << ".\n";
    }

    result = bootstrap(argc, argv);
    switch (result) {
        case -1:    return repl();
        case 0:     break;
        default:    return result;
    }
    
    SourceFile *mainFile = new SourceFile(fux.options.fileName, true);
    fux.options.libraries.push_back(mainFile->fileDir); // add src include path 

    mainFile->parse();
    RootAST::Ptr root = std::move(mainFile->root);
    if (mainFile->hasErrors()) {
        delete mainFile;
        return 1;
    } 
    mainFile->reportErrors();
    // root->debugPrint();
    mainFile->analysed->debugPrint();

    // RootAST::Ptr root = createTestAST();    
    // root->debugPrint();

    #ifdef FUX_BACKEND 
    FuxContext *context = new FuxContext(root);
    context->run();
    #endif

    return result;
}

int bootstrap(int argc, char **argv) {
    if (argc < 2)   
        return printHelp();
    
    for (int i = 1; i < argc; ++i) {

        if      (cmp("-V"))     return printVersion();
        else if (cmp("-o")) {
            if ((i + 1) >= argc)
                cerr << "output file required after option '-o'\n";
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
                fux.options.target = toLower(x);
            }
        }
        else if (cmp("-werror") || cmp("-werr"))    fux.options.werrors = true;
        else if (cmp("-release") || cmp("-r")) {
            fux.options.optimize = true;
            fux.options.debuggable = true;
            fux.options.strip = true;
        }
        else if (cmp("-debug") || cmp("-d"))    fux.options.debugMode = true;
        else if (cmp("-nothread"))              fux.options.threading = false;
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
        << "    -nothread           deactivate multihreading for parsing\n"
        << "    -repl               start repl\n"
        << "    -h -help            show this message and exit"
        << endl;

    return 1;
}

int printVersion() {
    cout << "Fux Version '" << fux.latest << "'\n";
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
        // getline(cin, input);
        input = "1 - - 1";

        if (input.empty()) 
            continue;
        else if (input == "exit" || input == "exit;")
            break;

        ErrorManager *error = new ErrorManager("<stdin>", {input});
        Parser *parser = new Parser(error, "<stdin>", input, true);
        RootAST::Ptr root = parser->parse();
        // Analyser *analyser = new Analyser(error, root);
        // StmtAST::Ptr analysed = analyser->analyse();
        delete parser;
        // delete analyser;

        if (error->hasErrors())
            error->panic();
        else {
            root->debugPrint();
            // TODO: generate and run ...
        }

        break;
    }

    return result;
}

#ifdef FUX_BACKEND
// create AST to test the generator without parser
RootAST::Ptr createTestAST() {
    RootAST::Ptr root = make_unique<RootAST>();

    // modify(num -> i64): void;
    StmtAST::Vec eArgs = StmtAST::Vec();
    eArgs.push_back(make_unique<VariableDeclAST>("num", FuxType::createRef(FuxType::I64)));
    StmtAST::Ptr emptyF = make_unique<PrototypeAST>(FuxType(FuxType::VOID), "modify", eArgs);
    root->addSub(emptyF);

    // main(argc: i64, argv: str[]): i64 {
    //      x: i64 = argc + 1;
    //      modify(x);
    //      return x;
    // }
    StmtAST::Vec args = StmtAST::Vec();
    args.push_back(make_unique<VariableDeclAST>("argc", FuxType::createStd(FuxType::U64, 0, {FuxType::FINAL})));
    args.push_back(make_unique<VariableDeclAST>("argv", FuxType::createArray(FuxType::LIT, 0, {FuxType::FINAL})));

    StmtAST::Vec bodyList = StmtAST::Vec();
    
    ExprAST::Ptr variable = make_unique<VariableExprAST>("argc");
    ExprAST::Ptr constant = make_unique<NumberExprAST, _i64>(1);
    ExprAST::Ptr binop = make_unique<BinaryExprAST>(BinaryOp::ADD, variable, constant);
    StmtAST::Ptr decl = make_unique<VariableDeclAST>("x", FuxType(FuxType::I64), binop);
    bodyList.push_back(std::move(decl));

    variable = make_unique<VariableExprAST>("x");
    ExprAST::Vec pass = ExprAST::Vec();
    pass.push_back(std::move(variable));
    StmtAST::Ptr call = make_unique<CallExprAST>("modify", pass);
    bodyList.push_back(std::move(call));

    variable = make_unique<VariableExprAST>("x");
    ExprAST::Vec retArgs = ExprAST::Vec();
    retArgs.push_back(std::move(variable));
    StmtAST::Ptr ret = make_unique<InbuiltCallAST>(Inbuilts::RETURN, retArgs);
    bodyList.push_back(std::move(ret));

    StmtAST::Ptr body = make_unique<CodeBlockAST>(bodyList);
    StmtAST::Ptr mFunc = make_unique<FunctionAST>(FuxType(FuxType::I64), "main", args, body);
    root->addSub(mFunc);

    return root;
}
#endif

void clearConsole() {
    #ifdef FUX_WIN
        system("cls");
    #else // unix, ...
        system("clear");
    #endif
}

// FuxOptions ; FuxStruct

FuxOptions::~FuxOptions() { 
    fileName.clear();
    fileLines.clear();
    out.clear(); 
    version.clear();
    libraries.clear();
    target.clear();
}