#include <fstream>
#include <sstream>

#include "include/io.hpp"

std::string read_file(const std::string file_path) {

    std::ifstream file(file_path);

    if (!file) {
        std::cerr << "Can't open file '" << file_path << "'" << std::endl;
        return NULL;
    }

    std::stringstream content;
    content << file.rdbuf();

    file.close();

    return content.str();

}
