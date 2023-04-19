/**
 * @file marking.cpp
 * @author fuechs
 * @brief Error Marking Implementation
 * @version 0.1
 * @date 2023-04-19
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "marking.hpp"


Marking::~Marking() {}

Underline::Underline(size_t line, size_t start, size_t end, size_t except, string message) 
: line(line), start(start), end(end), except(except), message(message) {}

Underline::~Underline() { message.clear(); }

string Underline::print(size_t padding, string line) {
    stringstream ss;

    ss << CC::BLUE << SC::BOLD << string(padding, ' ') << "|     " << string(start - 1, ' ');

    if (dashed)
        ss << string(end - start, '-');
    else
        ss << CC::RED << string(end - start, '~');

    ss << " " << message << "\n";

    return ss.str();
}

Marking::Kind Underline::kind() { return UNDERLINE; }

bool Underline::printAt(size_t line) { return this->line == line; }