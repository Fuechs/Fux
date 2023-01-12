/**
 * @file error.cpp
 * @author fuechs
 * @brief fux error manager 
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#include "error.hpp"
#include "../parser/ast.hpp"

// * ParseError

ParseError::ParseError() {
    reported = false;
    warning = false;
    aggressive = false;
    type = NO_ERR;
    message = "";
    fileName = "";
    lineContent = {};
    pos = Position();
}

ParseError::ParseError(const ParseError &pe) {
    operator=(pe);
}

ParseError::ParseError(ErrorType type, size_t lStart, size_t lEnd, size_t colStart, size_t colEnd, string fileName, vector<string> lineContent, string comment, bool warning, bool aggressive) {
    this->type = type;
    this->message = string(ErrorTypeString[type])+": "+comment;
    this->fileName = fileName;
    this->lineContent = {lineContent};
    this->pos = Position(lStart, lEnd, colStart, colEnd);
    this->reported = false;
    this->warning = warning;
    this->aggressive = aggressive;
}

ParseError::ParseError(ErrorType type, Token &token, string fileName, string lineContent, string comment, bool warning, bool aggressive) {
    this->type = type;
    this->pos = Position(token.line, token.line, token.start, token.end);
    this->message = string(ErrorTypeString[type])+": "+comment;
    this->fileName = fileName;
    this->lineContent = {lineContent};
    this->reported = false;
    this->warning = warning;
    this->aggressive = aggressive;
}

void ParseError::operator=(const ParseError &pe) {
    reported = pe.reported;
    warning = pe.warning;
    aggressive = pe.aggressive;
    type = pe.type;
    message = pe.message;
    fileName = pe.fileName;
    lineContent = pe.lineContent;
    pos = pe.pos;
}

void ParseError::free() {
    message.clear();
    fileName.clear();
    lineContent.clear();
}

void ParseError::report() {
    if ((aggressive && !fux.options.aggressiveErrors) || (warning && !fux.options.warnings) || reported) 
        return;

    if (fux.options.werrors)
        warning = false;

    stringstream errorMessage;
    
    errorMessage << SC::BOLD << fileName << ":" << pos.lStart << ":" << pos.colStart << ": ";
    
    if (warning)
        errorMessage << CC::MAGENTA << "warning: ";
    else
        errorMessage << CC::RED << "error: ";
    
    errorMessage 
        << CC::DEFAULT << "(" << type << ") " << message << "\n\t" // indent for visibility
        << SC::RESET << lineContent.at(0) << "\n\t";
    
    // TODO: correct position for multiple lines (see Position class)
    size_t i;
    for (i = 0; i < (pos.colStart - 1); i++) // -1 so arrow points at exact position
        errorMessage << " ";
    errorMessage << CC::GREEN << SC::BOLD;
    while (i++ < pos.colEnd)
        errorMessage << "^";
    errorMessage << CC::DEFAULT << SC::RESET << endl;
    
    cerr << errorMessage.str();
    reported = true;
}

// * ErrorManager

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