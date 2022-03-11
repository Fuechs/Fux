#include <iostream>
#include <cstring>

#include "include/std.h"
#include "include/io.h"

using namespace std;

void print_version() {

    cout << "-- Version --" << endl;
    cout << version_x << "." << version_y << "." << version_z << version_c << endl;

}

void print_help() {

    cout << "-- Help --" << endl;
    cout << "Usage: fux [command] [source]" << endl;
    cout << "Commands:" << endl;
    cout << "version            shows the current fux version" << endl;
    cout << "help               shows this help" << endl;
    cout << "compile [source]   compiles and runs the source" << endl << endl;
}

int main(int argc, char** argv) {

    cout << "--- Fux ---" << endl << endl;

    if (argc < 2) {
        cout << "Missing arguments" << endl << endl;
        print_help();
        return 1;
    } 

    if (argc == 2) {

        if (strcmp(argv[1], "version") == 0) print_version();
        else if (strcmp(argv[1], "help") == 0) print_help();
        else if (strcmp(argv[1], "compile") == 0) {
            cout << "Missing source" << endl; 
            return 1;
        }
        else return 1;

        return 0;
    }

    if (strcmp(argv[1], "compile") == 0) {

        const char* source_dir = argv[2];

        string source = read_file(source_dir);

        cout << source << endl;

        return 0;
    }

    cout << "Unknown arguments" << endl << endl;
    print_help();
    return 1;
    

    return 0;

}
