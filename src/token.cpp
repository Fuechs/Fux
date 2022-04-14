#include <iostream>

#include "include/token.hpp"

std::string as_string(TokenType type) {
    
    switch (type) {
        case TT_INT:    return "Integer";
        case TT_FLOAT:  return "Float";
        case TT_MINUS:  return "Minus";
        case TT_PLUS:   return "Plus";
    }

}