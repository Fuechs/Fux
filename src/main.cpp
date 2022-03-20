#include <iostream>
#include <cstring>

#include "include/std.hpp"
#include "include/io.hpp"
#include "include/utility.hpp"

using namespace std;


/* 
Copyright (c) 2020-2022 Fuechs
All rights reserved.
*/

struct _flags {
    bool run;
    bool debug;
    char newline = '\n';
    char print_spaces = ' ';
    string version = (string)version_x + "." + (string)ersion_y + "." + (string)version_z + (string)version_c;
} fux_flags;

bool check(bool debug) {

}

void repl() {

    fux_flags.run = check(fux_flags.debug);

    while (fux_flags.run) {
        
        clear();

        cout << "---[ FUX ]---" << endl << endl;

        cout << "Please input a command: (help for help)" << endl;
        string cmd;
        cin >> cmd;
        cout << endl;

    }

}

void print_help() {

    cout << "--- Help ---" << endl;
    cout << "Usage: fux [command] [source]" << endl;
    cout << "Commands:" << endl;
    cout << "version            shows the current fux version" << endl;
    cout << "help               shows this help" << endl;
    cout << "compile [source]   compiles and runs the source" << endl << endl;
}

int main(int argc, char** argv) {

    if (argc < 2) { // no arguments given
        repl();
        return 0;
    }

    switch (argv[1]) {

        case "version": cout << fux_flags.version << endl; return 0;
        case "help": print_help(); return 0;
        case "compile": break;
        case default: cout << "Unknown argument: " << argv[1] << endl; print_help(); return 1;

    }

    const string source_path = argv[2];
    string source = read_file(source_path);

    return 0;
}
