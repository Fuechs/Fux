/**
 * @file token.hpp
 * @author fuechs
 * @brief fux token header
 * @version 0.1
 * @date 2022-11-01
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"

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
    POINTER,        // ->
    LSHIFT,         // <<
    RSHIFT,         // >>
    AT,             // @ (this token is not used yet, but reserved for any new features)

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
    LSH_EQUALS,     // <|=
    RSH_EQUALS,     // |>=
    SWAP,           // <>

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
    BIT_NOT,        // ~
    BIT_OR,         // |
    BIT_XOR,        // ><
    BIT_AND,        // &
    BIT_LSHIFT,     // <|
    BIT_RSHIFT,     // |>

    // object
    COLON_COLON,    // ::
    HASH,           // #
    TRIPLE_DOT,     // ...
    ARRAY_BRACKET,  // []
    IDENTIFIER,     // identifier
    NUMBER,         // 0
    FLOAT,          // 0.0 or .0
    HEXADECIMAL,    // 0x0
    OCTAL,          // 0o0
    BINARY,         // 0b0
    STRING,         // "string"
    CHAR,           // 'c'

    KEY_GET,
    KEY_PACKAGE,
    KEY_IF,
    KEY_ELSE,
    KEY_WHILE,
    KEY_DO,
    KEY_FOR,
    KEY_IN,
    KEY_IS,
    KEY_EXCEPT,
    KEY_SWITCH,
    KEY_CASE,
    KEY_DEFAULT,
    KEY_NOT,
    KEY_BREAK,

    KEY_RETURN,
    KEY_THROW,
    KEY_EXIT,
    KEY_PUTS,
    KEY_PUTCH,
    KEY_READ,
    KEY_DELETE,
    KEY_RESERVE,
    KEY_FREE,
    
    KEY_SAFE,
    KEY_INTERN,
    KEY_FINAL,
    KEY_FIXED,
    KEY_ASYNC,

    KEY_USING,
    KEY_TYPEDEF,
    KEY_OPERATOR,
    
    KEY_TRUE,
    KEY_FALSE,
    KEY_NULL,

    KEY_VOID,
    KEY_BOOL,
    KEY_I8,
    KEY_U8,
    KEY_C8,
    KEY_I16,
    KEY_U16,
    KEY_C16,
    KEY_F16,
    KEY_I32,
    KEY_U32,
    KEY_F32,
    KEY_I64,
    KEY_U64,
    KEY_F64,
    KEY_VAR,
    KEY_ENUM,
    KEY_STRUCT,
    KEY_UNION,
    KEY_CLASS,
    KEY_TEMPLATE,
    KEY_MACRO,

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
    "POINTER",        // ->
    "LSHIFT",         // <<
    "RSHIFT",         // >>
    "AT",

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
    "LHS_EQUALS", 
    "RHS_EQUALS",
    "SWAP",           // <>

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
    "BIT_NOT",     
    "BIT_OR",         // |
    "BIT_XOR",
    "BIT_AND",        // &
    "BIT_LSHIFT",
    "BIT_RSHIFT",

    // object
    "COLON_COLON",    // ::
    "HASH",
    "TRIPLE_DOT",     // ...
    "ARRAY_BRACKET",  // []
    "IDENTIFIER",     // identifier
    "NUMBER",         // 0, 0.0, .0
    "FLOAT",
    "HEXADECIMAL",    // 0x0
    "OCTAL",
    "BINARY",
    "STRING",         
    "CHAR",

    "KEY_GET",
    "KEY_PACKAGE",
    "KEY_IF",
    "KEY_ELSE",
    "KEY_WHILE",
    "KEY_DO",
    "KEY_FOR",
    "KEY_IN",
    "KEY_IS",
    "KEY_EXCEPT",
    "KEY_SWITCH",
    "KEY_CASE",
    "KEY_DEFAULT",
    "KEY_NOT",
    "KEY_BREAK",

    "KEY_RETURN",
    "KEY_THROW",
    "KEY_EXIT",
    "KEY_PUTS",
    "KEY_PUTCH",
    "KEY_READ",
    "KEY_DELETE",
    "KEY_RESERVE",
    "KEY_FREE",
    
    "KEY_SAFE",
    "KEY_INTERN",
    "KEY_FINAL",
    "KEY_FIXED",
    "KEY_ASYNC",

    "KEY_USING",
    "KEY_TYPEDEF",
    "KEY_OPERATOR",
    
    "KEY_TRUE",
    "KEY_FALSE",
    "KEY_NULL",

    "KEY_VOID",
    "KEY_BOOL",
    "KEY_I8",
    "KEY_U8",
    "KEY_C8",
    "KEY_I16",
    "KEY_U16",
    "KEY_C16",
    "KEY_F16",
    "KEY_I32",
    "KEY_U32",
    "KEY_F32",
    "KEY_I64",
    "KEY_U64",
    "KEY_F64",
    "KEY_VAR",
    "KEY_ENUM",
    "KEY_STRUCT",
    "KEY_UNION",
    "KEY_CLASS",
    "KEY_TEMPLATE",
    "KEY_MACRO",

    "EOF",           // end of file
    "NONE",
};

static StringVec TokenTypeValue = {
    // structure
    "(",     
    ")",        
    "{",       
    "}",        
    "[",   
    "]",   
    ":",      
    ";",     
    ".",           
    ",",         
    "->",
    "<<",      
    ">>",       
    "@",

    // assignment
    "=",       
    "===",  
    "++",     
    "--",    
    "+=",    
    "-=",   
    "*=",
    "/=",  
    "^=",   
    "%=", 
    "|=",      
    "&=",     
    "<|=",
    "=|>",
    "<>", 

    // condition
    "==",  
    "!=",    
    "<",    
    ">", 
    "<=",      
    ">=",       
    "!",   
    "?",       
    "||",     
    "&&",          

    // binary operation
    "+",         
    "-",        
    "*",       
    "/",           
    "^",         
    "%",       

    // bitwise operation
    "~",
    "|",   
    "><",      
    "&",       
    "<|",
    "|>",

    // object
    "::",    
    "#",   
    "...",     
    "[]",   
    "ident",    
    "0",         
    "0.0", 
    "0x0",   
    "0o0",
    "0b0",
    "\"string\"",
    "'c'",

    "get",
    "package",
    "if",
    "else",
    "while",
    "do",
    "for",
    "in",
    "is",
    "except",
    "switch",
    "case",
    "default",
    "not",
    "break",

    "return",
    "throw",
    "exit",
    "puts",
    "putch",
    "read",
    "delete",
    "reserve",
    "free",
    
    "safe",
    "intern",
    "final",
    "fixed",
    "async",

    "using",
    "typedef",
    "operator",
    
    "true",
    "false",
    "null",

    "void",
    "bool",
    "i8",
    "u8",
    "c8",
    "i16",
    "u16",
    "c16",
    "f16",
    "i32",
    "u32",
    "f32",
    "i64",
    "u64",
    "f64",
    "var",
    "enum",
    "struct",
    "union",
    "class",
    "template",
    "macro",

    "end of file",        
    "none",
};

class Token {
public:
    using Vec = vector<Token>;
    using Iter = Vec::iterator;

    Token(TokenType type = NONE, string value = "");
    ~Token();

    Token &operator=(const Token &copy);
    bool operator==(TokenType type) const;
    bool operator!=(TokenType type) const;

    TokenType type;
    string value;
    size_t line, start, end;

    string str() const;

    bool isKeyword() const;
    bool isType() const;
    bool isModifier() const;
    bool isRelational() const;
    bool isAssignment() const;
    bool isInbuiltCall() const;
};