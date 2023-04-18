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

Source::Source(const string &file, const StringVec &source) {
    this->filePath = file;
    this->sourceCode = source;
    fux.sources.push_back(this);
    fux.libraries.push_back(file);
}

Source::Source(const string &filePath, const bool mainFile) {
    this->filePath = filePath;
    this->fileName = getFileName(filePath);
    this->fileDir = getDirectory(filePath);
    this->fileContent = readFile(filePath);
    this->mainFile = mainFile;
    fux.sources.push_back(this);
    fux.libraries.push_back(fileDir);
}

Source::~Source() {
    filePath.clear();
    fileName.clear();
    fileDir.clear();
    fileContent.clear();
}

void Source::operator()() { parse(); }

string &Source::operator[](size_t line) { return sourceCode.at(line - 1); }

void Source::parse() {
    parser = make_shared<Parser>(filePath, fileContent, mainFile);
    root = parser->parse();

    #ifdef FUX_ANALYSER
    analyser = new Analyser(filePath, root);
    analyser->analyse();
    #endif
}

size_t Source::getFileSize() {
    std::__fs::filesystem::path p {filePath};
    return (size_t) std::__fs::filesystem::file_size(p);
}