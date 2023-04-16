/**
 * @file error.cpp
 * @author fuechs
 * @brief Fux Compiler Error Implementation
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "error.hpp"

Error::Error(Type type, string title, Subject::Vec subjects) 
: type(type), flag(NEUTRAL), title(title), subjects(subjects) {}

Error::~Error() { 
    title.clear();
    subjects.clear();

    if (flag == NEUTRAL)
        assert(!"Error was neither reported or cancelled.");
}

void Error::report() {
    if (flag == REPORTED)
        assert(!"Error was already reported once.");

    flag = REPORTED;

    if (isWarning(type) && !fux.warnings
    ||  isAggressive(type) && !fux.aggressive)
        return;

    size_t padding = 4;
    for (Subject &subj : subjects) 
        padding = std::max(padding, std::max(subj.meta.fstLine + 4, subj.meta.lstLine + 4));

    stringstream ss;

    if (isWarning(type) && !fux.werrors)    
        ss << SC::BOLD << CC::MAGENTA << "[warning]";
    else                    
        ss << SC::BOLD << CC::RED << "[error]";

    ss << CC::DEFAULT 
        << "[" << (isAggressive(type) ? "EA" : "E") << to_string(type) << "]: "
        << getLiteral(type); 

    if (!title.empty())     
        ss << ": " << title;
        
    ss << ":\n" << SC::RESET;
    
    for (Subject &subj : subjects)
        ss << subj.print();

    if (--fux.errorLimit == 0) {
        ss << CC::RED << SC::BOLD << "\n[fatal error]" << CC::DEFAULT << ": Hit error limit.\n" << SC::RESET;
        cerr << ss.str();
        exit(1);
    } else if (isFatal(type)) {
        ss << CC::RED << SC::BOLD << "\n[fatal error]" << CC::DEFAULT << ": The error above was a fatal error.\n" << SC::RESET;
        cerr << ss.str();
        exit(1);
    }

    cerr << ss.str();
}

void Error::cancel() {
    if (flag == REPORTED)
        assert("Error::cancel(): Cannot cancel an error that was already reported.");

    flag = CANCELLED;
    delete this;
}

string Error::getLiteral(Type type) {
    switch (type) {
        case UNKNOWN:           return "Unknown Error";
        default:                assert(!"Error::getLiteral(): Error Type not implemented.");
    }
}

constexpr bool Error::isWarning(Type type) { return type % 2 != 0; }

constexpr bool Error::isAggressive(Type type) { return type >= 800 && type < 900; }

constexpr bool Error::isFatal(Type type) { return type >= 900; }