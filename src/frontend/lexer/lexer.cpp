/**
 * @file lexer.cpp
 * @author fuechs
 * @brief fux lexer 
 * @version 0.1
 * @date 2022-11-01
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "lexer.hpp"

Lexer::Lexer(const string source, const string &fileName) 
: fileName(fileName), source(source), tokens({}), currentToken(Token()), idx(0), col(1), line(1) {
    parseLines(); 
}

Lexer::~Lexer() {
    tokens.clear();
    source.clear();
}

Token::Vec Lexer::lex() {
    while (idx < source.length()) {
        getToken();
        currentToken.end = col - 1;
        endToken();
        // update line and column for next token
        currentToken.start = col;
        currentToken.line = line; 
    }

    currentToken.type = _EOF;
    currentToken.value = TokenTypeValue[_EOF];
    currentToken.end = col + 1;
    endToken();

    return tokens;
}


StringVec Lexer::parseLines() {
    StringVec ret = {};
    stringstream line;

    for (int i = 0; i < source.length(); i++) {
        if (source[i] == '\n') {
            ret.push_back(line.str());
            line.str("");
        } else
            line << source[i];
    }

    if (!line.str().empty())
        ret.push_back(line.str());
    
    return ret;
}

char Lexer::peek(int offset) {
    if ((idx + offset - 1) < source.length())
        return source[idx+offset];
    return '\0';
}

void Lexer::advance(int steps) {
    idx += steps;
    col += steps;
}

void Lexer::resetPos() {
    col = 1;
    ++line;
}

char Lexer::current() {
    return source[idx];
}

void Lexer::getToken() {
    currentToken.value = current();

    switch (currentToken.value.c_str()[0])  {

        case '(': 
            currentToken.type = LPAREN;
            advance();
            break;

        case ')': 
            currentToken.type = RPAREN;
            advance();
            break;
        
        case '{': 
            currentToken.type = LBRACE;
            advance();
            break;
        
        case '}': 
            currentToken.type = RBRACE;
            advance();
            break;
        
        case '[': 
            if (peek() == ']') {
                currentToken.type = ARRAY_BRACKET;
                currentToken.value = "[]";
                advance(2);
                break;
            }
            
            currentToken.type = LBRACKET;
            advance();
            break;

        case ']': 
            currentToken.type = RBRACKET;
            advance();
            break;

        case ':':
            if (peek() == ':') {
                currentToken.type = COLON_COLON;
                currentToken.value = "::";
                advance(2);
                break;
            }

            currentToken.type = COLON;
            advance();
            break;
        
        case ';':
            currentToken.type = SEMICOLON;
            advance();
            break;
        
        case '.':
            if (isdigit(peek())) {
                currentToken.type = FLOAT;
                advance(); // advance once, getNumber() will parse digits
                break;
            }

            if (peek() == '.' && peek(2) == '.') {
                currentToken.type = TRIPLE_DOT;
                currentToken.value = "...";
                advance(3);
                break;
            }

            currentToken.type = DOT;
            advance();
            break;
        
        case ',':
            currentToken.type = COMMA;
            advance();
            break;
        
        case '=':
            if (peek() == '=') {
                if (peek(2) == '=') {
                    currentToken.type = TRIPLE_EQUALS;
                    currentToken.value = "===";
                    advance(3);
                    break;
                }

                currentToken.type = EQUALS_EQUALS;
                currentToken.value = "==";
                advance(2);
                break;
            }
            currentToken.type = EQUALS;
            advance();
            break;
        
        case '<':
            if (peek() == '<') {
                currentToken.type = LSHIFT;
                currentToken.value = "<<";
                advance(2);
                break;
            }

            if (peek() == '=') {
                currentToken.type = LTEQUALS;
                currentToken.value = "<=";
                advance(2);
                break;
            }

            if (peek() == '>') {
                currentToken.type = SWAP;
                currentToken.value = "<>";
                advance(2);
                break;
            }
            
            if (peek() == '|') {
                if (peek(2) == '=') {
                    currentToken.type = LSH_EQUALS;
                    currentToken.value = "<|=";
                    advance(3);
                    break;
                } 

                currentToken.type = BIT_LSHIFT;
                currentToken.value = "<|";
                advance(2);
                break;
            }

            currentToken.type = LESSTHAN;
            advance();
            break;
        
        case '>':
            if (peek() == '>') {
                currentToken.type = RSHIFT;
                currentToken.value = ">>";
                advance(2);
                break;
            }

            if (peek() == '=') {
                currentToken.type = GTEQUALS;
                currentToken.value = ">=";
                advance(2);
                break;
            }

            if (peek() == '<') {
                currentToken.type = BIT_XOR;
                currentToken.value = "><";
                advance(2);
                break;
            }

            currentToken.type = GREATERTHAN;
            advance();
            break;

        case '@':
            currentToken.type = AT;
            advance();
            break;
        
        case '!':
            if (peek() == '=') {
                currentToken.type = NOT_EQUALS;
                currentToken.value = "!=";
                advance(2);
                break;
            }

            currentToken.type = EXCLAMATION;
            advance();
            break;
        
        case '?':
            currentToken.type = QUESTION;
            advance();
            break;
        
        case '+':
            if (peek() == '+') {
                currentToken.type = PLUS_PLUS;
                currentToken.value = "++";
                advance(2);
                break;
            }

            if (peek() == '=') {
                currentToken.type = PLUS_EQUALS;
                currentToken.value = "+=";
                advance(2);
                break;
            }

            currentToken.type = PLUS;
            advance();
            break;
        
        case '-':
            if (peek() == '-') {
                currentToken.type = MINUS_MINUS;
                currentToken.value = "--";
                advance(2);
                break;
            }

            if (peek() == '=') {
                currentToken.type = MINUS_EQUALS;
                currentToken.value = "-=";
                advance(2);
                break;
            }

            if (peek() == '>') {
                currentToken.type = POINTER;
                currentToken.value = "->";
                advance(2);
                break;
            }

            currentToken.type = MINUS;
            advance();
            break;
        
        case '*':
            if (peek() == '=') {
                currentToken.type = ASTERISK_EQUALS;
                currentToken.value = "*=";
                advance(2);
                break;
            }

            currentToken.type = ASTERISK;
            advance();
            break;
        
        case '/':
            if (skipComment())
                break;

            if (peek() == '=') {
                currentToken.type = SLASH_EQUALS;
                currentToken.value = "/=";
                advance(2);
                break;
            }

            currentToken.type = SLASH;
            advance();
            break;
        
        case '^':
            if (peek() == '=') {
                currentToken.type = CARET_EQUALS;
                currentToken.value = "^=";
                advance(2);
                break;
            }

            currentToken.type = CARET;
            advance();
            break;
        
        case '%':
            if (peek() == '=') {
                currentToken.type = PERCENT_EQUALS;
                currentToken.value = "%=";
                advance(2);
                break;
            }

            currentToken.type = PERCENT;
            advance();
            break;

        case '~':
            currentToken.type = BIT_NOT;
            advance();
            break;
    
        case '|':
            if (peek() == '|') {
                currentToken.type = OR;
                currentToken.value = "||";
                advance(2);
                break;
            }

            if (peek() == '=') {
                currentToken.type = OR_EQUALS;
                currentToken.value = "|=";
                advance(2);
                break;
            }
            
            if (peek() == '>') {
                if (peek(2) == '=') {
                    currentToken.type = RSH_EQUALS;
                    currentToken.value = "|>=";
                    advance(3);
                    break;
                }
                
                currentToken.type = BIT_RSHIFT;
                currentToken.value = "|>";
                advance(2);
                break;
            }

            currentToken.type = BIT_OR;
            advance();
            break;

        case '&':
            if (peek() == '&') {
                currentToken.type = AND;
                currentToken.value = "&&";
                advance(2);
                break;
            }

            if (peek() == '=') {
                currentToken.type = AND_EQUALS;
                currentToken.value = "&=";
                advance(2);
                break;
            }

            currentToken.type = BIT_AND;
            advance();
            break;

        case '#':
            if (skipComment())
                break;

            currentToken.type = HASH;
            advance();
            break;
        
        case ' ':
        case '\t':
        case '\v':
        case '\r':
            advance();
            break;
        
        case '\n':
            advance();
            resetPos();
            break;
        
        case '0'...'9':
            currentToken.type = NUMBER;
            advance();
            getNumber();
            break;
        
        case '\'':
            currentToken.type = CHAR;
            advance();
            currentToken.value = current();
            if (current() == '\\') { // get escape sequences
                advance();
                currentToken.value.push_back(current());
            }
            advance();
            // FIXME:
            // if (current() != '\'')
            //     error->plainError(Error::ILLEGAL_CHAR_LITERAL_FORMAT, "Expected ending quote after char literal",
            //         fileName, {error->createUL(line, currentToken.start, col, col),
            //             error->createPtr(line, col, "Expected a single quote \"'\" here")});
            // else 
                advance();
            break;
         
        case '"':
            currentToken.type = STRING;
            getString();
            break;
        
        default:
            if (!isalpha(current()) && currentToken.value != "_") {
                // FIXME:
                // error->simpleError(Error::UNKNOWN_CHARACTER, "Encountered an unknown character",
                //     Metadata(fileName), line, col, col, 
                //     "The character '"+currentToken.value+"' is unknown to the lexer");
                // advance();
            } else {
                currentToken.type = IDENTIFIER;
                getIdentifier();
                checkKeyword();
            }
            break;

    }
}

void Lexer::endToken() {
    if (currentToken.type != NONE)
        tokens.push_back(currentToken);
    
    currentToken.type = NONE;
    currentToken.value.erase();
}

void Lexer::getIdentifier() {
    bool hasLetter = false;
    currentToken.value.clear(); // we have to check wether first character is alphabetic

    do {
        if (isalpha(current()))
            hasLetter = true;
        currentToken.value.push_back(current());
        advance();
    } while ((isalpha(current()) || isdigit(current()) || current() == '_') && idx < source.length());

    // FIXME:
    // if (!hasLetter)
    //     error->simpleError(Error::GENERIC, "Invalid identifier format", 
    //         Metadata(fileName), line, currentToken.start, col, 
    //         "Expected at least one alphabetic character in identifier");
}

void Lexer::getString() {
    currentToken.value.clear(); // remove '"'
    advance();

    // FIXME:
    // if (idx >= source.length())
    //     error->plainError(Error::ILLEGAL_STRING_LITERAL_FORMAT, "Unterminated string", 
    //         fileName, {error->createUL(currentToken.line, currentToken.start, col, col, "Literal was not terminated before end of file"),
    //             error->createPtr(currentToken.line, col, "Expected a double quote '\"' here")});
    
    while (current() != '"') {
        if (current() == '\n') {
            // FIXME:
            // error->plainError(Error::ILLEGAL_STRING_LITERAL_FORMAT, "Unterminated string", fileName, 
            //     {error->createUL(line, currentToken.start, col, col, "Literal was not terminated before end of line"),
            //         error->createPtr(line, col, "Expected a double quote '\"' here")});
            return;
        }

        if (current() == '\\' & peek() != '\\' && !isalpha(peek())) {
            // FIXME:
            // error->plainError(Error::ILLEGAL_STRING_LITERAL_FORMAT, "Invalid escape sequence found in string literal",
            //     fileName, {error->createUL(line, currentToken.start, col, col),
            //         error->createPtr(line, col, "Invalid escape sequence found here")});
            return;
        }

        currentToken.value.push_back(current());
        advance();

        if (idx >= source.length()) {
            // FIXME:
            // error->plainError(Error::ILLEGAL_STRING_LITERAL_FORMAT, "Unterminated String", fileName,
            //     error->createMark(currentToken.line, line, currentToken.start, col, "Literal was not terminated before end of file",
            //         col, "Expected a double quote '\"' here"));
            return;
        }
    } 

    advance();
}


void Lexer::getNumber() {

    /*
        0x --> Hexadecimal
        .. --> Decimal
        0o --> Octal 
        0b --> Binary
    */

    #define LENOK idx < source.length()
    #define ISHEX (isdigit(current()) || (tolower(current()) >= 'a' && tolower(current()) <= 'f'))
    #define ISOCT (current() >= '0' && current() <= '7')
    #define ISBIN (current() == '0' || current() == '1')


    if (currentToken.value != "0")
        goto decimal;

    if (current() == 'x') {
        currentToken.type = HEXADECIMAL;
        currentToken.value = "0x";
        advance();
        // extra if for error tracking and not allowing '0x_'
        if (ISHEX && LENOK) {
            currentToken.value.push_back(current());
            advance();
        } // FIXME: else
            // error->plainError(Error::ILLEGAL_NUMBER_FORMAT, "Expected at least one digit in hexadecimal number",
            //     fileName, {error->createUL(line, currentToken.start, col, col, "Expected at least one digit after '0x'"),
            //         error->createPtr(line, col, "Expected at least one digit (0-9, a-f, A-F) here")});
        
        while ((ISHEX || current() == '_') && LENOK) {
            if (current() != '_')
                currentToken.value.push_back(current());
            advance();
        }

        return;
    } else if (current() == 'o') {
        currentToken.type = OCTAL;
        currentToken.value = "0o";
        advance();

        if (ISOCT && LENOK) {
            currentToken.value.push_back(current());
            advance();
        } // FIXME: else
        //     error->plainError(Error::ILLEGAL_NUMBER_FORMAT, "Expected at least one digit in octal number", 
        //         fileName, {error->createUL(line, currentToken.start, col, col, "Expected at least one digit after '0o'"),
        //             error->createPtr(line, col, "Expected at least one digit (0-7) here")});

        while ((ISOCT || current() == '_') && LENOK) {
            if (current() != '_')
                currentToken.value.push_back(current());
            advance();
        }

        return;
    } else if (current() == 'b') {
        currentToken.type = BINARY;
        currentToken.value = "0b";
        advance();

        if (ISBIN || current() == '_' && LENOK) {
            if (current() != '_')
                currentToken.value.push_back(current());
            advance();
        } // FIXME: else
            // error->plainError(Error::ILLEGAL_NUMBER_FORMAT, "Expected at least one digit in binary number",
            //     fileName, {error->createUL(line, currentToken.start, col, col, "Expected at least one digit after '0o'"),
            //         error->createPtr(line, col, "Expected at least one digit (0-1) here")});

        while ((ISBIN || current() == '_') && LENOK) {
            if (current() != '_')
                currentToken.value.push_back(current());
            advance();
        }

        return;
    }
    
    #undef ISHEX
    #undef ISOCT
    #undef ISBIN
    #define ISEXP (tolower(current()) == 'e' || current() == '+' || current() == '-')
    
    decimal: {
        bool eFound = false;
        bool signFound = false;
        
        while ((isdigit(current()) || ISEXP || current() == '_') && LENOK) {
            if (current() == '_') {
                advance();
                continue;
            } else if (current() == 'e') {
                if (eFound) {
                    // FIXME:
                    // error->plainError(Error::ILLEGAL_NUMBER_FORMAT, "Unexpected character in decimal integer", 
                    //     fileName, {error->createUL(line, currentToken.start, col, col, "Unexpected 'e' in decimal integer"),
                    //         error->createPtr(line, col, "'e' was found here"),
                    //         error->createNote(line, "An 'e' has already been found in this integer")});
                    advance();
                    return;
                }
                eFound = true;
            } else if (current() == '+' || current() == '-') {
                if (signFound) {
                    // FIXME:
                    // error->plainError(Error::ILLEGAL_NUMBER_FORMAT, "Unexpected character in decimal integer", 
                    //     fileName, {error->createUL(line, currentToken.start, col, col, "Unexpected '"+string(1, current())+"' in decimal integer"),
                    //         error->createPtr(line, col, "'"+string(1, current())+"' was found here"),
                    //         error->createNote(line, "A '"+string(1, current())+"' has already been found in this integer")});
                    advance();
                    return;
                }

                if (!eFound) // probably an operator and not part of this token
                    return;
                    
                signFound = true;
            }

            currentToken.value.push_back(current());
            advance();
        } 
    
        if (eFound && (currentToken.value.back() == 'e'
        || currentToken.value.back() == 'E'
        || currentToken.value.back() == '+'
        || currentToken.value.back() == '-')) {
            // FIXME:
            // error->plainError(Error::ILLEGAL_NUMBER_FORMAT, "Unexpected character in decimal integer",
            //     fileName, {error->createUL(line, currentToken.start, col, col - 1, 
            //                 "Expected at least on digit (0-9) after '"+string(1, currentToken.value.back())),
            //         error->createPtr(line, col - 1,  "'"+string(1, currentToken.value.back())+"' was found here")});         
            return;
        }
        
        if (current() == '.' && isdigit(peek()) && LENOK)
            goto floating;
        else
            return;
    }

    floating: {
        currentToken.type = FLOAT;

        bool eFound = false;
        bool signFound = false;

        do { // collect '.' first
            if (current() == '_') {
                advance();
                continue;
            } else if (current() == 'e') {
                if (eFound) {
                    // FIXME:
                    // error->plainError(Error::ILLEGAL_NUMBER_FORMAT, "Unexpected character in floating point integer", 
                    //     fileName, {error->createUL(line, currentToken.start, col, col, "Unexecpted 'e' in float"),
                    //         error->createPtr(line, col, "'e' was found here"),
                    //         error->createNote(line, "An 'e' has already been found in this float")});         
                    advance();
                    return;
                }
                eFound = true;
            } else if (current() == '+' || current() == '-') {
                if (signFound) {
                    // FIXME:
                    // error->plainError(Error::ILLEGAL_NUMBER_FORMAT, "Unexpected character in floating point integer", 
                    //     fileName, {error->createUL(line, currentToken.start, col, col, "Unexecpted '"+string(1, current())+"' in float"),
                    //         error->createPtr(line, col, "'"+string(1, current())+"' was found here"),
                    //         error->createNote(line, "A '"+string(1, current())+"' has already been found in this float")});    
                    advance();
                    return;
                }

                if (!eFound) // probably an operator and not part of this token
                    return;

                signFound = true;
            }

            currentToken.value.push_back(current());
            advance();
        } while ((isdigit(current()) || ISEXP || current() == '_') && LENOK);
            
        if (eFound && (currentToken.value.back() == 'e'
        || currentToken.value.back() == 'E'
        || currentToken.value.back() == '+'
        || currentToken.value.back() == '-')) {
            // FIXME:
            // error->plainError(Error::ILLEGAL_NUMBER_FORMAT, "Unexpected character in floating point integer",
            //     fileName, {error->createUL(line, currentToken.start, col, col - 1, 
            //                 "Expected at least on digit (0-9) after '"+string(1, currentToken.value.back())),
            //         error->createPtr(line, col - 1,  "'"+string(1, currentToken.value.back())+"' was found here")});                  
            return;
        }
    }

    #undef ISEXP
    #undef LENOK
}

