/**
 * @file metadata.hpp
 * @author fuechs
 * @brief metadata struct 
 * @version 0.1
 * @date 2023-02-26
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../fux.hpp"

struct Metadata {
    Metadata(string *fileName = nullptr, vector<string> *source = nullptr, 
        size_t fstLine = 0, size_t lstLine = 0, 
        size_t fstCol = 0, size_t lstCol = 0) 
    : file(fileName), source(source), fstLine(fstLine), lstLine(lstLine), 
        fstCol(fstCol), lstCol(lstCol) {}

    Metadata &operator=(const Metadata &copy) {
        file = copy.file;
        source = copy.source;
        fstLine = copy.fstLine;
        lstLine = copy.lstLine;
        fstCol = copy.fstCol;
        lstCol = copy.lstCol;
        return *this;
    }

    string *file;
    vector<string> *source;
    size_t fstLine, lstLine, fstCol, lstCol;
};