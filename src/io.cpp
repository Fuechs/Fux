#include <fstream>
#include <sstream>

#include "include/io.hpp"
#include "include/utility.hpp"

/* 
Copyright (c) 2020-2022 Fuechs
All rights reserved.
*/

std::string read_file(const std::string file_path) {

    std::ifstream file(file_path);
    
    std::stringstream content;
    content << file.rdbuf();

    if (!file) {
        std::string error_string = "Can't open file "+file_path;
        error(101, error_string);
        file.close();
        return NULL;
    }

    file.close();

    return content.str();

}
