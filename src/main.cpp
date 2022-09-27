// Created by Fuechs on 2022-09-26

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "include/BaseLex.hpp"

using BaseLex::Lexer, BaseLex::Token, BaseLex::TokenList;
using std::string;

std::string read_file(const string file_path);

int main(int argc, char** argv) {
    string source = read_file("src/test/main.fux");

    Lexer lexer = Lexer(source);
    TokenList tokens = lexer.lex();
    
    for (Token token : tokens)
        token.debugPrint();
}

string read_file(const string file_path) {

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