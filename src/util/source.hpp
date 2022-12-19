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

#pragma once

#include "../fux.hpp"
#include "../frontend/error/error.hpp"
#include "../frontend/parser/parser.hpp"
#include "../frontend/analyser/analyser.hpp"

class SourceFile {
public:
    SourceFile(const string& filePath, bool mainFile = false);
    
    ~SourceFile();

    // get std::thread to run this->parse
    void operator()(void) { this->parse(); }
    
    // parse file and save RootAST in root
    // will be called for every file that's referenced 
    void parse();

    // from https://stackoverflow.com/a/32286531
    size_t getFileSize();

    ErrorManager *error;
    Parser *parser;
    Analyser *analyser;
    RootAST *root;

    string filePath;
    string fileDir;
    string contents;

    bool mainFile;
};

typedef vector<SourceFile *> FileList;
typedef vector<FileList> FileGroups; 