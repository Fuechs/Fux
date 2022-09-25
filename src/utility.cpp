#include "include/utility.hpp"

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

void debug(std::string message, bool debug) {
    if (debug) {
        std::cout << "[DEBUG] ";
        std::cout << message << std::endl;
    }
}

void marked_log(std::string message, bool newline) {
    std::cout << "[FUX] ";
    std::cout << message;
    if (newline) std::cout << std::endl;
}
