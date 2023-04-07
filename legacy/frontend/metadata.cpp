/**
 * @file metadata.cpp
 * @author fuechs
 * @brief metadata struct 
 * @version 0.1
 * @date 2023-03-22
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "metadata.hpp"

Metadata::Metadata(const string *fileName, vector<string> *source, 
    size_t fstLine, size_t lstLine, size_t fstCol, size_t lstCol) 
        : file(fileName), source(source), fstLine(fstLine), lstLine(lstLine), 
            fstCol(fstCol), lstCol(lstCol) {}

Metadata::Metadata(const string &fileName, Token &token)
    : file(&fileName), source(nullptr), fstLine(token.line), lstLine(token.line),
        fstCol(token.start), lstCol(token.end) {}

Metadata &Metadata::operator=(const Metadata &copy) {
    file = copy.file;
    source = copy.source;
    fstLine = copy.fstLine;
    lstLine = copy.lstLine;
    fstCol = copy.fstCol;
    lstCol = copy.lstCol;
    return *this;
}

        string &Metadata::operator[](size_t line) { return source->at(line - 1); }

const   string &Metadata::operator[](size_t line) const { return source->at(line - 1); }

void Metadata::copyWhole(const Token &token) {
    this->fstLine = token.line;
    this->lstLine = token.line;
    this->fstCol = token.start;
    this->lstCol = token.end;
}

void Metadata::copyWhole(const Metadata &meta) {
    this->fstLine = meta.fstLine;
    this->lstLine = meta.lstLine;
    this->fstCol = meta.fstCol;
    this->lstCol = meta.lstCol;
}

void Metadata::copyEnd(const Token &token) {
    this->lstLine = token.line;
    this->lstCol = token.end;
}

void Metadata::copyEnd(const Metadata &meta) {
    this->lstLine = meta.lstLine;
    this->lstCol = meta.lstCol;
}