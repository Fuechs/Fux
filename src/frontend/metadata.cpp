/**
 * @file metadata.cpp
 * @author fuechs
 * @brief metadata
 * @version 0.1
 * @date 2023-04-07
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "metadata.hpp"
#include "../context.hpp"

Metadata::Metadata(Context::Ptr ctx, std::string file, 
    size_t fstLine, size_t lstLine, size_t fstCol, size_t lstCol) 
        : ctx(ctx), file(file), fstLine(fstLine), lstLine(lstLine), 
            fstCol(fstCol), lstCol(lstCol) {}

Metadata::~Metadata() { file.clear(); }

Metadata &Metadata::operator=(const Metadata &copy) {
    ctx = copy.ctx;
    file = copy.file;
    fstLine = copy.fstLine;
    lstLine = copy.lstLine;
    fstCol = copy.fstCol;
    lstCol = copy.lstCol;
    return *this;
}

        std::string &Metadata::operator[](size_t line) { return ctx->source(file).at(line - 1); }

const   std::string &Metadata::operator[](size_t line) const { return ctx->source(file).at(line - 1); }

void Metadata::copyWhole(const Metadata &meta) {
    this->fstLine = meta.fstLine;
    this->lstLine = meta.lstLine;
    this->fstCol = meta.fstCol;
    this->lstCol = meta.lstCol;
}

void Metadata::copyEnd(const Metadata &meta) {
    this->lstLine = meta.lstLine;
    this->lstCol = meta.lstCol;
}