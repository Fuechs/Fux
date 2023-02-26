/**
 * @file parseerror.cpp
 * @author fuechs
 * @brief fux ParseError
 * @version 0.1
 * @date 2023-01-16
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "parseerror.hpp"

ParseError::ParseError() {
    flags = FlagVec();
    type = UNKNOWN_ERROR;
    file = nullptr;
    source = nullptr;
    title = "";
    subject = nullptr;
    notes = vector<string>();
}

ParseError::~ParseError() { 
    flags.clear(); 
    title.clear();
}

void ParseError::report() {
    stringstream ss = stringstream();
    size_t padding = 2;

    // header 

    ss << SC::BOLD << "[ ";
    if (find(flags.begin(), flags.end(), WARNING) != flags.end())
        ss << CC::MAGENTA << "warning";
    else
        ss << CC::RED << "error";
    ss << CC::DEFAULT << " ][ " << to_string(type) << " ]: " << ErrorTypeString[type];
    if (!title.empty())
        ss << ": " << title;

    // position

    ss << SC::RESET << "\n"<< pad(padding - 2) << CC::BLUE << ">>> " << CC::DEFAULT
        << *file << ":" << subject->line << ":" << subject->start << "\n";

    // source and pointers
    
    string lineNumber = to_string(subject->line);
    ss << CC::BLUE << lineNumber << pad(padding-lineNumber.size()) << "|\t" << CC::DEFAULT;    
    ss << CC::GRAY << (*source)[subject->line - 1] << CC::DEFAULT << "\n";
    
    size_t i;
    ss << pad(padding) << CC::GREEN << SC::BOLD << "|\t";
    for (i = 0; i < (subject->start - 1); i++) // -1 so arrow points at exact position
        ss << " ";
    while (i++ < subject->end)
        ss << "^";
    ss << CC::DEFAULT << SC::RESET;
    if (!info.empty())
        ss << "\t<- " << info;
    ss  << "\n" << CC::BLUE << "..." << pad(padding - 3) << "|\t" << CC::DEFAULT << "...\n";

    cerr << ss.str();
}

string ParseError::pad(size_t padding) {
    string ret = "";
    while (padding --> 0)
        ret += " ";
    return ret;
}

// ParseError::ParseError() {
//     reported = false;
//     warning = false;
//     aggressive = false;
//     type = NO_ERR;
//     message = "";
//     fileName = "";
//     lines = {};
//     notes = {};
//     pos = Position();
// }

// ParseError::ParseError(const ParseError &pe) { operator=(pe); }

// ParseError::ParseError(ErrorType type, size_t lStart, size_t lEnd, size_t colStart, size_t colEnd, string fileName, vector<string> lines, string comment, bool warning, bool aggressive) {
//     this->type = type;
//     this->message = string(ErrorTypeString[type])+": "+comment;
//     this->fileName = fileName;
//     this->lines = lines;
//     this->pos = Position(lStart, lEnd, colStart, colEnd);
//     this->reported = false;
//     this->warning = warning;
//     this->aggressive = aggressive;
// }

// ParseError::ParseError(ErrorType type, Token &token, string fileName, string line, string comment, bool warning, bool aggressive) {
//     this->type = type;
//     this->pos = Position(token.line, token.line, token.start, token.end);
//     this->message = string(ErrorTypeString[type])+": "+comment;
//     this->fileName = fileName;
//     this->lines = {line};
//     this->reported = false;
//     this->warning = warning;
//     this->aggressive = aggressive;
// }

// void ParseError::operator=(const ParseError &pe) {
//     reported = pe.reported;
//     warning = pe.warning;
//     aggressive = pe.aggressive;
//     type = pe.type;
//     message = pe.message;
//     fileName = pe.fileName;
//     lines = pe.lines;
//     notes = pe.notes;
//     pos = pe.pos;
// }

// void ParseError::free() {
//     message.clear();
//     fileName.clear();
//     lines.clear();
// }

// void ParseError::addNote(ErrorNote note) { notes.push_back(note); }


// void ParseError::report() {
//     if ((aggressive && !fux.options.aggressiveErrors) || (warning && !fux.options.warnings) || reported) 
//         return;

//     if (fux.options.werrors)
//         warning = false;

//     stringstream errorMessage;
    
//     errorMessage << SC::BOLD << fileName << ":" << pos.lStart << ":" << pos.colStart << ": ";
    
//     if (warning)
//         errorMessage << CC::MAGENTA << "warning: ";
//     else
//         errorMessage << CC::RED << "error: ";
    
//     errorMessage 
//         << CC::DEFAULT << "(" << type << ") " << message << "\n\t" // indent for visibility
//         << SC::RESET << lines.at(0) << "\n\t";
    
//     // TODO: correct position for multiple lines (see Position class)
//     size_t i;
//     for (i = 0; i < (pos.colStart - 1); i++) // -1 so arrow points at exact position
//         errorMessage << " ";
//     errorMessage << CC::GREEN << SC::BOLD;
//     while (i++ < pos.colEnd)
//         errorMessage << "^";
//     errorMessage << CC::DEFAULT << SC::RESET << endl;

//     for (ErrorNote &note : notes)
//         errorMessage << note.str();
    
//     cerr << errorMessage.str();
//     reported = true;
// }