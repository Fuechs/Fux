/**
 * @file note.hpp
 * @author fuechs
 * @brief Fux ErrorNote header
 * @version 0.1
 * @date 2023-01-16
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../ast/position.hpp"
#include "../lexer/token.hpp"

class ErrorNote {
public:
    ErrorNote(const string message, const vector<string> &lines, size_t lStart = 1, size_t lEnd = 1, size_t colStart = 1, size_t colEnd = 1)
    : message(message), lines(lines), pos(lStart, lEnd, colStart, colEnd) {} 
    ErrorNote(const string message, const vector<string> &lines, Token &token)
    : message(message), lines(lines), pos(token.line, token.start, token.end) {}
    ~ErrorNote();

    // return string value of error note
    string str();

private:
    string message;
    vector<string> lines;
    Position pos;
};

typedef vector<ErrorNote> NoteList;