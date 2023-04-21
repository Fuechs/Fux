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
        COMMENT,
    };

    virtual ~Marking();

    virtual string print(size_t padding, string line) = 0;
    virtual constexpr bool printAt(size_t line) = 0;
    virtual constexpr size_t getLine() = 0;
    virtual constexpr size_t getCol() = 0;
    virtual constexpr Kind kind() = 0;

    // create a standard underline and pointer marking
    static Ptr std(size_t line, size_t start, size_t end, string message = "", size_t ptr = 0, string info = "");
};

struct Arrow;

struct Underline : public Marking {
    Underline(size_t line = 0, size_t start = 0, size_t end = 0, string message = "", shared_ptr<Arrow> arrow = nullptr);
    ~Underline() override;

    string print(size_t padding, string line) override;
    constexpr bool printAt(size_t line) override;
    constexpr size_t getLine() override;
    constexpr size_t getCol() override;
    constexpr Kind kind() override;

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
    size_t line, start, end;
/*
    foo_bar_foo;
    ~~~~~~~~~~~~ The message.

(if the space right from the underline is taken:)
    foo_bar_foo;
    |~~~~~~~~~~~ <taken>
    The message.
*/
    string message;
    shared_ptr<Arrow> arrow;
};

struct Arrow : public Marking {
    using Ptr = shared_ptr<Arrow>;

    Arrow(size_t line = 0, size_t col = 0, string message = "");
    ~Arrow() override; 

    string print(size_t padding, string line) override;
    constexpr bool printAt(size_t line) override;
    constexpr size_t getLine() override;
    constexpr size_t getCol() override;
    constexpr Kind kind() override;

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

struct Comment : public Marking {
    Comment(size_t line = 0, size_t col = 0, string message = "");
    ~Comment() override;

    string print(size_t padding, string line) override;
    constexpr bool printAt(size_t line) override;
    constexpr size_t getLine() override;
    constexpr size_t getCol() override;
    constexpr Kind kind() override;


/*
        {
            foo
            bar
        }
[line]  ; <message>
        |
       col 
*/
    size_t line, col;
    string message;
};

Marking::Vec operator+(const Marking::Ptr &lhs, const Marking::Ptr &rhs);