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

const string readFile(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "could not open file '" << path << "'\n";
        exit(1);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

const string getDirectory(const string path) {
    string directory = "";
    const size_t lastSlashIdx = path.rfind('/');
    if (lastSlashIdx != string::npos)
        directory = path.substr(0, lastSlashIdx);
    return directory;
}

const string getFileName(const string &path) {
    return path.substr(path.find_last_of("/") + 1);
}