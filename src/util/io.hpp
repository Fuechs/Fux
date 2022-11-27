/**
 * @file io.hpp
 * @author fuechs
 * @brief fux io util header
 * @version 0.1
 * @date 2022-11-06
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

const string readFile(const string& path);
const string getDirectory(const string path);