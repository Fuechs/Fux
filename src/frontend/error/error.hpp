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
    using Ptr = shared_ptr<Error>;

    // NOTE: 
    //      ID <= 100 is reserved for now
    //      error: ID > 100 && ID < 500 
    //      warning: ID >= 500 && ID < 800
    //      aggressive: ID >= 800 && ID < 900
    //      fatal error: ID >= 900
    enum Type {
        UNKNOWN                 = 0,

        UNEXPECTED_TOKEN        = 101,
        ILLEGAL_NUMBER_FORMAT   = 102,
        ILLEGAL_CHAR_FORMAT     = 103,
        ILLEGAL_STRING_FORMAT   = 104,
        UNKNOWN_CHARACTER       = 105, 
        UNEXPECTED_TYPE         = 106,
        INVALID_CAST            = 107,
        INVALID_TYPE            = 108,
        DUPLICATE_SYMBOL        = 109,
        DUPLICATE_DECL          = 110,
        EXPECTED_LVALUE         = 111, 
        VIOLATED_ACCESS         = 112,
        INVALID_ACCESS          = 113,
        INVALID_CALL            = 114,
        UNKNOWN_SYMBOL          = 115,
        MISSING_PAREN           = 116,
        ILLEGAL_OPERANDS        = 117,

        REDUNDANT_CAST          = 500, 
        RECURSION               = 501,
        USELESS_LIFETIME        = 502,
        REDUNDANT_IMPORT        = 503,
        
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