/**
 * @file source.cpp
 * @author fuechs
 * @brief fux source file
 * @version 0.1
 * @date 2022-11-27
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors.eserved.
 * 
 */

#include "source.hpp"

#include <filesystem>

SourceFile::SourceFile(ErrorManager *error, const string &filePath, const bool mainFile) {
    this->error = error;
    this->filePath = filePath;
    this->fileName = getFileName(filePath);
    this->fileDir = getDirectory(filePath);
    this->contents = readFile(filePath);
    this->mainFile = mainFile;
}

SourceFile::~SourceFile() {
    filePath.clear();
    fileName.clear();
    fileDir.clear();
    contents.clear();
    delete parser;
    delete analyser;
    delete error;
}

void SourceFile::parse() {
    parser = new Parser(error, filePath, contents, mainFile);
    root = parser->parse();
    // analyser = new Analyser(error, root);
    // analysed = analyser->analyse();
}

size_t SourceFile::errors() { return error->errors(); }

size_t SourceFile::getFileSize() {
    std::__fs::filesystem::path p {filePath};
    return (size_t) std::__fs::filesystem::file_size(p);
}