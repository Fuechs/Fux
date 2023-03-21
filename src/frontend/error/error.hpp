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
    typedef map<string, vector<string>> SourceMap;

    ErrorManager();
    ~ErrorManager();

    void addSourceFile(const string &fileName, const vector<string> &sourceLines);

    void createError(
        ParseError::Type type, string title,
        // subject
        const string &subjectFile,
        size_t subjectFstLine, size_t subjectLstLine, 
        size_t subjectFstCol, size_t subjectLstCol,
        string subjectInfo, size_t subjectPtr, string subjectPtrText,
        // reference
        const string &refFile,
        size_t refFstLine, size_t refLstLine, 
        size_t refFstCol, size_t refLstCol,
        string refInfo, size_t refPtr, string refPtrText,
        // other
        vector<string> notes, bool reference, bool warning, bool aggressive
    );

    void simpleError(ParseError::Type type, string title,
        const string &file,
        size_t fstLine, size_t lstLine,
        size_t fstCol, size_t lstCol,
        string info, vector<string> notes = {}, 
        bool warning = false, bool aggressive = false);
    
    void simpleError(ParseError::Type type, string title,
        const string &file,
        size_t fstLine, size_t lstLine, 
        size_t fstCol, size_t lstCol, 
        string info, size_t ptr, string ptrText, 
        vector<string> notes = {}, bool warning = false, bool aggressive = false);

    void metaError(ParseError::Type type, string title,
        Metadata &subject, string info, size_t ptr, string ptrText, 
        vector<string> notes = {}, bool warning = false, bool aggressive = false);

    size_t errors();
    size_t warnings();

private:
    ParseError::Vec _errors;
    size_t errorCount;
    size_t warningCount;
    SourceMap sources;
};