#include <iostream>

#include "include/utility.hpp"

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

int error(int id, std::string message) {
    // id
    std::cout << '[' << id << "] ";
    std::cout << message << std::endl;
    return id;
} 