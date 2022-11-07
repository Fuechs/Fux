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
    ParseError(ErrorType type, size_t line, size_t col, string fileName, string lineContent, string comment = "", bool warning = false, bool aggressive = false);
    ParseError(ErrorType type, Token token, string fileName, string lienContent, string comment = "", bool warning = false, bool aggressive = false);

    void operator=(const ParseError &pe);

    void free();
    void report();

    bool warning, aggressive;
    ErrorType type;
    string message, fileName, lineContent;
    size_t line, col;

private:
    bool reported;
};

typedef vector<ParseError> ErrorList;

// ast needs error -> fake class 

class ErrorManager {
public:
    ErrorManager(string fileName, vector<string> lines) : fileName(fileName), lines(lines) {}

    ~ErrorManager();

    size_t errorCount();
    size_t warningCount();

    bool hasErrors();
    bool hasWarnings();

    void createError(ErrorType type, size_t line, size_t col, string comment = "", bool aggressive = false);
    void createError(ErrorType type, Token token, string comment = "", bool aggressive = false);
    void createError(ErrorType type, AST &ast, string comment = "", bool aggressive = false);

    void createWarning(ErrorType type, size_t line, size_t col, string comment = "", bool aggressive = false);
    void createWarning(ErrorType type, Token token, string comment = "", bool aggressive = false);
    void createWarning(ErrorType type, AST &ast, string comment = "", bool aggressive = false);

    void reportAll();

    // literally self destruct because of a fatal error
    void panic() {
        reportAll();
        delete this;
    }
    
    string fileName;
    vector<string> lines;
    
private:
    ErrorList errors;

    void addError(ParseError error);
};