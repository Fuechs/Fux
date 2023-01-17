/**
 * @file error.hpp
 * @author fuechs
 * @brief fux error manager header
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "parseerror.hpp"

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

    // add note to recent ParseError
    void addNote(size_t line, size_t col, string comment = "");

    // report all errors and warnings
    void reportAll();

    // literally self destruct 
    // may be because of a fatal error
    void panic(bool fatal = false) {
        reportAll();
        if (fatal)
            cout 
                << CC::RED << SC::BOLD 
                << "Hit a fatal error.\n" 
                << CC::DEFAULT << SC::RESET;
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