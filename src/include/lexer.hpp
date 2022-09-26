// Created by Fuechs on 2022-09-26

#pragma once

#include <string>
#include <vector>

namespace fux {

    typedef enum {
        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,
        COLON,
        EQUALS,
        DOT,
        SEMICOLON,
        IDENTIFIER,
        INTEGER,
        FLOAT,
        STRING,
        UNKNOWN,
    } TokenType;

    class Token {
    public:
        Token(TokenType type = UNKNOWN, std::string value = "") 
        : mType(type), mValue(value) {}

        void debugPrint();

        TokenType mType;
        std::string mValue;
    };

    class Lexer {
    public:
        Lexer(std::string source = "") : mSource(source) {}

        std::vector<Token> lex();

    private:
        std::string mSource;
        size_t mIdx;
        std::vector<Token> mTokens;

        Token lex_integer();
        Token lex_string();
        Token lex_identifier();

        char peek(int offset = 1) {
            return mSource[mIdx+offset];
        }
    };  

}