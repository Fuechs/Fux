/**
 * @file metadata.hpp
 * @author fuechs
 * @brief metadata struct header
 * @version 0.1
 * @date 2023-02-26
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../fux.hpp"
#include "lexer/token.hpp"

struct Metadata {
    Metadata(const string *fileName = nullptr, vector<string> *source = nullptr, 
        size_t fstLine = 0, size_t lstLine = 0, size_t fstCol = 0, size_t lstCol = 0);
    Metadata(const string &fileName, Token &token);

    Metadata &operator=(const Metadata &copy);

    // get line from source code (line number, not index!)
            string &operator[](size_t line);
    const   string &operator[](size_t line) const;

    // copy whole position of a token
    void copyWhole(const Token &token);
    // copy whole position of other metadata (without file, source)
    void copyWhole(const Metadata &meta);

    // copy end position of a token
    void copyEnd(const Token &token);
    // copy end position of other metadata (without file, source)
    void copyEnd(const Metadata &meta);

    const string *file;
    vector<string> *source;
    size_t fstLine, lstLine, fstCol, lstCol;
};