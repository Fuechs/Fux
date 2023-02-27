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

/**
 * TODO: Rewrite the error system.
 * * Include more metadata about the error (e.g. "associated" code with the error is underlined, etc.),
 * * make errors prettier and easier to read,
 * * throw error as soon as it is encountered in the code,
 * * and make the system overall more user-friendly (for developers working on the compiler).
 */
class ErrorManager {
public:
    typedef map<string, vector<string>> SourceMap;

    ErrorManager();
    ~ErrorManager();

    void addSourceFile(const string &fileName, const vector<string> &sourceLines);

    void createError(const string &fileName, ParseError::Type type, const Token &token, string message, string info = "", bool aggressive = false);
    void createWarning(const string &fileName, ParseError::Type type, const Token &token, string message, string info = "", bool aggressive = false);

    void addNote(string message);
    void addHelp(string message);

    void report();

    size_t errors();
    size_t warnings();

private:
    ParseError::Vec _errors;
    size_t errorCount;
    size_t warningCount;
    SourceMap sources;
};