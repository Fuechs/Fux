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
    typedef map<string, StringVec> SourceMap;

    ErrorManager();
    ~ErrorManager();

    // provide metadata and markings
    void plainError(ParseError::Type type, string title,
        Metadata meta, Marking::Vec marks, bool aggressive = false);

    void plainError(ParseError::Type type, string title,
        const string &file, Marking::Vec marks, bool aggressive = false);

    // provide metadata and markings (creates warning)
    void plainWarning(ParseError::Type type, string title,
        Metadata meta, Marking::Vec marks, bool aggressive = false);
    
    void plainWarning(ParseError::Type type, string title,
        const string &file, Marking::Vec marks, bool aggressive = false);

    // create an error with an underline from `start` to `end`
    // and an additional information
    void simpleError(ParseError::Type type, string title,
        Metadata meta, size_t line, size_t start, size_t end, 
        string info = "", bool aggressive = false);

    // create a warning with an underline from `start` to `end`
    // and an additional information
    void simpleWarning(ParseError::Type type, string title,
        Metadata meta, size_t line, size_t start, size_t end, 
        string info = "", bool aggressive = false);

    // error with reference
    void refError(ParseError::Type type, string title,
        Metadata subj, Marking::Vec subjMarks, Metadata ref, Marking::Vec refMarks,
        bool warning = false, bool aggressive = false);

    size_t errors();
    size_t warnings();

    // create underline marking
    // if except == 0 then ARROW_UL else DASH_UL
    // except needs to be provided if there is a pointer marking on the same line
    Marking createUL(size_t line, size_t start, size_t end, size_t except = 0, string info = "");
    // uses meta.fstLine, meta.fstCol, meta.lstCol
    Marking createUL(const Metadata &meta, size_t except = 0, string info = "");
    // create pointer marking
    Marking createPtr(size_t line, size_t col, string info = "");

    // create underline marking
    // if ptr != 0 then create pointer marking too
    // if fstLine != lstLine then create only multi-line marking
    Marking::Vec createMark(size_t fstLine, size_t lstLine, size_t start, size_t end, 
        string info = "", size_t ptr = 0, string ptrInfo = "", Marking::Vec append = {});

    // create underline marking
    // if ptr != 0 then create pointer marking too
    // if fstLine != lstLine then create only multi-line marking
    Marking::Vec createMark(const Metadata &meta, string info = "", size_t ptr = 0, string ptrInfo = "", Marking::Vec append = {});

    Marking createHelp(size_t line, string message);
    Marking createNote(size_t line, string message);
    Marking createReplace(size_t line, size_t start, size_t end, string replacement);
    Marking createInsert(size_t line, size_t col, string insertion);
    Marking createInsert(size_t line, size_t col, string insertion, size_t col1, string insertion1);
    Marking createRemove(size_t line, size_t start, size_t end);
    Marking createMulti(size_t fstLine, size_t lstLine, string message);
    Marking createHighlight(size_t fstLine, size_t lstLine, size_t fstCol, size_t lstCol, string info);

private:
    ParseError::Vec _errors;
    size_t errorCount;
    size_t warningCount;

    // update count and report error
    void update(ParseError pe);
};