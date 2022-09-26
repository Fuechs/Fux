// Created by Fuechs on 2022-09-25

#include <iostream>
#include <vector>

#include "include/io.hpp"
#include "include/tokenizer.hpp"
#include "include/parser.hpp"

using fux::Token, fux::Tokenizer, fux::Parser;

int main() {
    std::cout << "Fux 0.1\n" << std::endl;

    std::string fileContents = read_file("src/test/main.fux");

    // std::cout << fileContents << std::endl;

    Tokenizer tokenizer;
    std::vector<Token> tokens = tokenizer.parse(fileContents);

    // for (Token currToken : tokens)
    //     currToken.debugPrint();

    Parser parser;
    parser.parse(tokens);

    parser.debugPrint();

    return 0;
}