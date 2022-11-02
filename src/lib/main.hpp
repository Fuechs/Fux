/**
 * @file main.hpp
 * @author fuechs
 * @brief fux main header
 * @version 0.1
 * @date 2022-11-02
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "options.hpp"

#define progname "fux"
#define progvers "0.1"

int _bootstrap(int argc, const char **argv);
bool startsWith(string &str, string prefix);

extern options cOptions;

#define opt(s) strcmp(argv[i], s) == 0