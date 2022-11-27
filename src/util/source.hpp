/**
 * @file source.hpp
 * @author fuechs
 * @brief fux source file header
 * @version 0.1
 * @date 2022-11-26
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "../fux.hpp"
#include "../frontend/error/error.hpp"
#include "../frontend/parser/parser.hpp"

class SourceFile {
public:
    SourceFile(const string& filePath, bool mainFile = false);
    
    ~SourceFile();
    
    AST *parse();

    ErrorManager *error;
    Parser *parser;

    string filePath;
    string fileDir;
    string contents;

    bool mainFile;
};