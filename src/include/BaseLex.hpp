/**
 * @file baselex.hpp
 * @author fuechs (fuechsss@gmail.com)
 * @brief Basic C++ Lexer
 * @version 1.0
 * @date 2022-09-26
 * 
 * @copyright Copyright (c) 2022-present Fuechs
 *            MIT LICENSE
 * 
 */


#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <exception>

namespace BaseLex {

    using std::string, std::cout, std::vector;

    /**
     * @brief Every possible type of token
     * 
     */
    typedef enum TokenType_ENUM {
        LPAREN,         // (
        RPAREN,         // )
        LBRACE,         // {
        RBRACE,         // }
        LBRACKET,       // [
        RBRACKET,       // ]

        LESSTHAN,       // <
        GREATERTHAN,    // >
        COLON,          // :
        COLON_COLON,    // ::
        SEMICOLON,      // ;
        DOT,            // .
        COMMA,          // ,
        EQUALS,         // =
        EQUALS_EQUALS,  // ==
        NOT_EQUALS,     // !=
        PLUS,           // +
        PLUS_PLUS,      // ++
        MINUS,          // -
        MINUS_MINUS,    // --
        ASTERISK,       // *
        ASTERISK_ASTERISK,      // **
        SLASH,          // /
        PLUS_EQUALS,    // +=
        MINUS_EQUALS,   // -=
        SLASH_EQUALS,   // /=
        ASTERISK_EQUALS,// *=
        EXCLAMATION,    // !

        QUESTION,       // ?
        BACKSLASH,      /* \ */
        VERTICAL,       // |
        OR,             // ||
        HASH,           // #
        AMPERSAND,      // &
        AND,            // &&
        AT,             // @
        CARET,          // ^

        LPOINTER,       // <-
        RPOINTER,       // ->
        LSHIFT,         // <<
        RSHIFT,         // >>
        LTRIPLE,        // <<<
        RTRIPLE,        // >>>

        CHARACTER,      // 'c'
        STRING,         // "string"
        INTEGER,        // 0
        FLOAT,          // 0.0
        IDENTIFIER,     // identifier  

        EOS,            // end of source
        EOL,            // end of line
        UNKNOWN,        // initial type
    } TokenType;

    static const char *TokenTypeString[] = {
        "LPAREN", "RPAREN", "LBRACE",
        "RBRACE", "LBRACKET", "RBRACKET",

        "LESSTHAN", "GREATERTHAN", "COLON",
        "COLON_COLON", "SEMICOLON", "DOT",
        "COMMA", "EQUALS", "EQUALS_EQUALS",  
        "NOT_EQUALS", "PLUS", "PLUS_PLUS", 
        "MINUS", "MINUS_MINUS", "ASTERISK", 
        "ASTERISK_ASTERISK", "SLASH", "PLUS_EQUALS", 
        "MINUS_EQUALS", "SLASH_EQUALS", 
        "ASTERISK_EQUALS", "EXCLAMATION",  

        "QUESTION", "BACKSLASH", "VERTICAL",      
        "OR", "HASH", "AMPERSAND", 
        "AND", "AT", "CARET",          

        "LPOINTER", "RPOINTER",
        "LSHIFT", "RSHIFT",  
        "LTRIPLE", "RTRIPLE",    

        "CHARACTER", "STRING",       
        "INTEGER", "FLOAT",          
        "IDENTIFIER",    

        "EOS", "EOL", "UNKNOWN",        
    };

    class Token {
    public:
        Token(TokenType type = UNKNOWN, string value = "") 
        : type(type), value(value) {}

        TokenType type;
        string value;

        /**
         * @brief Exact position of the token for error tracking
         * 
         */
        size_t line = 0;
        size_t col = 0;

