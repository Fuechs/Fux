/**
 * @file main.cpp
 * @author fuechs (fuechsss@gmail.com)
 * @brief main file of fux
 * @version 0.1
 * @date 2022-09-26
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. 
 *            All rights reserved.
 *            BSD 3-Clause License
 * 
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "include/BaseLex.hpp"
#include "include/parser.hpp"
#include "include/ast.hpp"
#include "include/preprocessor.hpp"

using 
    BaseLex::Lexer, 
    BaseLex::Token, 
    BaseLex::TokenList,
    fux::Parser,
    fux::AST,
    fux::PreProcessor,
    std::string
;

const string read_file(const string file_path);

int main(int argc, char** argv) {
    const string source = read_file("/Users/fuechs/Documents/GitHub/Fux/src/test/main.fux");

    Lexer lexer = Lexer(source);
    TokenList tokens = lexer.lex();

    PreProcessor preProcessor = PreProcessor(tokens);
    tokens = preProcessor.process();

    for (Token token : tokens)
        token.debugPrint();

    Parser parser = Parser(tokens);
    AST root = parser.parse();
}

const string read_file(const string file_path) {

    std::ifstream file(file_path);

    if (!file) {
        std::cerr << "Can't open file '" << file_path << "'" << std::endl;
        exit(1);
    }

    std::stringstream content;
    content << file.rdbuf();

    file.close();

    return content.str();

}