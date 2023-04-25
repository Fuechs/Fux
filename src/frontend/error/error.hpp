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
    //      ID < 100 is reserved for now
    //      error: ID >= 100 && ID < 500 
    //      warning: ID >= 500 && ID < 800
    //      aggressive: ID >= 800 && ID < 900
    //      fatal error: ID >= 900
    enum Type {
        UNKNOWN                 = 0,

        UNEXPECTED_TOKEN        = 100,
        ILLEGAL_NUMBER_FORMAT,
        ILLEGAL_CHAR_FORMAT,
        ILLEGAL_STRING_FORMAT,
        UNKNOWN_CHARACTER, 
        UNEXPECTED_TYPE,
        INVALID_CAST,
        INVALID_TYPE,
        DUPLICATE_SYMBOL,
        DUPLICATE_DECL,
        EXPECTED_LVALUE, 
        VIOLATED_ACCESS,
        INVALID_ACCESS,
        INVALID_CALL,
        UNKNOWN_SYMBOL,
        MISSING_PAREN,
        ILLEGAL_OPERANDS,
        EXCEEDED_LIFETIME,

        REDUNDANT_CAST          = 500,
        RECURSION,
        USELESS_LIFETIME,
        REDUNDANT_IMPORT,
        UNREACHABLE,
        
        IMPLICIT_CAST           = 800,
    };

    enum Flag {
        REPORTED,
        NEUTRAL,
        CANCELLED,
    };

    Error(Type type = UNKNOWN, string title = "", Subject::Ptr subject = nullptr);
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
    Subject::Ptr subject;
};