/**
 * @file note.cpp
 * @author fuechs
 * @brief 
 * @version 0.1
 * @date 2023-01-16
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#include "note.hpp"

ErrorNote::~ErrorNote() {
    message.clear();
    lines.clear();
}

// TODO: add support for multiple lines (Position class)
string ErrorNote::str() {
    stringstream ss;
    ss 
        << CC::GRAY << SC::BOLD << "note: " << CC::DEFAULT << SC::RESET << message
        << "\n\t" << lines.at(0) << "\n\t";
    
    size_t i;
    for (i = 0; i < (pos.colStart - 1); i++) // -1 so arrow points at exact position
        ss << " ";
    ss << CC::GREEN << SC::BOLD;
    while (i++ < pos.colEnd)
        ss << "^";
    ss << CC::DEFAULT << SC::RESET << "\n";

    return ss.str();
}