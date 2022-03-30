#include <fstream>
#include <sstream>

#include "include/io.hpp"
#include "include/utility.hpp"

/* 
Copyright (c) 2020-2022 Fuechs
All rights reserved.
*/

using std::string;
using std::ifstream;
using std::stringstream;

static string read_file(const string file_path) {

    ifstream file(file_path);

    if (!file) {
        string error_string = "Can't open file "+file_path;
        error(101, error_string);
        return NULL;
    }

    stringstream content;
    content << file.rdbuf();

    file.close();

    return content.str();

}
