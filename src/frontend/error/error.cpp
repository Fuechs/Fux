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

size_t ErrorManager::errors() { return errorCount; }

size_t ErrorManager::warnings() { return warningCount; }