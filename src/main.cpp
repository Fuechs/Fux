#include <iostream>
#include <string>

#include "include/std.hpp"
#include "include/io.hpp"
#include "include/utility.hpp"

using std::cout;
using std::endl;
using std::string;
using std::to_string;

/* 
Copyright (c) 2020-2022 Fuechs
All rights reserved.
*/

// init flags
struct {
    bool run = false;
    bool debug = false;
    string version = to_string(fux_version.year)+fux_version.sub;
} fux_flags;

static bool check() {
    debug("No checks to perform", fux_flags.debug);
    if (false) error(103, "Checks failed at ...");
    return true;
}

void print_help() {
    marked_log("Help ------");
    cout << "Usage: fux [command] [source]" << endl;
    cout << "Commands:" << endl;
    cout << "version            shows the current fux version" << endl;
    cout << "help               shows this help" << endl;
    cout << "compile [source]   compiles and runs the source" << endl << endl;
}

static int repl() {

    fux_flags.run = check();

    while (fux_flags.run) {

        marked_log("Please input a command. ('help' for help)");
        marked_log("", false);
        string cmd;
        cin >> cmd;
        cout << endl << endl;

        if (cmd.compare("exit") == 0) return 0;
        else if (cmd.compare("help") == 0) print_help();
        else if (cmd.compare("version") == 0) marked_log('v'+fux_flags.version);
        else return error(104, "Unknown command: "+cmd);
    }
    
    debug("Run is false; checks propably failed", fux_flags.debug);
    return 1;
}

static int main(int argc, char** argv) {

    clear();

    if (argc < 2) { // no arguments given
        return repl(); // repl() returns integer; 0 = success
    }

    string arg(argv[1]);

    if (arg.compare("version") == 0) {
        marked_log('v'+fux_flags.version);
        return 0;
    }
    else if (arg.compare("help") == 0) {
        print_help(); 
        return 0;
    }
    else if (arg.compare("debug") == 0) { // no arguments but 'debug' given -> repl with debug
        fux_flags.debug = true;
        return repl(); 
    }
    else if (arg.compare("compile") != 0) { // last argument possible is 'compile' + source
        error(106, "Unknown argument: "+arg);
        print_help();
        return 106;
    }

    if (argv[3]) {
        string arg3(argv[3]);
        if (arg3.compare("debug") == 0) fux_flags.debug = true;
    }

    if (!argv[2]) return error(105, "Source missing");
    
    const string source_path = argv[2];
    string source = read_file(source_path);
    
    if (source.compare("") == 0) return error(102, "Didn't receive source");
    
    debug("Received source; compiling program", fux_flags.debug);
    
    return 0;
}
