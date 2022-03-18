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
    string version = version_x + "." + version_y + "." + version_z + version_c;
} fux_flags;

bool check(bool debug) {

}

void repl() {

    fux_flags.run = check(fux_flags.debug);

    while (fux_flags.run) {
        
        clear();

        cout << "---[ FUX ]---" << endl << endl;

        cout << "Please input a command: (help for help)" << endl;
        cin >> string cmd;
        cout << endl;

        switch (cmd) {
            
        }
    }

}

void print_version() {

    cout << "--- Version ---" << endl;
    cout << fux_flags.version << endl;

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

    for (int i = 0; argv[i] != NULL, i++) {
        switch (argv[i]) {
            
        }
    }

    return 1;
}
