/**
 * @file parseerror.hpp
 * @author fuechs
 * @brief fux ParseError header
 * @version 0.1
 * @date 2023-01-16
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#pragma once

#include "note.hpp"

enum ErrorType {
    UNEXPECTED_SYMBOL,
    ILLEGAL_NUMBER_FORMAT,
    UNEXPECTED_EOF,
    EXPECTED_STRING_LITERAL_EOF,
    ILLEGAL_STRING_FORMAT,
    EXPECTED_CHAR_LITERAL_EOF,
    ILLEGAL_CHAR_LITERAL_FORMAT,
    GENERIC,
    ILLEGAL_ACCESS_DECLARATION,
    ILLEGAL_BRACKET_MISMATCH,
    MISSING_BRACKET,
    INVALID_ACCESS_SPECIFIER,
    MULTIPLE_DEFINITION,
    PREVIOUSLY_DEFINED,
    DUPLICATE_CLASS,
    REDUNDANT_TOKEN,
    INTERNAL_ERROR,
    COULD_NOT_RESOLVE,
    EXPECTED_REFERENCE_OF_TYPE,
    INVALID_CAST,
    REDUNDANT_CAST,
    REDUNDANT_IMPORT,
    UNEXPECTED_TOKEN,
    INVALID_ACCESS,
    SYMBOL_ALREADY_DEFINED,
    INVALID_PARAM,
    INCOMPATIBLE_TYPES,
    DUPLICATE_DECLARATION,
    NO_ERR,
};

// standard messages for each error type
static const char *ErrorTypeString[] = {
    "unexpected symbol",
    "illegal number format mismatch",
    "unexpected end of file",
    "expected string literal before end of file",
    "illegal string format",
    "expected character literal before end of file",
    "illegal character literal format",
    "",
    "illegal specification of access specifier(s)",
    "illegal symbol mismatch, unexpected bracket",
    "missing bracket",
    "invalid access specifier",
    "",
    "previously defined",
    "duplicate class",
    "redundant token",
    "internal runtime error",
    "could not resolve symbol",
    "expected reference of type",
    "invalid cast of type",
    "redundant cast of type",
    "redundant self import of package",
    "unexpected token",
    "invalid access of",
    "",
    "invalid param of type",
    "incompatible types",
    "duplicate declaration of",
    "",
};

class ParseError {
public:
    ParseError();
    ParseError(const ParseError &pe);
    ParseError(ErrorType type, size_t lStart, size_t lEnd, size_t colStart, size_t colEnd, string fileName, vector<string> lines, string comment = "", bool warning = false, bool aggressive = false);
    ParseError(ErrorType type, Token &token, string fileName, string line, string comment = "", bool warning = false, bool aggressive = false);

    void operator=(const ParseError &pe);

    // free the error data
    void free();
    // check if error is supposed to be reported and print it out
    // includes checks for aggressive and warnings and werror
    // errormanager has to free error when it's freed
    void report();

    void addNote(ErrorNote note);

    bool warning, aggressive;
    ErrorType type;
    string message, fileName;
    vector<string> lines;
    NoteList notes;
    Position pos;

private:
    bool reported;
};

namespace fuxErr { typedef vector<ParseError> ErrorList; }