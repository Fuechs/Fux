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
            << ": warning " << err.id << ": " << err.error
        << endl;
    else
        cout
            << filename << ":" << err.line << ":" << err.col
            << ":error " << err.id << ": " << err.error
        << endl;

    cout << "\t" << getLine(err.line) << endl << "\t";

    for (int i = 0; i < err.col -1; i++)
        cout << " ";
    cout << "^" << endl;
}

string ErrorManager::getErrors(list<ParseError> *errors) {
    stringstream errorlist;
    for (ParseError &err : *errors) {
        if (err.warning)
            errorlist 
                << filename << ":" << err.line << ":" << err.col 
                << ": warning " << err.id << ": " << err.error
            << endl;
        else
            errorlist
                << filename << ":" << err.line << ":" << err.col 
                << ": error " << err.id << ": " << err.error
            << endl;

        errorlist << "\t" << getLine(err.line) << endl << "\t";

        for (int i = 0; i < err.col -1; i++)
            errorlist << " ";
        errorlist << "^" << endl;
    }

    return errorlist.str();
}

void ErrorManager::printErrors() {
    if (!asis) {
        if (_err) {
            if (aggressive)
                cout << getErrors(unfilteredErrors);
            else
                cout << getErrors(errors);
        }
    
        cout << getErrors(warnings);
    }
}

int ErrorManager::createNewError(ErrorType err, Token token, string xcmts) {
    keypair<ErrorType, string> kp = getErrorByID(err);
    ParseError e(kp, token, xcmts);
    ParseError lastError = cm ? lastCheckedError : lastError;

    if (shouldReport(NULL, lastError, e)) {
        if (asis)
            printError(e);
        else if (cm) {
            getPossibleErrorList()->push_back(e);
            lastCheckedError = e;
            return 1;
        }

        _err = true;
        errors->push_back(e);
        unfilteredErrors->push_back(e);
        this->lastError = e;
        return 1;
    } else 
        unfilteredErrors->push_back(e);
    
    return 0;
}

bool ErrorManager::shouldReport(Token *token, const ParseError &lastError, const ParseError &e) const {
    if (lastError.error != e.error 
    && lastError.line != e.line 
    && lastError.col != e.col
    && lastError.error.find(e.error) == string::npos
    && !hasError(errors, e)) {
        if (token != NULL
        && token->type != CHAR
        && token->type != STRING
        && token->type != NUMBER)
            return (lastError.error.find(token->value) == string::npos) 
                    && ((lastError.line-e.line) != -1);

        return true;
    }

    return false;
}

bool ErrorManager::shouldReportWarning(Token *token, const ParseError &lastError, const ParseError &e) const {
    if (lastError.error != e.error 
    && lastError.line != e.line 
    && lastError.col != e.col
    && lastError.error.find(e.error) == string::npos
    && !hasError(errors, e)) {
        if (token != NULL
        && token->type != CHAR
        && token->type != STRING
        && token->type != NUMBER)
            return (lastError.error.find(token->value) == string::npos) 
                    && ((lastError.line-e.line) != -1);

        return true;
    }

    return false;
}

void ErrorManager::createNewError(ErrorType err, int l, int c, string xcmts) {
    keypair<ErrorType, string> kp = getErrorByID(err);
    ParseError e(kp, l, c, xcmts);
    ParseError lastError = cm ? lastCheckedError : lastError;

    if (shouldReport(NULL, lastError, e)) {
        if (asis)
            printError(e);
        else if (cm) {
            getPossibleErrorList()->push_back(e);
            lastCheckedError = e;
            return;
        }

        _err = true;
        errors->push_back(e);
        unfilteredErrors->push_back(e);
        this->lastError = e;
    } else 
        unfilteredErrors->push_back(e);
}

void ErrorManager::createNewWarning(ErrorType err, int l, int c, string xcmts) {
    keypair<ErrorType, string> kp = getErrorByID(err);
    ParseError e(kp, l, c, xcmts);
    ParseError lastError;

    if (warnings->size() > 0)
        lastError = *next(warnings->begin(), warnings->size() - 1);
    else
        lastError = cm ? lastCheckedError : lastError;

    if (warnings->size() == 0 || shouldReportWarning(NULL, lastError, e)) {
        if (asis)
            printError(e);

        warnings->push_back(e);
    }
}

