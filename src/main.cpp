/**
 * @file main.cpp
 * @author fuechs
 * @brief main file of fux
 * @version 0.1
 * @date 2022-09-26
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "lexer/BaseLex.hpp"
#include "parser/parser.hpp"
#include "parser/ast.hpp"
#include "preprocessor/preprocessor.hpp"
#include "vm/chunk.hpp"
#include "vm/vm.hpp"
#include "vm/value.hpp"

namespace fux {

    const string read_file(const string file_path);

    int main(int argc, char** argv) {
        // const string source = read_file("/Users/fuechs/Documents/GitHub/Fux/src/test/main.fux");

        // Lexer lexer = Lexer(source);
        // TokenList tokens = lexer.lex();

        // PreProcessor preProcessor = PreProcessor(tokens);
        // tokens = preProcessor.process();

        // for (Token token : tokens)
        //     token.debugPrint();

        // Parser parser = Parser(tokens);
        // AST root = parser.parse();

        Chunk chunk;
        // analyse ast and generate opcodes ...
        // chunk = opcodes ...
        VM vm;
        vm.constants.push_back(I32(42));
        chunk = {OP_CONST, 0, OP_HALT};
        cout << "PROGRAM\n";
        vm.exec(chunk);
        cout << "END" << endl;

        return 0;
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

}

// dumbass c++ doesn't recognize main in namespace
int main(int argc, char** argv) { return fux::main(argc, argv); } 