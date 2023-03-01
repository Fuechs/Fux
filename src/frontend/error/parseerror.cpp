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

ParseError::SUBJ_STRCT::SUBJ_STRCT(Metadata meta, string info, string pointerText, size_t pointer) {
    this->meta = meta;
    this->info = info;
    this->pointerText = pointerText;
    this->pointer = pointer;
}

ParseError::SUBJ_STRCT::~SUBJ_STRCT() {
    info.clear();
    pointerText.clear();
}

ParseError::SUBJ_STRCT &ParseError::SUBJ_STRCT::operator=(const SUBJ_STRCT &copy) {
    meta = copy.meta;
    info = copy.info;
    pointer = copy.pointer;
    pointerText = copy.pointerText;
    return *this;
}

ParseError::ParseError() {
    flags = FlagVec();
    type = UNKNOWN_ERROR;
    title = "";
    subject = SUBJ_STRCT();
    reference = SUBJ_STRCT();
    notes = vector<string>();
}

ParseError::ParseError(FlagVec flags, Type type, string title, SUBJ_STRCT subject, SUBJ_STRCT reference, vector<string> notes) {
    this->flags = flags;
    this->type = type;
    this->title = title;
    this->subject = subject;
    this->reference = reference;
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
    padding = to_string(std::max({subject.meta.lstLine, reference.meta.lstLine})).size() + 3;

    ss << printHead();
    ss << printSubject(subject);
    if (hasFlag(REFERENCE))
        ss << printSubject(reference);
    ss << printNotes();

    cerr << ss.str();
}

constexpr bool ParseError::hasFlag(Flag flag) {
    return find(flags.begin(), flags.end(), flag) != flags.end();
}

void ParseError::addNote(string note) { notes.push_back(note); }

string ParseError::pad(size_t sub, char fill) {
    string ret = "";
    for (sub = padding - sub; sub --> 0;)
        ret += fill;
    return ret;
}

string ParseError::tripleDot() {
    stringstream ss;
    ss << CC::BLUE << "..." << pad(3) << "|\t" << CC::GRAY << "...\n" << CC::DEFAULT;
    return ss.str();
}

string ParseError::printSubject(const SUBJ_STRCT &subj) { 
    stringstream ss;
    const Metadata &meta = subj.meta;

    ss << printPosition(meta);
    if (meta.fstLine == meta.lstLine) {
        ss << printLine(meta.fstLine, meta[meta.fstLine]);
        ss << printUnderline(meta.fstCol, meta.lstCol, subj.pointer);
        ss << printInfo(subj.info);
        ss << printArrow(subj);
    } else if (meta.lstLine - meta.fstLine > 6) {
        ss << printLine(meta.fstLine, meta[meta.fstLine]);
        ss << tripleDot();
        ss << printLine(meta.lstLine, meta[meta.lstLine]);
        ss << printInfo(subj.info, true);
    } else {
        for (size_t i = meta.fstLine; i <= meta.lstLine; i++) 
            ss << printLine(i, meta[i]);
        ss << printInfo(subj.info, true);
    }

    return ss.str();
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

string ParseError::printPosition(const Metadata &meta) {
    stringstream ss;
    ss << pad(2) << CC::BLUE << SC::BOLD << ">>> " << SC::RESET
        << *meta.file << ":" << meta.fstLine << ":" << meta.fstCol << "\n";
    return ss.str();
}

string ParseError::printLine(size_t lineNumber, string line) {
    stringstream ss;
    string lineStr = to_string(lineNumber);
    ss << CC::BLUE << SC::BOLD << lineStr << pad(lineStr.size()) << "|\t" << SC::RESET << CC::GRAY << line << "\n" << CC::DEFAULT;
    return ss.str();
}

string ParseError::printUnderline(size_t start, size_t end, size_t except) {
    stringstream ss;
    
    ss << pad() << SC::BOLD << CC::RED << "|\t";

    size_t i;
    size_t max = (except == 0 ? start  : std::min({start, except})) - 1;
    for (i = 1; i < max; i++) 
        ss << " ";
    for (; i <= std::max({end, except}) + 1; i++) {
        if (except == 0) {
            for (;i >= start && i <= end; i++) 
                ss << CC::RED << "^";
            break;
        }

        if (i == except - 1 || i == except + 1)
            ss << " ";
        else if (i == except) 
            ss << CC::RED << "^";
        else if (i >= start && i <= end) 
            ss << CC::BLUE << "-";
        else
            ss << " ";
    }
    
    // for (i = 0; i < (start - 1); i++) // -1 so arrow points at exact position
    //     ss << " ";

    // while (i++ < end) {
    //     if (except == 0) {
    //         ss << "^";
    //         continue;
    //     }

    //     if (i == except - 1 || i == except + 1) 
    //         ss << " ";
    //     else if (i == except) 
    //         ss << CC::RED << "^";
    //     else 
    //         ss << CC::BLUE << "-";
    // }

    ss << SC::RESET << " ";
    return ss.str();
}

string ParseError::printArrow(const SUBJ_STRCT &subj) {
    if (subj.pointer == 0)
        return "";
    
    const Metadata &meta = subj.meta;
    stringstream ss;

    ss << pad() << CC::RED << SC::BOLD << "|\t";
    for (size_t i = 0; i < subj.pointer - 1; i++)
        ss << " ";
    ss << "|\n";
    ss << pad() << "|\t";
    for (size_t i = 0; i != subj.pointer - 1; i++)
        ss << " ";
    ss << subj.pointerText << "\n" << SC::RESET;

    return ss.str();
}

string ParseError::printInfo(const string &info, bool wrap) {
    if (info.empty())
        return "\n";

    stringstream ss;
    ss << SC::BOLD << CC::RED;
    if (wrap) 
        ss << pad() << " \\___ " << info;
    else 
        ss << info;
    ss << "\n" << SC::RESET;
    return ss.str();
}

string ParseError::printNotes() {
    stringstream ss;
    ss << CC::YELLOW << SC::BOLD;
    for (const string &note : notes)
        ss << pad() << "|\t" << note << "\n";
    ss << SC::RESET;
    return ss.str();
}

// vector<string> ParseError::splitString(string data, size_t max) {
//     vector<string> ret = {""};
//     vector<string> tmp = vector<string>();
//     tmp = split(data, ' ');
//     for (string &word : tmp)
//         if (ret.back().size() < max)
//             ret.back() += word + " ";
//         else
//             ret.push_back(word + " ");
//     return ret;
// }