/**
 * @file parseerror.hpp
 * @author fuechs
 * @brief fux ParseError header
 * @version 0.1
 * @date 2023-01-16
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../lexer/token.hpp"
#include "../metadata.hpp"

// standard messages for each error type
static const char *ErrorTypeString[] = {
    "Generic",

    "Unexpected Token",
    "Unexpected End of File",
    "Unexpected Parameter",
    "Expected lvalue",

    "Unknown Error",
    "Unknown Character",

    "Illegal Number Format",
    "Illegal Char Literal Format",
    "Illegal String Literal Format",
    "Illegal Type",
    "Illegal Cast",
    "Illegal Access",
    "Illegal Import",
    "Illegal Operands",

    "Redundant Cast",
    "Redundant Token",
    "Redundant Import",

    "Duplicate Symbol",
    "Duplicate Declaration",
    
    "Missing Paren",
};

class ParseError {
public:
    typedef vector<ParseError> Vec;

    enum Type {
        GENERIC,

        UNEXPECTED_TOKEN,
        UNEXPECTED_EOF,
        UNEXPECTED_PARAMETER,
        EXPECTED_LVALUE,

        UNKNOWN_ERROR,
        UNKNOWN_CHARACTER,
        
        ILLEGAL_NUMBER_FORMAT,
        ILLEGAL_CHAR_LITERAL_FORMAT,
        ILLEGAL_STRING_LITERAL_FORMAT,
        ILLEGAL_TYPE,
        ILLEGAL_CAST,
        ILLEGAL_ACCESS,
        ILLEGAL_IMPORT,
        ILLEGAL_OPERANDS,

        REDUNDANT_CAST,
        REDUNDANT_TOKEN,
        REDUNDANT_IMPORT,

        DUPLICATE_SYMBOL,
        DUPLICATE_DECLARATION,

        MISSING_PAREN,
    };

    enum Flag {
        WARNING,        // is a warning
        AGGRESSIVE,     // is aggressive
        REPORTED,       // was reported
        REFERENCE,      // has a reference to another code
    };

    typedef vector<Flag> FlagVec;

    struct SUBJ_STRCT {
        SUBJ_STRCT(Metadata meta = Metadata(), string info = "", string pointerText = "", size_t pointer = 0);
        ~SUBJ_STRCT();

        SUBJ_STRCT &operator=(const SUBJ_STRCT &copy);

        Metadata meta;
        string info;
        string pointerText;
        size_t pointer;
    };

    ParseError();
    ParseError(FlagVec flags, Type type, string title, SUBJ_STRCT subject, SUBJ_STRCT reference = SUBJ_STRCT(), vector<string> notes = {});
    ~ParseError();

    void report();

    constexpr bool hasFlag(Flag flag);
    void addNote(string note);

private:
    FlagVec flags;
    Type type;
        
    string title;
    SUBJ_STRCT subject, reference;
    
    vector<string> notes;

    // helper functions for error reporting
    size_t padding = 3;

    string pad(size_t sub = 0, char fill = ' ');
    string tripleDot();

    string printHead();
    string printSubject(const SUBJ_STRCT &subj);
    string printPosition(const Metadata &meta);
    string printLine(size_t lineNumber, string line);
    string printUnderline(size_t start, size_t end, size_t except = 0);
    string printArrow(const SUBJ_STRCT &meta);
    string printInfo(const string &info, bool wrap = false, bool color = false);
    string printNotes();
};