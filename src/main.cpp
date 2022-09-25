#include <iostream>

#include "include/io.hpp"
#include "include/token.hpp"
#include "include/lexer.hpp"
#include "include/parser.hpp"

int main(int argc, const char** argv) {
    std::string source = read_file("src/test/main.fux");

    Lexer lexer = Lexer(source);
    std::vector<Token> tokens = lexer.lex();

    for (Token token : tokens)
        std::cout << token_to_string(token) << "\n";

    Parser parser = Parser(tokens);
    auto root = parser.parse();

    return 0;
}