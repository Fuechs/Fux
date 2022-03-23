#include <iostream>
#include <string>

#include "include/std.hpp"
#include "include/io.hpp"
#include "include/utility.hpp"
#include "include/lexer.hpp"
#include "include/parser.hpp"

using namespace std;

/* 
Copyright (c) 2020-2022 Fuechs
All rights reserved.
*/

// init flags
struct {
    bool run = false;
    bool debug = false;
    string version = to_string(fux_version.x)+'.'+to_string(fux_version.y)+'.'+to_string(fux_version.z)+fux_version.c;
} fux_flags;

bool check() {
    debug("No checks to perform", fux_flags.debug);
    if (false) error(103, "Checks failed at ...");
    return true;
}

void print_help() {

    cout << "--- Help ---" << endl;
    cout << "Usage: fux [command] [source]" << endl;
    cout << "Commands:" << endl;
    cout << "version            shows the current fux version" << endl;
    cout << "help               shows this help" << endl;
    cout << "compile [source]   compiles and runs the source" << endl << endl;
}

int repl() {

    fux_flags.run = check();

    while (fux_flags.run) {

        cout << "Please input a command: ('help' for help)" << endl;
        string cmd;
        cin >> cmd;
        cout << endl << endl;

        if (cmd.compare("exit") == 0) return 0;
        else if (cmd.compare("help") == 0) print_help();
        else if (cmd.compare("version") == 0) {
            cout << 'v' << fux_flags.version << endl << endl;
        }
        else return error(104, "Unknown command: "+cmd);
    }
    
    debug("Run is false; checks propably failed", fux_flags.debug);
    return 1;
}

int main(int argc, char** argv) {

    clear();
    cout << "---[ FUX ]---" << endl << endl;

    if (argc < 2) { // no arguments given
        return repl(); // repl() returns integer; 0 = success
    }

    string arg(argv[1]);

    if (arg.compare("version") == 0) {
        cout << 'v' << fux_flags.version << endl; 
        return 0;
    }
    else if (arg.compare("help") == 0) {
        print_help(); 
        return 0;
    }
    else if (arg.compare("compile") != 0) { 
        cout << "Unknown argument: " << arg << endl;
        print_help(); 
        return 1;
    }

    if (argv[3]) {
        string arg3(argv[3]);
        if (arg3.compare("debug") == 0) fux_flags.debug = true;
    }

    if (!argv[2]) {cout << "Source missing" << endl; return 1;}
    
    const string source_path = argv[2];
    string source = read_file(source_path);
    
    if (source.compare("") == 0) return error(102, "Didn't receive source");
    
    debug("Received source; compiling program", fux_flags.debug);
    
    typedef lexer_type lex_t; 
    typedef parser_type parse_t;
    
    // lexer
    lex_t lexed = lexer.init(source);
    // parser
    parse_t parsed = parser.init(lexed);
    // compiler
    
    
    return 0;
}
