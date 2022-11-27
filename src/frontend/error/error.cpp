/**
 * @file error.cpp
 * @author fuechs
 * @brief fux error manager 
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "error.hpp"
// #include "../parser/ast.hpp"

// * ParseError

ParseError::ParseError() {
    reported = false;
    warning = false;
    aggressive = false;
    type = NO_ERR;
    message = "";
    fileName = "";
    lineContent = "";
    line = 1;
    start = 1;
    end = 1;
}

ParseError::ParseError(const ParseError &pe) {
    operator=(pe);
}

ParseError::ParseError(ErrorType type, size_t line, size_t start, size_t end, string fileName, string lineContent, string comment, bool warning, bool aggressive) {
    this->type = type;
    this->line = line;
    this->start = start;
    this->end = end;
    this->message = string(ErrorTypeString[type])+": "+comment;
    this->fileName = fileName;
    this->lineContent = lineContent;
    reported = false;
    this->warning = warning;
    this->aggressive = aggressive;
}

ParseError::ParseError(ErrorType type, Token &token, string fileName, string lineContent, string comment, bool warning, bool aggressive) {
    this->type = type;
    this->line = token.line;
    this->start = token.start;
    this->end = token.end;
    this->message = string(ErrorTypeString[type])+": "+comment;
    this->fileName = fileName;
    this->lineContent = lineContent;
    reported = false;
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
    line = pe.line;
    start = pe.start;
    end = pe.end;
}

void ParseError::free() {
    message.clear();
    fileName.clear();
    lineContent.clear();
}

// check if error is supposed to be reported and print it out
// includes checks for aggressive and warnings and werror
// errormanager has to free error when it's freed
void ParseError::report() {
    if ((aggressive && !fux.options.aggressiveErrors) || (warning && !fux.options.warnings) || reported) 
        return;

    if (fux.options.werrors)
        warning = false;

    stringstream errorMessage;
    
    errorMessage << StyleCode::BOLD << fileName << ":" << line << ":" << start << ": ";
    
    if (warning)
        errorMessage << ColorCode::MAGENTA << "warning: ";
    else
        errorMessage << ColorCode::RED << "error: ";
    
    errorMessage 
        << ColorCode::DEFAULT << "(" << type << ") " << message << "\n\t" // indent for visibility
        << StyleCode::SLIM << lineContent << "\n\t";
    
    size_t i;
    for (i = 0; i < (start - 1); i++) // -1 so arrow points at exact position
        errorMessage << " ";
    errorMessage << ColorCode::GREEN << StyleCode::BOLD;
    while (i++ < end)
        errorMessage << "^";
    errorMessage << ColorCode::DEFAULT << StyleCode::SLIM << endl;
    
    cerr << errorMessage.str();
    
    errorMessage.clear();
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
    addError(ParseError(type, line, col, col, fileName, lines[line - 1], comment, false, aggressive));
}

void ErrorManager::createError(ErrorType type, Token &token, string comment, bool aggressive) {
    addError(ParseError(type, token, fileName, lines[token.line - 1], comment, false, aggressive));
}

void ErrorManager::createWarning(ErrorType type, size_t line, size_t col, string comment, bool aggressive) {
    addError(ParseError(type, line, col, col, fileName, lines[line - 1], comment, true, aggressive));
}

void ErrorManager::createWarning(ErrorType type, Token &token, string comment, bool aggressive) {
    addError(ParseError(type, token, fileName, lines[token.line - 1], comment, true, aggressive));
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
            << ColorCode::RED << StyleCode::BOLD 
            << "Hit error limit of " << fux.options.errorLimit << "."
            << ColorCode::DEFAULT << StyleCode::SLIM
            << endl;
        exit(error.type);
    }
}