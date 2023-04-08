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

Source::Source(ErrorManager *error, const string &filePath, const bool mainFile) {
    this->error = error;
    this->filePath = filePath;
    this->fileName = getFileName(filePath);
    this->fileDir = getDirectory(filePath);
    this->contents = readFile(filePath);
    this->mainFile = mainFile;
}

Source::~Source() {
    filePath.clear();
    fileName.clear();
    fileDir.clear();
    contents.clear();
    delete parser;
    FUX_AC(delete analyser);
}

void Source::operator()() { parse(); }

string &Source::operator[](size_t line) { return sourceCode.at(line - 1); }

void Source::parse() {
    parser = new Parser(error, filePath, contents, mainFile);
    root = parser->parse();
    FUX_AC( analyser = new Analyser(error, root);
            analyser->analyse();)
}

size_t Source::errors() { return error->errors(); }

size_t Source::getFileSize() {
    std::__fs::filesystem::path p {filePath};
    return (size_t) std::__fs::filesystem::file_size(p);
}