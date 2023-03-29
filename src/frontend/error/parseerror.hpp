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
#include "subject.hpp"

// standard messages for each error type
static const char *ErrorTypeString[] = {
    "Generic",
    "Recursion",
    "Useless Lifetime",

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

    "Implicit Cast",

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
        RECURSION,
        USELESS_LIFETIME,

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
        
        IMPLICIT_CAST,

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
    ParseError(FlagVec flags, Type type, string title, 
        Subject subject, Subject reference = Subject());
    ~ParseError();

    void report();

    bool hasFlag(Flag flag);

private:
    FlagVec flags;
    Type type;
        
    string title;
    Subject subject, reference;
    
    // padding for line numbers
    size_t padding = 5;

    // [error/warning][E-ID]: E-Type: E-Title 
    string printHead();
};