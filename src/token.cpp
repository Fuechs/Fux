#include "include/token.hpp"

std::string token_to_string(Token token) {
    std::string type_str;
    switch (token.type) {
        case HASH:          type_str = "HASH";          break;        
        case LESST:         type_str = "LESST";         break;
        case GREATERT:      type_str = "GREATERT";      break;    
        case LPAREN:        type_str = "LPAREN";        break;  
        case RPAREN:        type_str = "RPAREN";        break;  
        case LBRACE:        type_str = "LBRACE";        break;         
        case RBRACE:        type_str = "RBRACE";        break;        
        case COLON:         type_str = "COLON";         break;  
        case DOT:           type_str = "DOT";           break;
        case PLUS:          type_str = "PLUS";          break; 
        case INTEGER:       type_str = "INTEGER";       break;    
        case STRING:        type_str = "STRING";        break;     
        case SEMICOLON:     type_str = "SEMICOLON";     break; 
        case IDENTIFIER:    type_str = "IDENTIFIER";    break;
    }
    return "<TOKEN, "+type_str+", "+token.value+">";
}

std::string token_type_to_value(TokenType type) {
    std::string type_str;
    switch (type) {
        case HASH:          type_str = "#";             break;        
        case LESST:         type_str = "<";             break;
        case GREATERT:      type_str = ">";             break;    
        case LPAREN:        type_str = "(";             break;  
        case RPAREN:        type_str = ")";             break;  
        case LBRACE:        type_str = "{";             break;         
        case RBRACE:        type_str = "}";             break;        
        case COLON:         type_str = ":";             break;  
        case DOT:           type_str = ".";             break; 
        case PLUS:          type_str = "+";             break;
        case INTEGER:       type_str = "Integer";       break;        
        case STRING:        type_str = "String";        break;     
        case SEMICOLON:     type_str = ";";             break; 
        case IDENTIFIER:    type_str = "Identifier";    break;
    }
    return type_str;
}