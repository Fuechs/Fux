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
#include <vector>
#include <iterator>

// reads and returns contents of a file
const std::string readFile(const std::string &path);

// splits the directory from a file path
// "some/path/file.txt" --> "some/path/"
const std::string getDirectory(const std::string path);

// get file name from a file path
// "some/path/file.txt" --> "file.txt"
const std::string getFileName(const std::string &path);

// escape all escape sequences in string
std::string escapeSequences(std::string);

// unescape all escape sequences in string
std::string unescapeSequences(std::string);

// from https://stackoverflow.com/a/236803
template <typename Out>
void split(const std::string &s, char delim, Out result);

std::vector<std::string> split(const std::string &s, char delim);

std::string highlightedInsert(const std::string &dest, size_t pos, 
    std::string insert, size_t pos1 = 0, std::string insert1 = "");