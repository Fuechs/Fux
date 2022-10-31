/**
 * @file tokentype.hpp
 * @author fuechs
 * @brief fux token type header
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

enum TokenType {

        // structure
        LPAREN,         // (
        RPAREN,         // )
        LBRACE,         // {
        RBRACE,         // }
        LBRACKET,       // [
        RBRACKET,       // ]
        SEMICOLON,      // ;
        DOT,            // .
        COMMA,          // ,
        COLON,          // :
        RPOINTER,       // ->
        LSHIFT,         // <<
        RSHIFT,         // >>

        // asignment
        EQUALS,         // =
        TRIPLE_EQUALS,  // ===
        PLUS_PLUS,      // ++
        MINUS_MINUS,    // --
        PLUS_EQUALS,    // +=
        MINUS_EQUALS,   // -=
        SLASH_EQUALS,   // /=
        ASTERISK_EQUALS,// *=
        PERCENT_EQUALS, // %=
        CARET_EQUALS,   // ^=
        BIT_OR_EQUALS,  // |=
        BIT_AND_EQUALS, // &=

        // condition
        LESSTHAN,       // <
        GREATERTHAN,    // >
        EQUALS_EQUALS,  // ==
        NOT_EQUALS,     // !=
        LTE,            // <=
        GTE,            // >=
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
    
        // objects
        SINGLE_QUOTE,   // '
        DOUBLE_QUOTE,   // "
        NUMBER,         // 0
        LETTER,         // a
        UNDERSCORE,     // _
        COLON_COLON,    // ::
        TRIPLE_DOT,     // ...

        _EOF,           // end of file
        _EOL,           // end of line
        NONE,           // no type
};