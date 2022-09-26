// Created by Fuechs on 2022-09-26

#include <iostream>

#include "include/lexer.hpp"

namespace fux {

    void Token::debugPrint() {

        std::string type;

        switch (mType) {
            case LPAREN:        type = "LPAREN"; break;
            case RPAREN:        type = "RPAREN"; break;
            case LBRACE:        type = "LBRACE"; break;
            case RBRACE:        type = "RBRACE"; break;
            case COLON:         type = "COLON"; break;
            case EQUALS:        type = "EQUALS"; break;
            case DOT:           type = "DOT"; break;
            case SEMICOLON:     type = "SEMICOLON"; break;
            case IDENTIFIER:    type = "IDENTIFIER"; break;
            case INTEGER:       type = "INTEGER"; break;
            case FLOAT:         type = "FLOAT"; break;
            default:            type = "UNKNOWN"; break;
        }

        std::cout << "<TOKEN, " << type << ", '" << mValue << "'>\n";

    }

    #define current mSource[mIdx]

    std::vector<Token> Lexer::lex() {
        mIdx = 0;

        Token token;
        token.mValue = std::to_string(current);

        while (mIdx < mSource.length()) {
            if (isdigit(current))
                token = lex_integer();
            else if (isspace(current)) {
                mIdx++;
                continue;
            }
            else {
                switch (current) {

                    case '(': token.mType = RPAREN; mIdx++; break;
                    case ')': token.mType = LPAREN; mIdx++; break;
                    case '{': token.mType = RBRACE; mIdx++; break;
                    case '}': token.mType = LBRACE; mIdx++; break;
                    case ':': token.mType = COLON; mIdx++; break;
                    case '=': token.mType = EQUALS; mIdx++; break;
                    case '.': token.mType = DOT; mIdx++; break;
                    case ';': token.mType = SEMICOLON; mIdx++; break;

                    case '"': token = lex_string(); break;

                    default: token = lex_identifier(); break;
                }
            }

            mTokens.push_back(token);
        }

        return mTokens;
    }

    Token Lexer::lex_integer() {

        Token token;

        do {
            token.mValue.push_back(current);
            mIdx++;
            if (current == '.') {
                if (token.mType == FLOAT) {
                    std::cerr << "Unexpected character '.' in float definition" << std::endl;
                    exit(1);
                } else if (isdigit(peek())) {
                    token.mType = FLOAT;
                } else {
                    std::cerr << "Unexpected character '.' in integer definition" << std::endl;
                    exit(1);
                }

                token.mValue.push_back(current);
                mIdx++;
            }
        } while (isdigit(current));

        return token;

    }

    Token Lexer::lex_string() {
        Token token;
        token.mType = STRING;

        mIdx++;

        while (current != '"') {
            token.mValue.push_back(current);
            mIdx++;
        }

        return token;
    }

    Token Lexer::lex_identifier() {
        Token token;
        token.mType = IDENTIFIER;
        token.mValue = std::to_string(current);
        mIdx++;

        while (!isspace(current)) {
            token.mValue.push_back(current);
            mIdx++;
        }

        return token;
    }

}