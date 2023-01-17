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

ErrorManager::~ErrorManager() {
    fileName.clear();
    for (ParseError &error : errors)
        error.free();
    errors.clear();
    for (string &line : lines)
        line.clear();
    lines.clear();
}

size_t ErrorManager::errorCount() {
    if (fux.options.werrors) 
        return errors.size();
    
    size_t count = 0;
    
    for (ParseError &error : errors)
        if (!error.warning)
            ++count;
    
    return count;
}

size_t ErrorManager::warningCount() {
    size_t count = 0;
    
    for (ParseError &error : errors)
        if (error.warning)
            ++count;
    
    return count;
}

bool ErrorManager::hasErrors() {
    if (fux.options.werrors)
        return !errors.empty();

    for (ParseError &error : errors)
        if (!error.warning)
            return true;
    
    return false;
}

bool ErrorManager::hasWarnings() {
    for (ParseError &error : errors)
        if (error.warning)
            return true;
    
    return false;
}

void ErrorManager::createError(ErrorType type, size_t line, size_t col, string comment, bool aggressive) {
    addError(ParseError(type, line, line, col, col, fileName, {lines[line - 1]}, comment, false, aggressive));
}

void ErrorManager::createError(ErrorType type, Token &token, string comment, bool aggressive) {
    addError(ParseError(type, token, fileName, {lines[token.line - 1]}, comment, false, aggressive));
}

void ErrorManager::createWarning(ErrorType type, size_t line, size_t col, string comment, bool aggressive) {
    addError(ParseError(type, line, line, col, col, fileName, {lines[line - 1]}, comment, true, aggressive));
}

void ErrorManager::createWarning(ErrorType type, Token &token, string comment, bool aggressive) {
    addError(ParseError(type, token, fileName, {lines[token.line - 1]}, comment, true, aggressive));
}

void ErrorManager::addNote(size_t line, size_t col, string comment) {
    errors.back().addNote(ErrorNote(comment, lines, line, line, col, col));
}

void ErrorManager::reportAll() {
    for (ParseError &error : errors)
        error.report();
}

void ErrorManager::addError(ParseError error) {
    errors.push_back(error);

    if (errorCount() >= fux.options.errorLimit) {
        reportAll();
        cerr 
            << CC::RED << SC::BOLD 
            << "Hit error limit of " << fux.options.errorLimit << "."
            << CC::DEFAULT << SC::RESET
            << endl;
        exit(error.type);
    }
}