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
    subject = Subject();
    reference = Subject();
}

ParseError::ParseError(FlagVec flags, Type type, string title, Subject subject, Subject reference) {
    this->flags = flags;
    this->type = type;
    this->title = title;
    this->subject = subject;
    this->reference = reference;
}

ParseError::~ParseError() { 
    flags.clear(); 
    title.clear();
}

void ParseError::report() {
    if (hasFlag(REPORTED) /*|| !fux.options.warnings && hasFlag(WARNING)*/)
        return;
    flags.push_back(REPORTED);

    stringstream ss = stringstream();
    padding = to_string(std::max({subject.meta.lstLine, reference.meta.lstLine})).size() + 4;

    subject.padding = padding;
    reference.padding = padding;

    ss << printHead();           
    ss << subject.print();     
    if (hasFlag(REFERENCE))         
        ss << reference.print();      

    cerr << ss.str();
}

bool ParseError::hasFlag(Flag flag) {
    return find(flags.begin(), flags.end(), flag) != flags.end();
}

string ParseError::printHead() {
    stringstream ss;
    ss << SC::BOLD;
    if (/*!fux.options.werrors &&*/ hasFlag(WARNING)) 
        ss << CC::MAGENTA << "[warning]";
    else 
        ss << CC::RED << "[error]";
    ss << CC::DEFAULT << "["; 
    if (hasFlag(AGGRESSIVE)) ss << "A";
    ss << "E" << type << "]: " << ErrorTypeString[type];
    if (!title.empty()) ss << ": " << title;
    ss << "\n" << SC::RESET;
    return ss.str();
}