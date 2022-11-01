/**
 * @file token.hpp
 * @author fuechs
 * @brief fux token header
 * @version 0.1
 * @date 2022-11-01
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "../../../../include.hpp"

enum TokenType {

    // structure
    LPAREN,         // (
    RPAREN,         // )
    LBRACE,         // {
    RBRACE,         // }
    LBRACKET,       // [
    RBRACKET,       // ]
    COLON,          // :
    SEMICOLON,      // ;
    DOT,            // .
    COMMA,          // ,
    RPOINTER,       // ->
    LSHIFT,         // <<
    RSHIFT,         // >>

    // assignment
    EQUALS,         // =
    TRIPLE_EQUALS,  // ===
    PLUS_PLUS,      // ++
    MINUS_MINUS,    // --
    PLUS_EQUALS,    // +=
    MINUS_EQUALS,   // -=
    ASTERISK_EQUALS,// *=
    SLASH_EQUALS,   // /=
    CARET_EQUALS,   // ^=
    PERCENT_EQUALS, // %=
    OR_EQUALS,      // |=
    AND_EQUALS,     // &=

    // condition
    EQUALS_EQUALS,  // ==
    NOT_EQUALS,     // !=
    LESSTHAN,       // <
    GREATERTHAN,    // >
    LTEQUALS,       // <=
    GTEQUALS,       // >=
    EXCLAMATION,    // !
    QUESTION,       // ?
    OR,             // ||
    AND,            // &&

    // binary operation
    PLUS,           // +
    MINUS,          // -
    ASTERISK,       // *
    SLASH,          // /
    CARET,          // ^
    PERCENT,        // %

    // bitwise operation
    BIT_OR,         // |
    BIT_AND,        // &

    // object
    COLON_COLON,    // ::
    TRIPLE_DOT,     // ...
    IDENTIFIER,     // identifier
    NUMBER,         // 0, 0.0, .0
    STRING,         // "string"
    CHAR,           // 'c'

    _EOF,           // end of file
    NONE,
};

static const char *TokenTypeString[] = {
    // structure
    "LPAREN",         // (
    "RPAREN",         // )
    "LBRACE",         // {
    "RBRACE",         // }
    "LBRACKET",       // [
    "RBRACKET",       // ]
    "COLON",          // :
    "SEMICOLON",      // ;
    "DOT",            // .
    "COMMA",          // ,
    "RPOINTER",       // ->
    "LSHIFT",         // <<
    "RSHIFT",         // >>

    // assignment
    "EQUALS",         // =
    "TRIPLE_EQUALS",  // ===
    "PLUS_PLUS",      // ++
    "MINUS_MINUS",    // --
    "PLUS_EQUALS",    // +=
    "MINUS_EQUALS",   // -=
    "ASTERISK_EQUALS",// *=
    "SLASH_EQUALS",   // /=
    "CARET_EQUALS",   // ^=
    "PERCENT_EQUALS", // %=
    "OR_EQUALS",      // |=
    "AND_EQUALS",     // &=

    // condition
    "EQUALS_EQUALS",  // ==
    "NOT_EQUALS",     // !=
    "LESSTHAN",       // <
    "GREATERTHAN",    // >
    "LTEQUALS",       // <=
    "GTEQUALS",       // >=
    "EXCLAMATION",    // !
    "QUESTION",       // ?
    "OR",             // ||
    "AND",            // &&

    // binary operation
    "PLUS",           // +
    "MINUS",          // -
    "ASTERISK",       // *
    "SLASH",          // /
    "CARET",          // ^
    "PERCENT",        // %

    // bitwise operation
    "BIT_OR",         // |
    "BIT_AND",        // &

    // object
    "COLON_COLON",    // ::
    "TRIPLE_DOT",     // ...
    "IDENTIFIER",     // identifier
    "NUMBER",         // 0, 0.0, .0
    "STRING",         
    "CHAR",

    "EOF",           // end of file
    "NONE",
};

static const char *TokenTypeValue[] = {
    // structure
    "(",         // (
    ")",         // )
    "{",         // {
    "}",         // }
    "[",       // [
    "]",       // ]
    ":",          // :
    ";",      // ;
    ".",            // .
    ",",          // ,
    "->",       // ->
    "<<",         // <<
    ">>",         // >>

    // assignment
    "=",         // =
    "===",  // ===
    "++",      // ++
    "--",    // --
    "+=",    // +=
    "-=",   // -=
    "*=",// *=
    "/=",   // /=
    "^=",   // ^=
    "%=", // %=
    "|=",      // |=
    "&=",     // &=

    // condition
    "==",  // ==
    "!=",     // !=
    "<",       // <
    ">",    // >
    "<=",       // <=
    ">=",       // >=
    "!",    // !
    "?",       // ?
    "||",             // ||
    "&&",            // &&

    // binary operation
    "+",           // +
    "-",          // -
    "*",       // *
    "/",          // /
    "^",          // ^
    "%",        // %

    // bitwise operation
    "|",         // |
    "&",        // &

    // object
    "::",    // ::
    "...",     // ...
    "identifier",     // identifier
    "number",         // 0, 0.0, .0
    "string",
    "char",

    "eof",           // end of file
    "none",
};

class Token {
public:
    Token(TokenType type = NONE, string value = "")
    : type(type), value(value), line(0), col(0) {}

    Token(TokenType type = NONE, string value = "", size_t line = 0, size_t col = 0)
    : type(type), value(value), line(line), col(col) {}

    ~Token() { value.clear(); }

    TokenType type;
    string value;
    size_t line, col;

    void debugPrint() {
        cout    
            << "<Token, "
            << TokenTypeString[type]
            << ", '" 
            << value 
            << "', "
            << "Line "
            << line
            << ":"
            << col
            << ">\n";
    }
};

typedef vector<Token> TokenList;
typedef TokenList::iterator TokenIter;