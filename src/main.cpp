// Created by Fuechs on 2022-09-26

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "include/lexer.hpp"

using fux::Lexer, fux::Token;

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


int main(int argc, char** argv) {

    std::string source = read_file("src/test/main.fux");

    Lexer lexer = Lexer(source);
    std::vector<Token> tokens = lexer.lex();
    
    for (Token token : tokens)
        token.debugPrint();

}