string ErrorManager::getLine(int line) {
    if ((line - 1) >= lines.size())
        return "End of File";
    return lines.get(line - 1);
}

keypair<ErrorType, string> ErrorManager::getErrorByID(ErrorType err) {
    return *next(predefinedErrors.begin(), (int) err);
}

bool ErrorManager::hasErrors() {
    return _err && unfilteredErrors->size() != 0;
}

void ErrorManager::enableErrorCheckMode() {
    this->cm = true;
    addPossibleErrorList();
}

list<ParseError> *ErrorManager::getPossibleErrorList() {
    return *next(possibleErrors->begin(), teCursor);
}

void ErrorManager::removePossibleErrorList() {
    if (possibleErrors->size() != 0) {
        list<ParseError> *lst = *next(possibleErrors->begin(), teCursor);
        lst->clear();
        delete lst;
        possibleErrors->pop_back();
        --teCursor;
        if (teCursor < 0) cm = false;
    }
}

void ErrorManager::addPossibleErrorList() {
    possibleErrors->push_back(new list<ParseError>());
    ++teCursor;
}

void ErrorManager::fail() {
    if (possibleErrors->size() > 0) {
        for (ParseError &err : *getPossibleErrorList()) {
            if (shouldReport(NULL, lastError, err)) {
                errors->push_back(err);
                lastError = err;
                unfilteredErrors->push_back(err);
            }
        }

        if (teCursor <= 0) {
            lastError = lastCheckedError;
            _err = true;
        }
    }

    lastCheckedError = ParseError();
    removePossibleErrorList();
}

void ErrorManager::pass() {
    lastCheckedError = ParseError();
    removePossibleErrorList();
}

void ErrorManager::free() {
    cm = false;
    _err = false;
    lastCheckedError.free();
    lastError.free();
    lines.free();
    RuntimeEngine::freeList(*errors);
    RuntimeEngine::freeList(*warnings);
    RuntimeEngine::freeList(*unfilteredErrors);
    for (list<ParseError> *lst : *possibleErrors) {
        RuntimeEngine::freeList(*lst);
        delete lst;
    }
    possibleErrors->clear();
    delete errors; errors = NULL;
    delete warnings; warnings = NULL;
    delete possibleErrors; possibleErrors = NULL;
    delete unfilteredErrors; unfilteredErrors = NULL;
}

bool ErrorManager::hasError(list<ParseError> *e, const ParseError &perror) const {
    for (ParseError &pe : *e)
        if (pe.error == perror.error)
            return true;
    return false;
}

int ErrorManager::createNewError(ErrorType err, AST *pAST, string xcmts) {
    keypair<ErrorType, string> kp = getErrorByID(err);
    ParseError e(kp, pAST->line, pAST->col, xcmts);
    ParseError lastError = cm ? lastCheckedError : lastError;

    if (shouldReport(NULL, lastError, e)) {
        if (asis)
            printError(e);
        else if (cm) {
            getPossibleErrorList()->push_back(e);
            lastCheckedError = e;
            return 1;
        }

        _err = true;
        errors->push_back(e);
        unfilteredErrors->push_back(e);
        this->lastError = e;
        return 1;
    } else 
        unfilteredErrors->push_back(e);
    
    return 0;
}

void ErrorManager::createNewWarning(ErrorType err, AST *pAST, string xcmts) {
    keypair<ErrorType, string> kp = getErrorByID(err);
    ParseError e(kp, pAST->line, pAST->col, xcmts);
    ParseError lastError;

    if (warnings->size() > 0)
        lastError = *next(warnings->begin(), warnings->size() - 1);
    else
        lastError = cm ? lastCheckedError : lastError;

    if (warnings->size() == 0 || shouldReportWarning(NULL, lastError, e)) {
        if (asis)
            printError(e);

        warnings->push_back(e);
    }
}