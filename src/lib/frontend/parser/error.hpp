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

#include "../../../include.hpp"
#include "../../util/keypair.hpp"
#include "lexer/token.hpp"
#include "../list.hpp"

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

static list<keypair<ErrorType, string>> predefinedErrors;
void initialzeErrors();

class ParseError {
public:
    ParseError() {
        error = "";
        id = NO_ERR;
        line = 0;
        col = 0;
    }

    ParseError(const ParseError &pe) {
        operator=(pe);
    }

    ParseError(
        keypair<ErrorType, string> err,
        int l,
        int c, 
        string addon = ""
    ) {
        id = err.key;
        error = err.value + addon;
        line = l;
        col = c;
        warning = false;
    } 

    ParseError(
        bool warning,
        keypair<ErrorType, string> err,
        int l,
        int c, 
        string addon = ""
    ) {
        id = err.key;
        error = err.value + addon;
        line = l;
        col = c;
        this->warning = warning;
    }

    ParseError(
        keypair<ErrorType, string> err,
        Token token,
        string addon = ""
    ) {
        id = err.key;
        error = err.value + addon;
        line = token.line;
        col = token.col;
        this->warning = warning;
    }

    void operator=(const ParseError &pe) {
        error = pe.error;
        id = pe.id;
        line = pe.line;
        col = pe.col;
        warning = pe.warning;
    }

    void free() {
        error.clear();
    }

    ErrorType id;
    string error;
    int line;
    int col;
    bool warning;
};

class AST {};

class ErrorManager {
public:
    ErrorManager(
        List<string> &lines, 
        string file_name, 
        bool asis, 
        bool aggressiveReporting
    ) 
    :   lines(), teCursor(-1), _err(false), cm(false), 
        filename(file_name), asis(asis), aggressive(aggressiveReporting) 
    {
        this->lines.addAll(lines);
        errors = new list<ParseError>();
        warnings = new list<ParseError>();
        unfilteredErrors = new list<ParseError>();
        possibleErrors = new list<list<ParseError>*>();
        lastError = ParseError();
        lastCheckedError = ParseError();
    }

    void printErrors();
    uint64_t getErrorCount() { return errors->size(); }
    uint64_t getWarningCount() { return unfilteredErrors->size(); }
    int createNewError(ErrorType err, Token token, string xcmts = "");
    int createNewError(ErrorType err, AST *pAST, string xcmts = "");
    void createNewError(ErrorType err, int line, int col, string xcmts);
    void createNewWarning(ErrorType err, int line, int col, string xcmts);
    void createNewWarning(ErrorType err, AST *pAST, string xcmts);
    bool hasErrors();
    bool hasWarnings() { return warnings; }
    void enableErrorCheckMode();
    void fail();
    void pass();

    void free();
    string getLine(int line);

private:
    keypair<ErrorType, string> getErrorID(ErrorType);
    list<ParseError>* getPossibleErrorList();
    void addPossibleErrorList();
    void removePossibleErrorList();

    List<string> lines;
    list<ParseError> *errors, *unfilteredErrors, *warnings;
    list<list<ParseError>*> *possibleErrors;
    int64_t teCursor;
    ParseError lastError;
    ParseError lastCheckedError;
    bool _err, cm, asis, aggressive;
    // cm: error check mode
    string filename;

    bool shouldReport(Token *token, const ParseError &last_err, const ParseError &e) const;
    string getErrors(list<ParseError> *errors);
    void printError(ParseError &err);
    bool hasError(list<ParseError> *e, const ParseError &parseerror1) const;
    bool shouldReportWarning(Token *token, const ParseError &last_err, const ParseError &e) const;
    keypair<ErrorType, string> getErrorByID(ErrorType err);
};