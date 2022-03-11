#include <fstream>

#include "include/io.h"

std::string read_file(const char* file_dir) {

    std::string content;

    std::ifstream file(file_dir);

    if (!file) {
        std::cout << "Can't open '" << file_dir << "'" << std::endl;
        return NULL;
    }

    std::cout << "SoonTM" << std::endl;
    file.close();

    return content;

}
