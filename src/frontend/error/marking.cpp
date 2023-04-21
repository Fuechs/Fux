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

Marking::Ptr Marking::std(size_t line, size_t start, size_t end, string message, size_t ptr, string info) {
    return make_shared<Underline>(line, start, end, message, 
        ptr == 0 ? nullptr : make_shared<Arrow>(line, ptr, info));
}

Underline::Underline(size_t line, size_t start, size_t end, string message, Arrow::Ptr arrow) 
: dashed(true), line(line), start(start), end(end), message(message), arrow(arrow) {}

Underline::~Underline() { message.clear(); }

string Underline::print(size_t padding, string line) {
    stringstream ss;

    ss << CC::BLUE << SC::BOLD << string(padding, ' ') << "|     ";

    if (!dashed) 
        ss << CC::RED;

    for (size_t col = 1; col <= end || (arrow && col <= arrow->col); col++) 
        if (arrow && col == arrow->col)
            ss << CC::MAGENTA << '^' << (dashed ? CC::BLUE : CC::RED);
        else if (arrow && (col == arrow->col - 1 || col == arrow->col + 1))
            ss << ' ';
        else if (col >= start && col <= end)
            ss << (dashed ? '-' : '~');
        else
            ss << ' '; 

    ss << ' ' << message << '\n' << SC::RESET;

    if (arrow) 
        ss << arrow->print(padding, line);

    return ss.str();
}

constexpr bool Underline::printAt(size_t line) { return this->line == line; }

constexpr Marking::Kind Underline::kind() { return UNDERLINE; }

Arrow::Arrow(size_t line, size_t col, string message) 
: line(line), col(col), size(1), message(message) {}

Arrow::~Arrow() { message.clear(); }

string Arrow::print(size_t padding, string line) {
    stringstream ss;

    while (size --> 0)
        ss << CC::BLUE << SC::BOLD << string(padding, ' ') << "|     " << string(col - 1, ' ') 
            << CC::MAGENTA << "|\n";
    
    ss << CC::BLUE << SC::BOLD << string(padding, ' ') << "|     " << string(col - 1, ' ') 
        << CC::MAGENTA << message << '\n' << SC::RESET;
    
    return ss.str();
}

constexpr bool Arrow::printAt(size_t line) { return this->line == line; }

constexpr Marking::Kind Arrow::kind() { return ARROW; }

Comment::Comment(size_t line, size_t col, string message) 
: line(line), col(col), message(message) {}

Comment::~Comment() { message.clear(); }

string Comment::print(size_t padding, string line) {
    return "" + CC::BLUE + SC::BOLD + string(padding, ' ') + "|     " 
        + string(col - 1, ' ') + CC::GREEN + "; " + message + "\n" + SC::RESET;
}

constexpr bool Comment::printAt(size_t line) { return this->line == line; }

constexpr Marking::Kind Comment::kind() { return COMMENT; }

Marking::Vec operator+(const Marking::Ptr &lhs, const Marking::Ptr &rhs) { return {lhs, rhs}; }