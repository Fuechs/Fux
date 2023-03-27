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

void ErrorManager::addSourceFile(const string &fileName, const vector<string> &sourceLines) { 
    sources[fileName] = sourceLines; 
}

void ErrorManager::plainError(ParseError::Type type, string title, 
    Metadata meta, Marking::Vec marks, bool aggressive) {

        meta.source = &sources[*meta.file];
        if (aggressive)
            update(ParseError({ParseError::AGGRESSIVE}, type, title, Subject(meta, marks))); 
        else
            update(ParseError({}, type, title, Subject(meta, marks)));
}

void ErrorManager::plainError(ParseError::Type type, string title,
    const string &file, Marking::Vec marks, bool aggressive) {
        return plainError(type, title, getMeta(file), marks, aggressive);
}

void ErrorManager::plainWarning(ParseError::Type type, string title, 
    Metadata meta, Marking::Vec marks, bool aggressive) {

        meta.source = &sources[*meta.file];
        if (aggressive)
            update(ParseError({ParseError::AGGRESSIVE, ParseError::WARNING}, 
                type, title, Subject(meta, marks))); 
        else
            update(ParseError({ParseError::WARNING}, type, title, Subject(meta, marks)));
}

void ErrorManager::plainWarning(ParseError::Type type, string title,
    const string &file, Marking::Vec marks, bool aggressive) {
        return plainWarning(type, title, getMeta(file), marks, aggressive);
}

void ErrorManager::simpleError(ParseError::Type type, string title,
    Metadata meta, size_t line, size_t start, size_t end, string info, bool aggressive) {

        meta.source = &sources[*meta.file];
        Marking::Vec marks = {createUL(line, start, end, 0, info)};
        if (aggressive)
            update(ParseError({ParseError::AGGRESSIVE}, type, title, Subject(meta, marks)));
        else
            update(ParseError({}, type, title, Subject(meta, marks)));
}

void ErrorManager::simpleWarning(ParseError::Type type, string title,
    Metadata meta, size_t line, size_t start, size_t end, string info, bool aggressive) {

        meta.source = &sources[*meta.file];
        Marking::Vec marks = {createUL(line, start, end, 0, info)};
        if (aggressive)
            update(ParseError({ParseError::AGGRESSIVE, ParseError::WARNING}, 
                type, title, Subject(meta, marks)));
        else
            update(ParseError({ParseError::WARNING}, type, title, Subject(meta, marks)));
}

void ErrorManager::refError(ParseError::Type type, string title,
    Metadata subj, Marking::Vec subjMarks, Metadata ref, Marking::Vec refMarks,
    bool warning, bool aggressive) {

        subj.source = &sources[*subj.file];
        ref.source = &sources[*ref.file];
        ParseError::FlagVec flags = {ParseError::REFERENCE};

        if (warning)
            flags.push_back(ParseError::WARNING);
        if (aggressive)
            flags.push_back(ParseError::AGGRESSIVE);
        
        update(ParseError(flags, type, title, Subject(subj, subjMarks), Subject(ref, refMarks)));
}

Metadata ErrorManager::getMeta(const string &fileName) { 
    return Metadata(&fileName, &sources[fileName], 1, sources[fileName].size(), 1, sources[fileName].back().size());
}

size_t ErrorManager::errors() { return errorCount; }

size_t ErrorManager::warnings() { return warningCount; }

Marking ErrorManager::createUL(size_t line, size_t start, size_t end, size_t except, string info) {
    return Marking(except == 0 ? Marking::ARROW_UL : Marking::DASH_UL, info, line, start, except, end);
}

Marking ErrorManager::createUL(const Metadata &meta, size_t except, string info) {
    return createUL(meta.fstLine, meta.fstCol, meta.lstCol, except, info);
}

Marking ErrorManager::createPtr(size_t line, size_t col, string info) {
    return Marking(Marking::POINTER, info, line, col, col, col);
}


Marking::Vec ErrorManager::createMark(size_t fstLine, size_t lstLine, size_t start, size_t end, 
    string info, size_t ptr, string ptrInfo, Marking::Vec append) {
        
        Marking::Vec marks = {};

        if (fstLine != lstLine) 
            marks.push_back(createMulti(fstLine, lstLine, info));
        else {
            marks.push_back(createUL(fstLine, start, end, ptr, info));
            if (ptr != 0)
                marks.push_back(createPtr(fstLine, ptr, ptrInfo));
        }

        marks.reserve(append.size());
        for (Marking &mark : append)
            marks.push_back(mark);
        return marks;
}

Marking::Vec ErrorManager::createMark(const Metadata &meta, string info, size_t ptr, string ptrInfo, Marking::Vec append) {
    return createMark(meta.fstLine, meta.lstLine, meta.fstCol, meta.lstCol, info, ptr, ptrInfo, append);
}

Marking ErrorManager::createHelp(size_t line, string message) {
    return Marking(Marking::HELP, message, line);
}

Marking ErrorManager::createNote(size_t line, string message) {
    return Marking(Marking::NOTE, message, line);
}

Marking ErrorManager::createMulti(size_t fstLine, size_t lstLine, string message) {
    return Marking(Marking::MULTILINE, message, fstLine, fstLine, 0, lstLine);
}

void ErrorManager::update(ParseError pe) {
    _errors.push_back(pe);

    if (pe.hasFlag(ParseError::WARNING))
        warningCount++;
    else
        errorCount++;
    
    pe.report();
}