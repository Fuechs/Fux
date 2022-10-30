/**
 * @file error.cpp
 * @author fuechs
 * @brief fux error manager 
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include <sstream>

#include "error.hpp"
#include "../../util/keypair.hpp"
#include "ast.hpp"

void initializeErrors() {
    keypair<ErrorType, string> err;

    err.set(UNEXPECTED_SYMBOL, "unexpected symbol");
    predefinedErrors.push_back(err);

    err.set(ILLEGAL_NUMBER_FORMAT, "illegal number format mismatch");
    predefinedErrors.push_back(err);

    err.set(UNEXPECTED_EOF, "unexpected end of file");
    predefinedErrors.push_back(err);

    err.set(EXPECTED_STRING_LITERAL_EOF, "expected string literal before end of file");
    predefinedErrors.push_back(err);

    err.set(ILLEGAL_STRING_FORMAT, "illegal string format");
    predefinedErrors.push_back(err);

    err.set(EXPECTED_CHAR_LITERAL_EOF, "expected character literal before end of file");
    predefinedErrors.push_back(err);

    err.set(ILLEGAL_CHAR_LITERAL_FORMAT, "illegal character literal format");
    predefinedErrors.push_back(err);

    err.set(GENERIC, "");
    predefinedErrors.push_back(err);

    err.set(ILLEGAL_ACCESS_DECLARATION, "illegal specification of access specifier(s)");
    predefinedErrors.push_back(err);

    err.set(ILLEGAL_BRACKET_MISMATCH, "illegal symbol mismatch, unexpected bracket");
    predefinedErrors.push_back(err);

    err.set(MISSING_BRACKET, "missing bracket");
    predefinedErrors.push_back(err);

    err.set(INVALID_ACCESS_SPECIFIER, "invalid access specifier");
    predefinedErrors.push_back(err);

    err.set(PREVIOUSLY_DEFINED, "");
    predefinedErrors.push_back(err);

    err.set(DUPLICATE_CLASS, "duplicate class:");
    predefinedErrors.push_back(err);

    err.set(REDUNDANT_TOKEN, "redundant token");
    predefinedErrors.push_back(err);

    err.set(INTERNAL_ERROR, "internal runtime error");
    predefinedErrors.push_back(err);

    err.set(COULD_NOT_RESOLVE, "could not resolve symbol");
    predefinedErrors.push_back(err);

    err.set(EXPECTED_REFERENCE_OF_TYPE, "expected reference of type");
    predefinedErrors.push_back(err);

    err.set(INVALID_CAST, "invalid cast of type");
    predefinedErrors.push_back(err);
    
    err.set(REDUNDANT_CAST, "redundant cast of type");
    predefinedErrors.push_back(err);

    err.set(REDUNDANT_IMPORT, "redundant self import of package");
    predefinedErrors.push_back(err);

    err.set(UNEXPECTED_TOKEN, "unexpected token");
    predefinedErrors.push_back(err);

    err.set(INVALID_ACCESS, "invalid access of");
    predefinedErrors.push_back(err);

    err.set(SYMBOL_ALREADY_DEFINED, "");
    predefinedErrors.push_back(err);

    err.set(INVALID_PARAM, "invalid param of type");
    predefinedErrors.push_back(err);

    err.set(INCOMPATIBLE_TYPES, "incompatible types");
    predefinedErrors.push_back(err);

    err.set(DUPLICATE_DECLARATION, "duplicate declaration of");
    predefinedErrors.push_back(err);
}

void ErrorManager::printError(ParseError &err) {
    if (err.warning)
        cout
            << filename << ":" << err.line << ":" << err.col
            << ": warning " << err.id << ": " << err.error.c_str()
        << endl;
    else
        cout
            << filename << ":" << err.line << ":" << err.col
            << ":error " << err.id << ": " << err.error.c_str()
        << endl;

    cout << "\t" << getLine(err.line) << endl << "\t";

    for (int i = 0; i < err.col -1; i++)
        cout << " ";
    cout << "^" << endl;
}