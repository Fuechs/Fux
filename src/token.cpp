#include <iostream>

#include "include/token.hpp"

std::string as_string(TokenType type) {
    
    switch (type) {
        case TT_INT:    return "TT_INT";
        case TT_FLOAT:  return "TT_FLOAT";
        case TT_MINUS:  return "TT_MINUS";
        case TT_PLUS:   return "TT_PLUS";
    }

}