#include <iostream>

#include "include/token.hpp"

std::string as_string(TokenType type) {
    
    switch (type) {
        case TT_INT:    return "integer";
        case TT_FLOAT:  return "float";
        case TT_MINUS:  return "minus";
        case TT_PLUS:   return "plus";
    }

}