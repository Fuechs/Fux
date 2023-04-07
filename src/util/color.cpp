/**
 * @file color.cpp
 * @author fuechs
 * @brief colored output implementation
 * @version 0.1
 * @date 2023-04-07
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "color.hpp"

std::ostream &operator<<(std::ostream &os, CC code) {
    return os << "\033[" << static_cast<int>(code) << "m";
}

std::ostream &operator<<(std::ostream &os, BC code) {
    return os << "\033[" << static_cast<int>(code) << "m";
}

std::ostream &operator<<(std::ostream &os, SC code) {
    return os << "\033[" << static_cast<int>(code) << "m";
}

std::string operator+(std::string LHS, CC RHS) {
    return LHS + "\033[" + std::to_string(static_cast<int>(RHS)) + "m";
}

std::string operator+(std::string LHS, BC RHS) {
    return LHS + "\033[" + std::to_string(static_cast<int>(RHS)) + "m";
}

std::string operator+(std::string LHS, SC RHS) {
    return LHS + "\033[" + std::to_string(static_cast<int>(RHS)) + "m";
}

std::string debugText = "\033[33mDebug\033[39m: ";
