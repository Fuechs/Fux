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
#include "../ast/position.hpp"
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
    "Illegal Import"

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

    ParseError();
    ParseError(FlagVec flags, Type type, string title, Metadata subject, string info, Metadata reference, string refInfo, vector<string> notes);
    ~ParseError();

    void report();

    constexpr bool hasFlag(Flag flag);

private:
    FlagVec flags;
    Type type;
        
    string title;
    Metadata subject;
    string info; // info for subject
    Metadata reference;
    string refInfo; // info for reference
    
    vector<string> notes;

    // helper functions for error reporting

    string pad(size_t padding, char fill = ' ');
    string tripleDot(size_t padding);
    string printLine(size_t lineNumber, string line, size_t padding);
    string printSubject(const Metadata &subject, const string &info, size_t padding);
    vector<string> splitString(string data, size_t max);
};