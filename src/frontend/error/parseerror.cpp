/**
 * @file Error.cpp
 * @author fuechs
 * @brief fux Error
 * @version 0.1
 * @date 2023-01-16
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "Error.hpp"

Error::Error() {
    flags = FlagVec();
    type = UNKNOWN_ERROR;
    title = "";
    subject = Subject();
    reference = Subject();
}

Error::Error(FlagVec flags, Type type, string title, Subject subject, Subject reference) {
    this->flags = flags;
    this->type = type;
    this->title = title;
    this->subject = subject;
    this->reference = reference;
}

Error::~Error() { 
    flags.clear(); 
    title.clear();
}

void Error::report() {
    if (hasFlag(REPORTED) /* TODO: || !fux.options.warnings && hasFlag(WARNING)*/)
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

bool Error::hasFlag(Flag flag) {
    return find(flags.begin(), flags.end(), flag) != flags.end();
}

string Error::printHead() {
    stringstream ss;
    ss << SC::BOLD;
    if (/* TODO: !fux.options.werrors &&*/ hasFlag(WARNING)) 
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