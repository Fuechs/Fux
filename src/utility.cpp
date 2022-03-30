#include <iostream>

#include "include/utility.hpp"

using std::cout;
using std::endl;
using std::string;

/* 
Copyright (c) 2020-2022 Fuechs
All rights reserved.
*/

void clear() {
    #if defined _WIN32
        system("cls");
    #elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__) || defined(__APPLE__)
        system("clear");
    #endif
}

int error(int id, string message) {
    // id
    cout << '[' << id << "] ";
    cout << message << endl;
    return id;
} 

void debug(string message, bool debug) {
    if (debug) {
        cout << "[DEBUG] ";
        cout << message << endl;
    }
}

void marked_log(string message, bool newline) {
    cout << "[FUX] ";
    cout << message;
    if (newline) cout << endl;
}
