/**
 * @file io.cpp
 * @author fuechs
 * @brief fux io util
 * @version 0.1
 * @date 2022-11-07
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "io.hpp"

const std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "could not open file '" << path << "'\n";
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

const std::string getDirectory(const std::string path) {
    std::string directory = "";
    const size_t lastSlashIdx = path.rfind('/');
    if (lastSlashIdx != std::string::npos)
        directory = path.substr(0, lastSlashIdx);
    return directory;
}

const std::string getFileName(const std::string &path) {
    return path.substr(path.find_last_of("/") + 1);
}