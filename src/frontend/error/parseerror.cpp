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

ParseError::ParseError(FlagVec flags, Type type, string title, 
    Metadata subject, string info, Metadata reference, string refInfo, vector<string> notes) {
    this->flags = flags;
    this->type = type;
    this->title = title;
    this->subject = subject;
    this->info = info;
    this->reference = reference;
    this->refInfo = refInfo;
    this->notes = notes;
}

ParseError::~ParseError() { 
    flags.clear(); 
    title.clear();
}

void ParseError::report() {
    if (hasFlag(REPORTED) /* || !fux.options.warnings && hasFlag(WARNING) */)
        return;
    flags.push_back(REPORTED);

    stringstream ss = stringstream();
    size_t padding = to_string(std::max({subject.lstLine, reference.lstLine})).size() + 3;

    // header 
    ss << SC::BOLD;
    if (/*!fux.options.werrors &&*/ hasFlag(WARNING)) 
        ss << CC::MAGENTA << "[warning]";
    else 
        ss << CC::RED << "[error]";
    ss << CC::DEFAULT << "["; 
    if (hasFlag(AGGRESSIVE))
        ss << "A";
    ss << "E" << type << "]: " << ErrorTypeString[type];
    if (!title.empty())
        ss << ": " << title;
    ss << "\n" << SC::RESET;

    // subjects
    ss << printSubject(subject, info, padding);
    if (hasFlag(REFERENCE))
        ss << printSubject(reference, refInfo, padding);
    for (string &note : notes) 
        ss << pad(padding) << "|>\t" << note << "\n";

    cerr << ss.str();
}

constexpr bool ParseError::hasFlag(Flag flag) {
    return find(flags.begin(), flags.end(), flag) != flags.end();
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

string ParseError::printSubject(const Metadata &subject, const string &info, size_t padding) { 
    stringstream ss;
    // position
    ss << pad(padding - 2) << CC::BLUE << ">>> " << CC::DEFAULT
        << *subject.file << ":" << subject.fstLine << ":" << subject.fstCol << "\n";

    // source and pointers    
    if (subject.fstLine == subject.lstLine) {
        ss << printLine(subject.fstLine, subject[subject.fstLine], padding);

        size_t i;
        ss << pad(padding) << CC::GREEN << "|\t";
        for (i = 0; i < (subject.fstCol - 1); i++) // -1 so arrow points at exact position
            ss << " ";
        while (i++ < subject.lstCol)
            ss << "^";

        if (!info.empty()) {
            if (info.size() > subject[subject.fstLine].size()) {
                vector<string> data = splitString(info, subject[subject.fstLine].size());
                ss << "\n" << pad(padding) << "|-> " << data[0];
                for (size_t i = 1; i < data.size(); i++)
                    ss << "\n" << pad(padding) << "|\t" << data[i];
            } else
                ss << " <-- " << info;
        }

        ss << CC::DEFAULT << "\n";
    } else {
        if (subject.lstLine - subject.fstLine > 6) {
            ss << printLine(subject.fstLine, subject[subject.fstLine], padding);
            ss << tripleDot(padding);
            ss << printLine(subject.lstLine, subject[subject.lstLine], padding);
        } else 
            for (size_t line = subject.fstLine; line < subject.lstLine + 1; line++)
                ss << printLine(line, subject[line], padding);
        
        if (!info.empty()) {
            ss << CC::GREEN;
            if (info.size() > subject[subject.lstLine].size()) {
                vector<string> data = splitString(info, subject[subject.fstLine].size());
                ss << pad(padding) << " \\_ " << data[0];
                for (size_t i = 1; i < data.size(); i++)
                    ss << "\n" << pad(padding) << "|\t" << data[i];
            } else
                ss << pad(padding) << " \\_ " << info;
            ss << "\n" << CC::DEFAULT;
        }
    } 

    return ss.str();
}

vector<string> ParseError::splitString(string data, size_t max) {
    vector<string> ret = {""};
    vector<string> tmp = vector<string>();
    tmp = split(data, ' ');
    for (string &word : tmp)
        if (ret.back().size() < max)
            ret.back() += word + " ";
        else
            ret.push_back(word + " ");
    return ret;
}