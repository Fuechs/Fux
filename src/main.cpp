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
#include <vector>


#include "lexer/BaseLex.hpp"

#include "preprocessor/preprocessor.hpp"

#include "parser/parser.hpp"
#include "parser/ast.hpp"

#include "generator/generator.hpp"

#include "vm/instruction.hpp"
#include "vm/vm.hpp"

namespace fux {

    using
        std::cout,
        std::cerr,
        std::endl,
        std::string,
        std::vector
    ;

    void print_help();
    const string read_file(const string file_path);

    int main(int argc, char **argv) {

        if (argc < 2) {
            cerr << "Missing arguments: command, source.\n"; 
            print_help();
        } else if (strcmp(argv[1], "generate") == 0) {
            if (argc < 3) {
                cerr << "Missing arguments: source.\n";
                print_help();
            } else {
                const string source = read_file(string(argv[2]));

                Lexer lexer = Lexer(source);
                TokenList tokens = lexer.lex();

                PreProcessor preProcessor = PreProcessor(tokens);
                tokens = preProcessor.process();

                for (Token token : tokens)
                    token.debugPrint();
                
                Parser parser = Parser(tokens);
                auto root = parser.parse();

                // TODO: ast analysis
                
                // TODO: generate bytecode
                // Generator generator = Generator(root);
                // auto bytecode = generator.generate();
            }
        } else if (strcmp(argv[1], "vm") == 0) {
            if (argc < 3) {
                cerr << "Missing arguments: source.\n";
                print_help();
            } else {
                // const string source = read_file(string(argv[2]));
                // TODO: read bytecode

                vector<Instruction> code = {
                    Instruction{ PUSH_I32, 0, 4000 },
                    Instruction{ PUSH_I32, 0, 1042 },
                    Instruction{ ADD_I32, 0, 0 },
                    Instruction{ PRINT_I32, 0, 0 },
                    Instruction{ EXIT, 0, 0 }
                };
                
                VM vm;
                vm.code = code;
                vm.run();
            }
        } else {
            cerr << "Unknown command: " << string(argv[1]) << '\n';
            print_help();
        }

        return 0;
    }

    void print_help() {
        cout
            << "--- Fux --- Help ---\n"
            << "Version 0.1\n"
            << "Usage: fux [command] [argument]\n- - - -\n"
            << "help                            shows this message\n"
            << "generate    [source_path]       generates bytecode\n"
            << "vm          [bytecode_path]     interprets bytecode\n- - - -\n"
        ;
    }

    const string read_file(const string file_path) {

        std::ifstream file(file_path);

        if (!file.is_open()) {
            cerr << "Can't open file '" << file_path << "'" << endl;
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