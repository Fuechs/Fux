#include "include/lexer.hpp"


std::vector<Token> Lexer::lex() {
    Token token;

    while (idx < source.length()) {
        token = tokenize();
        switch (token.type) {
            case STRING: {
                idx++;
                std::string lexeme;
                while (source[idx] != '"') 
                    lexeme.push_back(source[idx++]);
                idx++; // ending quote
                token.value = lexeme;
                break;
            }
            case IDENTIFIER: {
                std::string lexeme;
                while (tokenize().type == IDENTIFIER)
                    lexeme.push_back(source[idx++]);
                token.value = lexeme;
                break;
            }
            case INTEGER: {
                std::string lexeme = token.value;
                idx++;
                while (tokenize().type == INTEGER)
                    lexeme.push_back(source[idx++]);
                token.value = lexeme;
                break;
            }
            default: idx++;
        }
        tokens.push_back(token);
    }

    return tokens;
}

Token Lexer::tokenize() {
    Token token;

    if (source[idx] == '\n') {
        idx++; 
        line++;
    }

    while (isspace(source[idx])) idx++;

    token.col = idx+1;
    token.line = line;

    if (isdigit(source[idx])) {
        token.type = INTEGER;
        token.value = source[idx];
        return token;
    }
    
    switch (source[idx]) {
        

        case '#': 
            token.type = HASH;        
            token.value = "#";  
            break;
        case '<': 
            token.type = LESST;      
            token.value = "<";  
            break;
        case '>': 
            token.type = GREATERT;    
            token.value = ">";  
            break;
        case '(': 
            token.type = LPAREN;      
            token.value = "(";  
            break;
        case ')': 
            token.type = RPAREN;      
            token.value = ")";  
            break;
        case '{': 
            token.type = LBRACE;      
            token.value = "{";  
            break;
        case '}': 
            token.type = RBRACE;      
            token.value = "}"; 
            break;
        case ':': 
            token.type = COLON;       
            token.value = ":";  
            break;
        case '.': 
            token.type = DOT;         
            token.value = ".";  
            break;
        case ';': 
            token.type = SEMICOLON;   
            token.value = ";";  
            break;
        case '+': 
            token.type = PLUS;        
            token.value = "+";  
            break;
        case '"': 
            token.type = STRING; 
            break;
        default: 
            token.type = IDENTIFIER; 
            token.value = source[idx]; 
            break;
    }

    return token;
}