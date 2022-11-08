/**
 * @file lexer.cpp
 * @author fuechs
 * @brief fux lexer 
 * @version 0.1
 * @date 2022-11-01
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "lexer.hpp"

TokenList Lexer::lex() {
    while (idx < source.length()) {
        getToken();

        switch (currentToken.type) {
            case IDENTIFIER:
                do
                    advance();
                while (getIdent());
                hasLetter = false;
                checkKeyword();
                break;

            case STRING:
                getString();
                break;

            case NUMBER:
            case FLOAT:
                getNumber();
                break;

            default: break;
        }

        currentToken.end = col - 1;
        endToken();
        // update line and column for next token
        currentToken.start = col;
        currentToken.line = line; 
    }

    currentToken.type = _EOF;
    currentToken.value = TokenTypeValue[_EOF];
    endToken();

    return tokens;
}

void Lexer::parseLines() {
    stringstream line;

    for (int i = 0; i < source.length(); i++) {
        if (source[i] == '\n') {
            lines.push_back(line.str());
            line.str("");
        } else
            line << source[i];
    }

    if (!line.str().empty())
        lines.push_back(line.str());
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

            currentToken.type = GREATERTHAN;
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
                currentToken.type = RPOINTER;
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
        
        case ' ':
        case '\t':
        case '\r':
            advance();
            break;
        
        case '\n':
            advance();
            resetPos();
            break;
        
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            currentToken.type = NUMBER;
            advance();
            break;
        
        case '\'':
            currentToken.type = CHAR;
            if (!isalpha(peek()))
                error->createError(EXPECTED_CHAR_LITERAL_EOF, line, col, "expected actual character");
            else {
                advance();
                currentToken.value = current();
                if (peek() != '\'')
                    error->createError(ILLEGAL_CHAR_LITERAL_FORMAT, line, col, "expected ending quote");
                else
                    advance();
            }
            break;
         
        case '"':
            currentToken.type = STRING;
            currentToken.value = "";
            break;
        
        default:
            if (!isalpha(current()) && currentToken.value != "_") {
                error->createError(UNEXPECTED_SYMBOL, line, col, "unknown character '"+currentToken.value+"'");
                advance();
            }
            else {
                if (isalpha(current()))
                    hasLetter = true;
                currentToken.type = IDENTIFIER;
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

bool Lexer::getIdent() {
    if ((isalpha(current())
    ||  isdigit(current())
    ||  current() == '_')
    &&  idx < source.length()) {
        if (isalpha(current()))
            hasLetter = true;
        currentToken.value.push_back(current());
        return true;
    }

    if (!hasLetter)
        error->createError(GENERIC, currentToken, "expected at least one character in identifier '"+currentToken.value+"'");
    
    return false;
}

void Lexer::getString() {
    advance();

    if (idx >= source.length())
        error->createError(EXPECTED_STRING_LITERAL_EOF, line, col, "unterminated string");
    
    while (current() != '"') {
        if (current() == '\n') {
            error->createError(ILLEGAL_STRING_FORMAT, line, col, "expected '\"' before end of line");
            return;
        }

        if (current() == '\\' & peek() != '\\' && !isalpha(peek())) {
            error->createError(ILLEGAL_STRING_FORMAT, line, col, "invalid escape sequence");
            return;
        }

        currentToken.value.push_back(current());
        advance();

        if (idx >= source.length()) {
            error->createError(EXPECTED_STRING_LITERAL_EOF, line, col, "unterminated string");
            return;
        }
    } 

    advance();
}

void Lexer::getNumber() {

    if (currentToken.value == "0" && peek() == 'x') {
        currentToken.type = HEXADECIMAL;
        currentToken.value = "0x";
        advance();
        // extra if for error tracking and not allowing '0x_'
        if (isdigit(current()) && idx < source.length()) {
            currentToken.value.push_back(current());
            advance();
        } else
            error->createError(ILLEGAL_NUMBER_FORMAT, line, col, "expected at least one digit after '0x'");
        
        while ((isdigit(current()) || current() == '_') && idx < source.length()) {
            if (current() != '_')
                currentToken.value.push_back(current());
            advance();
        }
    } else {

        bool eFound = false;

        while (idx < source.length()) {
            if (isdigit(current())) {
                currentToken.value.push_back(current());
                advance();
            } else if (current() == '_')
                advance();
            else if (tolower(current()) == 'e') {
                if (eFound) {
                    error->createError(ILLEGAL_NUMBER_FORMAT, line, col, "duplicate 'e' in number");
                    return;
                }

                currentToken.value.push_back(current());
                eFound = true;
                
                if (!isdigit(peek()) && peek() != '+' && peek() != '-') {
                    error->createError(ILLEGAL_NUMBER_FORMAT, line, col, "expected at least one digit or sign after 'e'");
                    return;
                }

                advance();
                currentToken.value.push_back(current());
                if ((current() == '+' || current() == '-') && !isdigit(peek())) {
                    error->createError(ILLEGAL_NUMBER_FORMAT, line, col, "expected at least on digit after 'e"+ to_string(current()) +"' in number");
                    return;
                }
                advance();
            } else 
                break;
        }

        eFound = false;

        if (currentToken.type == FLOAT || idx >= source.length())
            return;

        if (current() == '.') {
            advance();

            if (idx >= source.length()) {
                error->createError(ILLEGAL_NUMBER_FORMAT, line, col, "character '.' in number with no following digits");
                return;
            } else if (current() == '.') {
                error->createError(ILLEGAL_NUMBER_FORMAT, line, col, "unexpected character '.' in float");
                return;
            } else
                currentToken.value.push_back('.');
            
            currentToken.type = FLOAT;
            
            while ((isdigit(current()) || current() == '_' || tolower(current()) == 'e') && idx < source.length()) {
                if (tolower(current()) == 'e') {
                    if (eFound) {
                        error->createError(ILLEGAL_NUMBER_FORMAT, line, col, "duplicate 'e' found");
                        return;
                    }
                    eFound = true;
                    currentToken.value.push_back(current());
                } else if (current() != '_')
                    currentToken.value.push_back(current());
                advance();
            }
        }

    }

}

bool Lexer::skipComment() {
    if (peek() == '/') {
        while (idx < source.length() && current() != '\n')
            advance();
        advance();
        resetPos();
        return true;
    }

    if (peek() == '*') {
        while (idx < source.length() 
        && current() != '/' 
        && peek(2) != '*') {
            if (current() == '\n') {
                resetPos();
                --idx;
            }
            advance();
        }
        
        if (idx >= source.length())
            error->createError(UNEXPECTED_EOF, line, col, "expected multi-line comment to end");
        else
            advance(2);
        
        return true;
    }

    return false;
}

void Lexer::checkKeyword() {
    if      (currentToken.value == "get")           currentToken.type = KEY_GET;
    else if (currentToken.value == "package")       currentToken.type = KEY_PACKAGE;
    else if (currentToken.value == "if")            currentToken.type = KEY_IF;
    else if (currentToken.value == "else")          currentToken.type = KEY_ELSE;
    else if (currentToken.value == "while")         currentToken.type = KEY_WHILE;
    else if (currentToken.value == "do")            currentToken.type = KEY_DO;
    else if (currentToken.value == "for")           currentToken.type = KEY_FOR;
    else if (currentToken.value == "in")            currentToken.type = KEY_IN;
    else if (currentToken.value == "return")        currentToken.type = KEY_RETURN;
    else if (currentToken.value == "except")        currentToken.type = KEY_EXCEPT;
    else if (currentToken.value == "switch")        currentToken.type = KEY_SWITCH;
    else if (currentToken.value == "case")          currentToken.type = KEY_CASE;
    else if (currentToken.value == "default")       currentToken.type = KEY_DEFAULT;
    else if (currentToken.value == "break")         currentToken.type = KEY_BREAK;
    else if (currentToken.value == "label")         currentToken.type = KEY_LABEL;
    else if (currentToken.value == "goto")          currentToken.type = KEY_GOTO;
    else if (currentToken.value == "delete")        currentToken.type = KEY_DELETE;
    else if (currentToken.value == "exit")          currentToken.type = KEY_EXIT;
    
    else if (currentToken.value == "safe")          currentToken.type = KEY_SAFE;
    else if (currentToken.value == "intern")        currentToken.type = KEY_INTERN;
    else if (currentToken.value == "static")        currentToken.type = KEY_STATIC;
    else if (currentToken.value == "final")         currentToken.type = KEY_FINAL;
    
    else if (currentToken.value == "using")         currentToken.type = KEY_USING;
    else if (currentToken.value == "typedef")       currentToken.type = KEY_TYPEDEF;
    
    else if (currentToken.value == "true")          currentToken.type = KEY_TRUE;
    else if (currentToken.value == "false")         currentToken.type = KEY_FALSE;
    else if (currentToken.value == "null")          currentToken.type = KEY_NULL;
    
    else if (currentToken.value == "void")          currentToken.type = KEY_VOID;
    else if (currentToken.value == "bool")          currentToken.type = KEY_BOOL;
    else if (currentToken.value == "i8")            currentToken.type = KEY_I8;
    else if (currentToken.value == "u8")            currentToken.type = KEY_U8;
    else if (currentToken.value == "char")          currentToken.type = KEY_CHAR;
    else if (currentToken.value == "i16")           currentToken.type = KEY_I16;
    else if (currentToken.value == "u16")           currentToken.type = KEY_U16;
    else if (currentToken.value == "wchar")         currentToken.type = KEY_WCHAR;
    else if (currentToken.value == "i32")           currentToken.type = KEY_I32;
    else if (currentToken.value == "u32")           currentToken.type = KEY_U32;
    else if (currentToken.value == "float")         currentToken.type = KEY_FLOAT;
    else if (currentToken.value == "i64")           currentToken.type = KEY_I64;
    else if (currentToken.value == "u64")           currentToken.type = KEY_U64;
    else if (currentToken.value == "double")        currentToken.type = KEY_DOUBLE;
    else if (currentToken.value == "int")           currentToken.type = KEY_INT;
    else if (currentToken.value == "uint")          currentToken.type = KEY_UINT;
    else if (currentToken.value == "str")           currentToken.type = KEY_STR;
    else if (currentToken.value == "var")           currentToken.type = KEY_VAR;
    else if (currentToken.value == "enum")          currentToken.type = KEY_ENUM;
    else if (currentToken.value == "struct")        currentToken.type = KEY_STRUCT;
}