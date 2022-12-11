/**
 * @file source.cpp
 * @author fuechs
 * @brief fux source file
 * @version 0.1
 * @date 2022-11-27
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "source.hpp"

SourceFile::SourceFile(const string& filePath, bool mainFile) {
    this->filePath = filePath;
    this->fileDir = getDirectory(filePath);
    this->contents = readFile(filePath);
    this->mainFile = mainFile;
}

SourceFile::~SourceFile() {
    filePath.clear();
    fileDir.clear();
    contents.clear();
    error->panic();
    delete parser;
    delete analyser;
}

RootAST *SourceFile::parse() {
    error = new ErrorManager(filePath, vector<string>());
    parser = new Parser(error, filePath, contents, mainFile);
    RootAST *root = parser->parse();
    analyser = new Analyser(error, root);
    analyser->analyse();
    return root;
}