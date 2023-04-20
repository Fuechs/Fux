/**
 * @file marking.hpp
 * @author fuechs
 * @brief Error Marking Header
 * @version 0.1
 * @date 2023-04-18
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"

struct Marking {
    using Ptr = shared_ptr<Marking>;
    using Vec = vector<Ptr>;

    enum Kind {
        UNDERLINE,
        ARROW,
    };

    virtual ~Marking();

    virtual string print(size_t padding, string line) = 0;
    virtual bool printAt(size_t line) = 0;
    virtual Kind kind() = 0;
};

struct Underline : public Marking {
    Underline(size_t line = 0, size_t start = 0, size_t end = 0, size_t except = 0, string message = "");
    ~Underline() override;

    string print(size_t padding, string line) override;
    bool printAt(size_t line) override;
    Kind kind() override;

    // false: (red) ~~~~~~
    // true : (blue) ------
    bool dashed;
/*           
      start       end
       |          |
[line] foo_bar_foo; 
       ~~~~~~~~~~~~

      start       end
       |          |
[line] foo_bar_foo; 
       ~~~~ ^ ~~~~~
            |
          except
*/
    size_t line, start, end, except; // TODO: Subject should set except, not the user
/*
    foo_bar_foo;
    ~~~~~~~~~~~~ The message.

(if the space right from the underline is taken:)
    foo_bar_foo;
    |~~~~~~~~~~~ <taken>
    The message.
*/
    string message;
};

struct Arrow : public Marking {
    Arrow(size_t line = 0, size_t col = 0, string message = "");
    ~Arrow() override; 

    string print(size_t padding, string line) override;
    bool printAt(size_t line) override;
    Kind kind() override;

    CC color; // the color of the arrow (set by Subject)
/*
        col
        |
[line] foobar;
        ^ (arrow previously printed by underline)
        | 
        | - size = 3 (size of the arrow)
        | 
        <message>
*/
    size_t line, col, size;
    string message;    
};