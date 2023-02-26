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
    title = "";
    subject = Metadata();
    reference = Metadata();
    notes = vector<string>();
}

ParseError::~ParseError() { 
    flags.clear(); 
    title.clear();
}

void ParseError::report() {
    stringstream ss = stringstream();
    size_t padding = to_string(std::max({subject.lstLine, reference.lstLine})).size() + 3;

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
        << *subject.file << ":" << subject.fstLine << ":" << subject.fstCol << "\n";

    // source and pointers    
    if (subject.fstCol != 0) {
        ss << printLine(subject.fstLine, subject.source->at(subject.fstLine - 1), padding);

        size_t i;
        ss << pad(padding) << CC::GREEN << SC::BOLD << "|\t";
        for (i = 0; i < (subject.fstCol - 1); i++) // -1 so arrow points at exact position
            ss << " ";
        while (i++ < subject.lstCol)
            ss << "^";

        if (!info.empty())
            ss << "\t<-- " << info;

        ss << CC::DEFAULT << SC::RESET << "\n";
    } else {
        if (subject.lstLine - subject.fstLine > 8) {
            ss << printLine(subject.fstLine, subject.source->at(subject.fstLine - 1), padding);
            ss << tripleDot(padding);
            ss << printLine(subject.lstLine, subject.source->at(subject.lstLine - 1), padding);
        } else 
            for (size_t line = subject.fstLine; line < subject.lstLine + 1; line++)
                ss << printLine(line, subject.source->at(line - 1), padding);
        
        if (!info.empty())
            ss << CC::GREEN << SC::BOLD << pad(padding) << " \\__ " << info << "\n" << CC::DEFAULT << SC::RESET;
    }  

    ss << tripleDot(padding);

    cerr << ss.str();
}

string ParseError::pad(size_t padding, char fill) {
    string ret = "";
    while (padding --> 0)
        ret += fill;
    return ret;
}

string ParseError::tripleDot(size_t padding) {
    stringstream ret = stringstream();
    ret << CC::BLUE << "..." << pad(padding - 3) << "|\t" << CC::GRAY << "...\n" << CC::DEFAULT;
    return ret.str();
}

string ParseError::printLine(size_t lineNumber, string line, size_t padding) {
    stringstream ret = stringstream();
    string lineStr = to_string(lineNumber);
    ret << CC::BLUE << lineStr << pad(padding - lineStr.size()) << "|\t" << CC::GRAY << line << "\n" << CC::DEFAULT;
    return ret.str();
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