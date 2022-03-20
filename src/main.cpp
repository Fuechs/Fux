#include <iostream>
#include <string>

#include "include/std.hpp"
#include "include/io.hpp"
#include "include/utility.hpp"

using namespace std;

/* 
Copyright (c) 2020-2022 Fuechs
All rights reserved.
*/

// init flagss
struct {
    bool run;
    bool debug;
    string version = to_string(fux_version.x)+'.'+to_string(fux_version.y)+'.'+to_string(fux_version.z)+fux_version.c;
} fux_flags;

bool check(bool debug) {
    if (debug) cout << "[DEBUG] No checks to perform" << endl;
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

    fux_flags.run = check(fux_flags.debug);

    while (fux_flags.run) {

        cout << "Please input a command: ('help' for help)" << endl;
        string cmd;
        cin >> cmd;
        cout << endl << endl;

        if (cmd.compare("exit") == 0) return 0;
        else if (cmd.compare("help") == 0) {cout << endl; print_help();}
        else return 1;
    }

    return 1;
}

int main(int argc, char** argv) {

    clear();
    cout << "---[ FUX ]---" << endl << endl;

    if (argc < 2) { // no arguments given
        return repl(); // repl() return integer; 0 = success
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

    return 0;
}
