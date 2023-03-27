/**
 * @file subject.hpp
 * @author fuechs
 * @brief subject struct header
 * @version 0.1
 * @date 2023-03-25
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../metadata.hpp"

// error markings (underline, arrows, notes, help, replacement, etc.)
struct Marking {
    using Vec = std::vector<Marking>;

    enum Kind {
        DASH_UL,    // ---- underline
        ARROW_UL,   // ^^^^ underline
        POINTER,   
        NOTE,
        HELP,
        REPLACE,
        INSERT,
        REMOVE,
        MULTILINE, // information to multiple lines
        NONE,
    };

    Marking(Kind kind = Kind::NONE, string text = "", size_t line = 0, size_t start = 0, size_t except = 0, size_t end = 0);
    ~Marking();

    Marking &operator=(const Marking &copy);

    // 'print' the marking; should be called by the subject owning this marking
    string print(size_t padding);
    // check when this marking should be printed
    size_t pos();

    Kind kind;
    string text;
    size_t line, start, except, end; // ! MULTILINE uses `start` as first line and `end` as last line
};

// error subject containing metadata and markings
struct Subject {
    using Vec = std::vector<Subject>;

    Subject(Metadata meta = Metadata(), Marking::Vec marks = {});
    ~Subject();

    Subject &operator=(const Subject &copy);

    // 'print' the subject; get the string with all contents of the subject
    string print();

    Metadata meta;
    Marking::Vec marks;
    // TODO: Subject *traceback;

    size_t padding;
};