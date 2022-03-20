#include <fstream>

#include "include/io.hpp"

/* 
Copyright (c) 2020-2022 Fuechs
All rights reserved.
*/

std::string read_file(const std::string file_path) {

    std::string content;

    std::ifstream file;

    file.open(file_path, 'r');

    if (!file) {
        std::cout << "Can't open '" << file_path << "'" << std::endl;
        return NULL;
    }

    std::cout << "SoonTM" << std::endl;
    file.close();

    return content;

}
