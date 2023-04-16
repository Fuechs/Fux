/**
 * @file error.hpp
 * @author fuechs
 * @brief Fux Compiler Error Header
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "subject.hpp"

class Error {
public:
    // NOTE: 
    //      ID <= 100 is reserved for special IDs
    //      error: ID % 2 == 0
    //      warning: ID % 2 != 0
    //      aggressive: ID >= 800 && ID < 900
    //      fatal error: ID >= 900
    enum Type {
        UNKNOWN                 = 0,

        UNEXPECTED_TOKEN        = 101,
        REDUNDANT_CAST          = 102, 
        ILLEGAL_NUMBER_FORMAT   = 103,
        RECURSION               = 104,
        ILLEGAL_CHAR_FORMAT     = 105,
        USELESS_LIFETIME        = 106,
        ILLEGAL_STRING_FORMAT   = 107,
        REDUNDANT_IMPORT        = 108,
        UNKNOWN_CHARACTER       = 109, 
        UNEXPECTED_TYPE         = 111,
        INVALID_CAST            = 113,
        INVALID_TYPE            = 115,
        DUPLICATE_SYMBOL        = 117,
        DUPLICATE_DECL          = 119,
        EXPECTED_LVALUE         = 121, 
        VIOLATED_ACCESS         = 123,
        INVALID_ACCESS          = 125,
        INVALID_CALL            = 127,
        UNKNOWN_SYMBOL          = 129,
        MISSING_PAREN           = 131,
        ILLEGAL_OPERANDS        = 133,
        IMPLICIT_CAST           = 800,
    };

    enum Flag {
        REPORTED,
        NEUTRAL,
        CANCELLED,
    };

    Error(Type type = UNKNOWN, string title = "", Subject::Vec subjects = {});
    ~Error();

    void report();
    void cancel();

    static string getLiteral(Type type);
    static constexpr bool isWarning(Type type);
    static constexpr bool isAggressive(Type type);
    static constexpr bool isFatal(Type type);

private:
    Type type;
    Flag flag;
    string title;
    Subject::Vec subjects;
};