        /**
         * @brief Prints out information about the token
         * 
         */
        void debugPrint() {
            cout    << "<Token, "
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

    class Lexer {
    public:
        Lexer(const string input = "", bool detectNewLine = false) 
        : source(input), detectNewLine(detectNewLine) {}

        /**
         * @brief Tokenizes the program source
         * 
         * @param source : Program source
         * @return TokenList : Vector of all tokens collected
         */
        TokenList lex() {

            TokenList tokens;
            Token currentToken;

            currentToken.line = 1;
            currentToken.col = 1;

            while (idx < source.length()) {

                tokenize(currentToken);

                switch (currentToken.type) {
                    
                    // TODO: implement special types

                    case IDENTIFIER: {
                        while (lex_identifier(currentToken) && idx < source.length())
                            advance();
                        endToken(currentToken, tokens);
                        currentToken.col = col;
                        currentToken.line = line;
                        break;
                    }

                    case STRING: {
                        advance();
                        while (current() != '"') {
                            currentToken.value.push_back(current());
                            advance();
                        }
                        endToken(currentToken, tokens);
                        advance();
                        currentToken.col = col;
                        currentToken.line = line;
                        break;
                    }

                    case FLOAT:
                    case INTEGER: {
                        while (isdigit(current())) {
                            currentToken.value.push_back(current());
                            advance();
                        }
                        if (current() == '.') {
                            if (peek() == '.') {
                                std::cerr
                                << "Unexpected second '.' in float definition on Line "
                                << line << ":" << col << " .\n";
                                exit(1);
                            }

                            if (!isdigit(peek())) {
                                std::cerr
                                << "Unexpected character '"
                                << peek()
                                << "' in integer definition on Line "
                                << line << ":" << col << " .\n";
                                exit(1);
                            }

                            if (currentToken.type == FLOAT) {
                                std::cerr
                                << "Unexpected character '.' in float definition on Line "
                                << line << ":" << col << " .\n";
                                exit(1);
                            }

                            currentToken.type = FLOAT;
                            currentToken.value.push_back(current());
                            advance();
                            while (isdigit(current())) {
                                currentToken.value.push_back(current());
                                advance();
                            }
                        }
                        endToken(currentToken, tokens);
                        currentToken.col = col;
                        currentToken.line = line;
                        break;
                    }

                    default: {
                        endToken(currentToken, tokens);
                        currentToken.col = col;
                        currentToken.line = line;
                        break;
                    }

                }

            }

            currentToken.type = EOS;
            currentToken.value = "EOS";
            endToken(currentToken, tokens);

            return tokens;
        
        }

    private:
        const string source;
        size_t idx = 0; // character index in source
        size_t col = 1; // column; resets every newline
        size_t line = 1;

        bool detectNewLine;

        /**
         * @brief returns character in source at an offset
         * 
         * @param offset : provided offset
         * @return char : found character
         * 
         */
        char peek(int offset = 1) {
            return source[idx+offset];
        }

        /**
         * @brief advances the lexer
         * 
         * @param steps : advances N steps
         */
        void advance(int steps = 1) {
            idx += steps;
            col += steps;
        }

        /**
         * @brief resets token position
         * 
         */
        void resetPos() {
            col = 1;
            ++line;
        }

        /**
         * @brief returns current char 
         * 
         * @return char 
         */
        char current() {
            return source[idx];
        }

        /**
         * @brief reads source and detects one token
         * 
         * @param token : current token from Lexer::lex()
         * 
         */
        void tokenize(Token &token) {

            switch (current()) {
                    case '(': {
                        token.type = LPAREN;
                        token.value = "(";
                        advance();
                        break;
                    }

                    case ')': {
                        token.type = RPAREN;
                        token.value = ")";
                        advance();
                        break;
                    }

                    case '{': {
                        token.type = LBRACE;
                        token.value = "{";
                        advance();
                        break;
                    }

                    case '}': {
                        token.type = RBRACE;
                        token.value = "}";
                        advance();
                        break;
                    }

                    case '[': {
                        token.type = LBRACKET;
                        token.value = "[";
                        advance();
                        break;
                    }

                    case ']': {
                        token.type = RBRACKET;
                        token.value = "]";
                        advance();
                        break;
                    }

                    case '<': {
                        if (peek() == '<') {
                            if (peek(2) == '<') {
                                token.type = LTRIPLE;
                                token.value = "<<<";
                                advance(3);
                                break;
                            }

                            token.type = LSHIFT;
                            token.value = "<<";
                            advance(2);
                            break;
                        }

                        if (peek() == '-') {
                            token.type = LPOINTER;
                            token.value = "<-";
                            advance(2);
                            break;
                        }
                        
                        token.type = LESSTHAN;
                        token.value = "<";
                        advance();
                        break;
                    }

                    case '>': {
                        if (peek() == '>') {
                            if (peek(2) == '>') {
                                token.type = RTRIPLE;
                                token.value = ">>>";
                                advance(3);
                                break;
                            }

                            token.type = RSHIFT;
                            token.value = ">>";
                            advance(2);
                            break;
                        }
                        
                        token.type = GREATERTHAN;
                        token.value = ">";
                        advance();
                        break;
                    }

                    case ':': {
                        if (peek() == ':') {
                            token.type = COLON_COLON;
                            token.value = "::";
                            advance(2);
                            break;
                        }
                        
                        token.type = COLON;
                        token.value = ":";
                        advance();
                        break;
                    }

                    case ';': {
                        token.type = SEMICOLON;
                        token.value = ";";
                        advance();
                        break;
                    }

                    case '.': {
                        token.type = DOT;
                        token.value = ".";
                        advance();
                        break;
                    }

                    case ',': {
                        token.type = COMMA;
                        token.value = ",";
                        advance();
                        break;
                    }

                    case '=': {
                        if (peek() == '=') {
                            token.type = EQUALS_EQUALS;
                            token.value = "==";
                            advance(2);
                            break;
                        }
                        
                        token.type = EQUALS;
                        token.value = "=";
                        advance();
                        break;
                    }

                    case '+': {
                        if (peek() == '+') {
                            token.type = PLUS_PLUS;
                            token.value = "++";
                            advance(2);
                            break;
                        }  

                        if (peek() == '=') {
                            token.type = PLUS_EQUALS;
                            token.value = "+=";
                            advance(2);
                            break;
                        }

                        token.type = PLUS;
                        token.value = "+";
                        advance();
                        break;
                    }

                    case '-': {
                        if (peek() == '-') {
                            token.type = MINUS_MINUS;
                            token.value = "--";
                            advance(2);
                            break;
                        }

                        if (peek() == '=') {
                            token.type = MINUS_EQUALS;
                            token.value = "-=";
                            advance(2);
                            break;
                        }

                        if (peek() == '>') {
                            token.type = RPOINTER;
                            token.value = "->";
                            advance(2);
                            break;
                        }

                        token.type = MINUS;
                        token.value = "-";
                        advance();
                        break;
                    }

                    case '/': {
                        if (peek() == '/') {
                            idx += 2;
                            while (current() != '\n')
                                ++idx;
                            ++idx;
                            resetPos();
                            break;
                        } 
                        
                        if (peek() == '=') {
                            token.type = SLASH_EQUALS;
                            token.value = "/=";
                            advance(2);
                            break;
                        }

                        token.type = SLASH;
                        token.value = "/";    
                        advance();                  
                        break;
                    }

                    case '*': {
                        if (peek() == '*') {
                            token.type = ASTERISK_ASTERISK;
                            token.value = "**";
                            advance(2);
                            break;
                        }
                        
                        if (peek() == '=') {
                            token.type = ASTERISK_EQUALS;
                            token.value = "*=";
                            advance(2);
                            break;
                        }

                        token.type = ASTERISK;
                        token.value = "*";
                        advance();
                        break;
                    }

                    case '!': {
                        if (peek() == '=') {
                            token.type = NOT_EQUALS;
                            token.value = "!=";
                            advance(2);
                            break;
                        }

                        token.type = EXCLAMATION;
                        token.value = "!";
                        advance();
                        break;
                    }

                    case '?': {
                        token.type = QUESTION;
                        token.value = "?";
                        advance();
                        break;
                    }

                    case '\\': {
                        token.type = BACKSLASH;
                        token.value = "\\";
                        advance();
                        break;
                    }

                    case '|': {
                        if (peek() == '|') {
                            token.type = OR;
                            token.value = "||";
                            break;
                            advance(2);
                        }

                        token.type = VERTICAL;
                        token.value = "|";
                        advance();
                        break;
                    }

                    case '#': {
                        token.type = HASH;
                        token.value = "#";
                        advance();
                        break;
                    }

                    case '&': {
                        if (peek() == '&') {
                            token.type = AND;
                            token.value = "&&";
                            advance(2);
                            break;
                        }

                        token.type = AMPERSAND;
                        token.value = "&";
                        advance();
                        break;
                    }

                    case '@': {
                        token.type = AT;
                        token.value = "@";
                        advance();
                        break;
                    }

                    case '^': {
                        token.type = CARET;
                        token.value = "^";
                        advance();
                        break;
                    }

                    case '\'': {
                        token.type = CHARACTER;
                        advance();
                        token.value.push_back(current());
                        advance();
                        if (current() != '\'') {
                            // TODO: implement proper errors
                            std::cerr 
                            << "Invalid character expression at Line "
                            << line << ":" << col << ".\n";
                            exit(1);
                        }
                        advance();
                        break;
                    }

                    case '"': {
                        token.type = STRING;
                        advance();
                        token.value.push_back(current());
                        break;
                    }

                    case ' ':
                    case '\t': {
                        advance();
                        break;
                    }

                    case '\n':
                    case '\r': {
                        ++idx;
                        if (detectNewLine) {
                            token.type = EOL;
                            token.value = "\\n";
                        }
                        resetPos();
                        break;
                    }

                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': {
                        token.type = INTEGER;
                        token.value.push_back(current());
                        advance();
                        break;
                    }

                    default: {                        
                        token.type = IDENTIFIER;
                        token.value.push_back(current());
                        advance();
                        break;
                    }

            }
        }

        /**
         * @brief Appends token to TokenList and resets current token
         * 
         * @param token : Token to append
         * @param tokens : TokenList to append to
         * 
         */
        void endToken(Token &token, TokenList &tokens) {
            if (token.type != UNKNOWN) 
                tokens.push_back(token);

            token.type = UNKNOWN;
            token.value.erase();
        }

        /**
         * @brief Lex one identifier
         * 
         * @param token : identifier token
         * @return true : continue lexing identifier
         * @return false : stop lexing identifier
         */
        bool lex_identifier(Token &token) {

            switch (current()) {

                case '(':
                case ')':
                case '{':
                case '}':
                case '[':
                case ']':
                case '<':
                case '>':
                case ':':
                case ';':
                case '.':
                case ',':
                case '=':
                case '+':
                case '-':
                case '*':
                case '/':
                case '!':
                case '?':
                case '\\':
                case '|':
                case '#':
                case '&':
                case '@':
                case '^':
                case '\'':
                case '"':
                    return false;

                default: {
                    token.value.push_back(current());
                    return true;
                }
                
            }

        }

    };

}