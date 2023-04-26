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
        HIGHLIGHT,
    };

    virtual ~Marking();

    virtual constexpr size_t getLine() = 0;
    virtual constexpr size_t getCol() = 0;
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
    Underline(size_t line = 0, size_t start = 0, size_t end = 0, string message = "", Arrow::Ptr arrow = nullptr);
    ~Underline() override;

    constexpr size_t getLine() override;
    constexpr size_t getCol() override;
    constexpr bool hasMessage() override;
    constexpr Kind kind() override;
    void setSize(size_t size) override;

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
    size_t line, start, end, size; // size is the same as for `Arrow`
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
    Comment(size_t line = 0, size_t col = 0, string message = "");
    ~Comment() override;

    constexpr size_t getLine() override;
    constexpr size_t getCol() override;
    constexpr bool hasMessage() override;
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

struct Highlight : public Marking {
    Highlight(size_t fstLine = 0, size_t lstLine = 0, size_t fstCol = 0, size_t lstCol = 0, 
        string message = "", Marking::Vec markings = {});
    ~Highlight() override;

    constexpr size_t getLine() override;
    constexpr size_t getCol() override;
    constexpr bool hasMessage() override;
    constexpr Kind kind() override;

/*
          |  ┏━━━━━━━━
<fstLine> |  ┃ foo;
<line   > |  ┃ bar;
<line   > |  ┃ foo;
          |  ┃ <some marking>
<lstLine> |  ┃ bar_thing;
          |  ┗━┻━━━━━━━┻━ <message>
               |       |
             fstCol  lstCol
*/
    size_t fstLine, lstLine, fstCol, lstCol;
    string message;
    Marking::Vec markings; 
    StringVec content; // source lines
};

Marking::Vec operator+(const Marking::Ptr &lhs, const Marking::Ptr &rhs);