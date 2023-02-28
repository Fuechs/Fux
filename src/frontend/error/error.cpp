/**
 * @file error.cpp
 * @author fuechs
 * @brief fux error manager 
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "error.hpp"

ErrorManager::ErrorManager() : _errors(ParseError::Vec()), 
    errorCount(0), warningCount(0), sources(SourceMap()) {}

ErrorManager::~ErrorManager() {
    _errors.clear();
    sources.clear();
}

void ErrorManager::addSourceFile(const string &fileName, const vector<string> &sourceLines) { sources[fileName] = sourceLines; }

void ErrorManager::createError(
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
) {
    Metadata subjectMeta = Metadata(&subjectFile, &sources.at(subjectFile),
        subjectFstLine, subjectLstLine, subjectFstCol, subjectLstCol);
    Metadata refMeta = Metadata(&refFile, &sources.at(refFile),
        refFstLine, refLstLine, refFstCol, refLstCol);
    ParseError::FlagVec flags = {};
    if (reference)
        flags.push_back(ParseError::REFERENCE);
    if (warning)
        flags.push_back(ParseError::WARNING);
    if (aggressive)
        flags.push_back(ParseError::AGGRESSIVE);
    ParseError pe = ParseError(flags, type, title, 
        ParseError::SUBJ_STRCT(subjectMeta, subjectInfo, subjectPtrText, subjectPtr),
        ParseError::SUBJ_STRCT(refMeta, refInfo, refPtrText, refPtr),
        notes);
    pe.report();
    _errors.push_back(pe);
    
    if (warning)
        warningCount++;
    else
        errorCount++;
}

void ErrorManager::simpleError(
    ParseError::Type type, string title,
    const string &file,
    size_t fstLine, size_t lstLine,
    size_t fstCol, size_t lstCol,
    string info, vector<string> notes, 
    bool warning, bool aggressive) {
        createError(
            type, title, 
            file, fstLine, lstLine, fstCol, lstCol, info, 0, "", 
            file, 0, 0, 0, 0, "", 0, "", // empty reference
            notes, false, warning, aggressive 
        );
}

void ErrorManager::simpleError(ParseError::Type type, string title,
    const string &file,
    size_t fstLine, size_t lstLine,
    size_t fstCol, size_t lstCol, 
    string info, size_t ptr, string ptrText, 
    vector<string> notes, bool warning, bool aggressive) {
        createError(
            type, title, 
            file, fstLine, lstLine, fstCol, lstCol,
            info, ptr, ptrText, 
            file, 0, 0, 0, 0, "", 0, "", // empty reference
            notes, false, warning, aggressive
        );
}

size_t ErrorManager::errors() { return errorCount; }

size_t ErrorManager::warnings() { return warningCount; }