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

    ss << CC::BLUE << SC::BOLD << string(padding, ' ') << "|     ";

    if (!dashed) 
        ss << CC::RED;

    for (size_t col = 1; col <= end || col <= except; col++) 
        if (col == except)
            ss << (dashed ? CC::RED : CC::BLUE) << '^' << (dashed ? CC::BLUE : CC::RED);
        else if (col == except - 1 || col == except + 1)
            ss << ' ';
        else if (col >= start && col <= end)
            ss << (dashed ? '-' : '~');
        else
            ss << ' '; 

    ss << ' ' << message << '\n';

    return ss.str();
}

bool Underline::printAt(size_t line) { return this->line == line; }

Marking::Kind Underline::kind() { return UNDERLINE; }

Arrow::Arrow(size_t line, size_t col, string message) 
: color(CC::BLUE), line(line), col(col), size(1), message(message) {}

Arrow::~Arrow() { message.clear(); }

string Arrow::print(size_t padding, string line) {
    stringstream ss;

    while (size --> 0)
        ss << CC::BLUE << SC::BOLD << string(padding, ' ') << "|     " << string(col - 1, ' ') 
            << color << "|\n";
    
    ss << CC::BLUE << SC::BOLD << string(padding, ' ') << "|     " << string(col - 1, ' ') 
        << color << message << '\n';
    
    return ss.str();
}

bool Arrow::printAt(size_t line) { return this->line == line; }

Marking::Kind Arrow::kind() { return ARROW; }