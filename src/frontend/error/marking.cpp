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

constexpr size_t Marking::getSize() { return 0; }

void Marking::setSize(size_t size) { return; }

Marking::Ptr Marking::std(size_t line, size_t start, size_t end, string message, size_t ptr, string info) {
    return make_shared<Underline>(line, start, end, message, 
        ptr == 0 ? nullptr : make_shared<Arrow>(line, ptr, info));
}

Arrow::Arrow(size_t line, size_t col, string message) 
: line(line), col(col), message(message) {}

Arrow::~Arrow() { message.clear(); }

Underline::Underline(size_t line, size_t start, size_t end, string message, Arrow::Ptr arrow) 
: dashed(true), line(line), start(start), end(end), size(0), message(message), arrow(arrow) {}

Underline::~Underline() { message.clear(); }

string Underline::print(size_t padding, size_t cursor, size_t size) {
    return "[Internal Error]: Underline not implemented. ("
        +to_string(padding)+", "
        +to_string(cursor)+", "
        +to_string(size)+"); ";
}

// string Underline::print(size_t padding, string line, bool isInHighlight) {
//     stringstream ss;
    
//     #define color() (dashed ? CC::BLUE : CC::RED) << SC::BOLD

//     #define printBorder() \
//         if (isInHighlight) \
//             ss << CC::RED << SC::BOLD << string(padding, ' ') << "|  ┃  " << color(); \
//         else \
//             ss << Marking::printLeft(padding) << color();

//     printBorder();

//     /* handle this case:
//         foobarfoobar
//         | ---- ^ --- <taken>
//         |      |
//         |      <arrow>
//         <underline>
        
//         ^start ^arrow->col
//     */
//     if (arrow && arrow->col > start && size != 0) 
//         size += (size == 1) + 1;

//     // used later for printing the message
//     bool isLong = size != 0;

//     for (size_t col = 1; col <= end || (arrow && col <= arrow->col); col++) 
//         if (isLong && col == start) {
//             ss << '|';
//             size--;
//         } else if (arrow && col == arrow->col)
//             ss << CC::MAGENTA << '^' << color();
//         else if (arrow && (col == arrow->col - 1 || col == arrow->col + 1))
//             ss << ' ';
//         else if (col >= start && col <= end)
//             ss << (dashed ? '-' : '~');
//         else
//             ss << ' '; 

//     for (size_t i = size; i --> 0;) {
//         if (arrow && i + 1 == size && arrow->col > start) {
//             ss << '\n';
//             printBorder();

//             ss << string(start - 1, ' ') << color() << '|'
//                 << string(arrow->col - start - 1, ' ') << CC::MAGENTA << "|\n";
            
//             printBorder();
            
//             ss << string(start - 1, ' ') << color() << '|'
//                 << string(arrow->col - start - 1, ' ') << CC::MAGENTA << arrow->message;

//             if (i != 0)
//                 i--;
//         } else {
//             printBorder();

//             if (arrow && arrow->col < start) 
//                     ss << CC::MAGENTA << SC::BOLD << string(arrow->col - 1, ' ') << '|'
//                         << color() << string(start - arrow->col - 1, ' ') << '|';
//             else 
//                 ss << color() << string(start - 1, ' ') << '|';
//         }
//     }

//     if (isLong) {
//         printBorder();

//         if (arrow && arrow->col < start) {
//             ss << CC::MAGENTA << SC::BOLD << string(arrow->col - 1, ' ') << '|'
//                 << color() << string(start - arrow->col - 1, ' ') << message << '\n';

//             printBorder();

//             ss << string(arrow->col - 1, ' ')
//                 << CC::MAGENTA << SC::BOLD << arrow->message << '\n';
//         } else
//             ss << color() << string(start - 1, ' ') << message << '\n' << SC::RESET;
//     } else {
//         ss << ' ' << message << '\n' << SC::RESET;

//         if (arrow) {
//             printBorder()
//             ss << CC::MAGENTA << SC::BOLD << string(arrow->col - 1, ' ') << "|\n"; 
//             printBorder();
//             ss << CC::MAGENTA << SC::BOLD << string(arrow->col - 1, ' ') << arrow->message << '\n';
//         }
//     }

//     #undef printBorder
//     #undef color

//     return ss.str();
// }

constexpr size_t Underline::getLine() { return line; }

constexpr size_t Underline::getCol() { return start; }

constexpr size_t Underline::getSize() { return size; }

constexpr bool Underline::hasMessage() { return !message.empty(); }

constexpr Marking::Kind Underline::kind() { return UNDERLINE; }

void Underline::setSize(size_t size) { this->size = size; }

Comment::Comment(size_t line, size_t col, string message) 
: line(line), col(col), message(message) {}

Comment::~Comment() { message.clear(); }

string Comment::print() {
    return string(col - 1, ' ') + CC::GREEN + SC::BOLD + "; " + message + "\n" + SC::RESET;
}

constexpr size_t Comment::getLine() { return line; }

constexpr size_t Comment::getCol() { return col; }

// always false, since it does not affect message positions
constexpr bool Comment::hasMessage() { return false; }

constexpr Marking::Kind Comment::kind() { return COMMENT; }

Marking::Vec operator+(const Marking::Ptr &lhs, const Marking::Ptr &rhs) { return {lhs, rhs}; } 