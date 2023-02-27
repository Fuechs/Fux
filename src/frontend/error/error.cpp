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

void ErrorManager::createError(string &fileName, ParseError::Type type, const Token &token, string message, string info, bool aggressive) {
    Metadata meta = Metadata(&fileName, &sources.at(fileName), token.line, token.line, token.start, token.end);
    _errors.push_back(ParseError(aggressive ? (ParseError::FlagVec) {ParseError::AGGRESSIVE} : (ParseError::FlagVec) {}, 
        type, message, {meta, info, "", 0}));
}

void ErrorManager::createWarning(string &fileName, ParseError::Type type, const Token &token, string message, string info, bool aggressive) {
    ParseError::FlagVec flags = {ParseError::WARNING};
    if (aggressive)
        flags.push_back(ParseError::AGGRESSIVE);
    Metadata meta = Metadata(&fileName, &sources.at(fileName), token.line, token.line, token.start, token.end);
    _errors.push_back(ParseError(flags, type, message, {meta, info, "", 0}));
}

void ErrorManager::addHelp(string message) {
    _errors.back().addNote("Help: "+message);
}

void ErrorManager::addNote(string message) {
    _errors.back().addNote("Note: "+message);
}

size_t ErrorManager::errors() { return errorCount; }

size_t ErrorManager::warnings() { return warningCount; }