bool Lexer::skipComment() {
    if (current() == '#' && peek() == '!') { // ignore '#!...'
        do advance(); while (idx < source.length() && peek(-1) != '\n'); // peek(-1) so '\n' gets skipped
        resetPos();
        return true;
    } else if (peek() == '/') { // single line comment '// ...'
        do advance(); while (idx < source.length() && peek(-1) != '\n'); // peek(-1) so '\n' gets skipped
        resetPos();
        return true;
    } else if (peek() == '*') { // multi line comment '/* ... */'
        size_t start = line;
                                                // check for  '*/'
        while (idx < source.length() && (current() != '*' || peek() != '/')) {
            advance();
            if (peek(-1) == '\n') 
                resetPos();
        }
        
        // FIXME:
        // if (idx >= source.length())
        //     error->plainError(Error::UNEXPECTED_EOF, "Expected multi-line comment to end",
        //         fileName, {error->createMulti(start, line, "The captured comment until EOF")});
        // else
            advance(2); // skip the '*/'
        
        return true;
    }

    return false;
}

void Lexer::checkKeyword() { 
    auto it = find(TokenTypeValue.begin(), TokenTypeValue.end(), currentToken.value);
  
    if (it != TokenTypeValue.end())
        currentToken.type = (TokenType) (it - TokenTypeValue.begin());
}