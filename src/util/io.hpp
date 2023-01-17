/**
 * @file io.hpp
 * @author fuechs
 * @brief fux io util header
 * @version 0.1
 * @date 2022-11-06
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// reads and returns contents of a file
const string readFile(const string &path);

// splits the directory from a file path
// "some/path/file.txt" --> "some/path/"
const string getDirectory(const string path);

// get file name from a file path
// "some/path/file.txt" --> "file.txt"
const string getFileName(const string &path);