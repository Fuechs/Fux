/**
 * @file error.hpp
 * @author fuechs
 * @brief fux error manager header
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../lexer/token.hpp"
#include "../parser/ast.hpp"

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
    ParseError(ErrorType type, size_t lStart, size_t lEnd, size_t colStart, size_t colEnd, string fileName, vector<string> lineContent, string comment = "", bool warning = false, bool aggressive = false);
    ParseError(ErrorType type, Token &token, string fileName, string lineContent, string comment = "", bool warning = false, bool aggressive = false);

    void operator=(const ParseError &pe);

    // free the error data
    void free();
    // check if error is supposed to be reported and print it out
    // includes checks for aggressive and warnings and werror
    // errormanager has to free error when it's freed
    void report();

    bool warning, aggressive;
    ErrorType type;
    string message, fileName;
    vector<string> lineContent;
    Position pos;

private:
    bool reported;
};

namespace fuxErr { typedef vector<ParseError> ErrorList; }

class ErrorManager {
public:
    ErrorManager(string fileName, vector<string> lines) 
    : fileName(fileName), lines(lines), errors(fuxErr::ErrorList()) {}

    ~ErrorManager();

    size_t errorCount();
    size_t warningCount();

    bool hasErrors();
    bool hasWarnings();

    // create an error with custom line and column
    void createError(ErrorType type, size_t line, size_t col, string comment = "", bool aggressive = false);
    // create an error with token position
    void createError(ErrorType type, Token &token, string comment = "", bool aggressive = false);

    // create a warning with custom line and column
    void createWarning(ErrorType type, size_t line, size_t col, string comment = "", bool aggressive = false);
    // create a warning with token position
    void createWarning(ErrorType type, Token &token, string comment = "", bool aggressive = false);

    // report all errors and warnings
    void reportAll();

    // literally self destruct 
    // may be because of a fatal error
    void panic(bool fatal = false) {
        reportAll();
        if (fatal)
            cout 
                << ColorCode::RED << StyleCode::BOLD 
                << "Hit a fatal error.\n" 
                << ColorCode::DEFAULT << StyleCode::SLIM;
        delete this;
    }

    string fileName;
    vector<string> lines;

    void debugPrint();
    
private:
    fuxErr::ErrorList errors;

    // add an error to errors (checks for errorlimit too)
    void addError(ParseError error);

};