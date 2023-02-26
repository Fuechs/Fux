/**
 * @file parseerror.hpp
 * @author fuechs
 * @brief fux ParseError header
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
#include "../metadata.hpp"

// standard messages for each error type
static const char *ErrorTypeString[] = {
    "",

    "unexpected token",
    "unexpected end of file",
    "unexpected parameter",
    "expected lvalue",

    "unknown error",
    "unknown character",

    "illegal number format",
    "illegal char literal format",
    "illegal string literal format",
    "illegal type",
    "illegal cast",
    "illegal access",
    "illegal import"

    "redundant cast",
    "redundant token",
    "redundant import",

    "duplicate symbol",
    "duplicate declaration",
    
    "missing paren",
};

class ParseError {
public:
    typedef vector<ParseError> Vec;

    enum Type {
        GENERIC,

        UNEXPECTED_TOKEN,
        UNEXPECTED_EOF,
        UNEXPECTED_PARAMETER,
        EXPECTED_LVALUE,

        UNKNOWN_ERROR,
        UNKNOWN_CHARACTER,
        
        ILLEGAL_NUMBER_FORMAT,
        ILLEGAL_CHAR_LITERAL_FORMAT,
        ILLEGAL_STRING_LITERAL_FORMAT,
        ILLEGAL_TYPE,
        ILLEGAL_CAST,
        ILLEGAL_ACCESS,
        ILLEGAL_IMPORT,

        REDUNDANT_CAST,
        REDUNDANT_TOKEN,
        REDUNDANT_IMPORT,

        DUPLICATE_SYMBOL,
        DUPLICATE_DECLARATION,

        MISSING_PAREN,
    };

    enum Flag {
        WARNING,        // is a warning
        AGGRESSIVE,     // is aggressive
        REPORTED,       // was reported
        REFERENCE,      // has a reference to another code
    };
    
    typedef vector<Flag> FlagVec;

    ParseError();
    ~ParseError();

    void report();

// private:
    FlagVec flags;
    Type type;
        
    string title;
    string info;
    Metadata subject;
    Metadata reference;
    
    vector<string> notes;

    // helper functions for error reporting

    string pad(size_t padding, char fill = ' ');
    string tripleDot(size_t padding);
    string printLine(size_t lineNumber, string line, size_t padding);
};

// class ParseError {
// public:
//     typedef vector<ParseError> Vec;

//     ParseError();
//     ParseError(const ParseError &pe);
//     ParseError(ErrorType type, size_t lStart, size_t lEnd, size_t colStart, size_t colEnd, string fileName, vector<string> lines, string comment = "", bool warning = false, bool aggressive = false);
//     ParseError(ErrorType type, Token &token, string fileName, string line, string comment = "", bool warning = false, bool aggressive = false);

//     void operator=(const ParseError &pe);

//     // free the error data
//     void free();
//     // check if error is supposed to be reported and print it out
//     // includes checks for aggressive and warnings and werror
//     // errormanager has to free error when it's freed
//     void report();

//     void addNote(ErrorNote note);

//     bool warning, aggressive;
//     ErrorType type;
//     string message, fileName;
//     vector<string> lines;
//     NoteList notes;
//     Position pos;

// private:
//     bool reported;
// };