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
        COMMENT,
    };

    virtual ~Marking();

    virtual constexpr size_t getLine() = 0;
    virtual constexpr size_t getCol() = 0;
    virtual constexpr size_t getSize();
    virtual constexpr bool hasMessage() = 0;
    virtual constexpr Kind kind() = 0;
    virtual void setSize(size_t size);

    // create a standard underline and pointer marking
    static Ptr std(size_t line, size_t start, size_t end, string message = "", size_t ptr = 0, string info = "");
};

struct Arrow {
    using Ptr = shared_ptr<Arrow>;

    Arrow(size_t line = 0, size_t col = 0, string message = "");
    ~Arrow(); 

/*
        col
        |
[line] foobar;
        ^ 
        | 
        <message>
*/
    size_t line, col;
    string message;    
};

struct Underline : public Marking {
    using Ptr = shared_ptr<Underline>;
    using Vec = vector<Ptr>;

    Underline(size_t line = 0, size_t start = 0, size_t end = 0, string message = "", Arrow::Ptr arrow = nullptr);
    ~Underline() override;

    constexpr size_t getLine() override;
    constexpr size_t getCol() override;
    constexpr size_t getSize() override;
    constexpr bool hasMessage() override;
    constexpr Kind kind() override;
    void setSize(size_t size) override;

    string print(size_t padding, size_t cursor, size_t size);

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
    size_t line, start, end, size;
                                /* foo_bar;
                                   |~~~~~~
                                   | <- size = 3
                                   | 
                                   <message>
                                */
/*
    foo_bar_foo;
    ~~~~~~~~~~~~ The message.

(if the space right from the underline is taken:)
    foo_bar_foo;
    |~~~~~~~~~~~ <taken>
    The message.
*/
    string message;
    Arrow::Ptr arrow;
};

struct Comment : public Marking {
    using Ptr = shared_ptr<Comment>;
    using Vec = vector<Ptr>;

    Comment(size_t line = 0, size_t col = 0, string message = "");
    ~Comment() override;

    constexpr size_t getLine() override;
    constexpr size_t getCol() override;
    constexpr bool hasMessage() override;
    constexpr Kind kind() override;

    string print();

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