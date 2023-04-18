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
        /*
           3 |        reserve i32, 2, ptr;
             |        -------------------- Memory of type 'i32' is reserved here
        */
        DASH_UL,  

        /*
           2 |        ptr: *i64;
             |        ^^^^^^^^^^ Pointer is declared with type '*i64' here
        */
        ARROW_UL,

        /*
           3 |        reserve i32, 2, ptr;
             |        --------------- ^ -- Memory of type 'i32' is reserved here
             |                        |
             |                        Provided pointer `ptr` points to value of type 'i64', not 'i32'
        */
        POINTER,   
        
        // Note |    This error got thrown because there is no condition enclosing the call
        NOTE,
        
        // Help |    Enclose the call in parenthesis to silence this error
        HELP,
        
        /*
                 |        reserve i64, 2, ptr;
                 |                /~~
        */
        REPLACE,
        
        /*
                 |        x = (self.process(x));
                 |            +               +
        */
        INSERT,
        DOUBLE_INSERT, // insert two single strings at two positions (start and end)

        /*
               10 |    (void *) ptr;
                  |    ptr;
        */
        REMOVE,

        /*
                2 |        ptr: *i64;
                3 |        reserve i32, 2, ptr;
                4 |        x = self.process(x);
                5 |        free ptr;
                   \___ Whole lifetime of `ptr`
        */
        MULTILINE, // information to multiple lines

        /*
                  |  _______________
                8 | | ptr: *void;
                9 | | free ptr;
                  | \_____ ^~~ This is a test message
        */
        HIGHLIGHT, // highlight of multiple lines (enclosed in a box)
        NONE,
    };

    Marking(Kind kind = Kind::NONE, string text = "", size_t line = 0, size_t start = 0, size_t except = 0, size_t end = 0);
    ~Marking();

    Marking &operator=(const Marking &copy);

    // 'print' the marking; should be called by the subject owning this marking
    string print(size_t padding, string line = "");
    // check when this marking should be printed
    size_t pos();

    Kind kind;
    string text;
    size_t line, start, except, end; 
    
    // MULTILINE uses `start` as first line and `end` as last line

    // HIGHLIGHT uses `except` as first line and `except` as last line
    //  since `start` and `end` are used for arrow positions
};

// error subject containing metadata and markings
struct Subject {
    using Ptr = std::shared_ptr<Subject>;
    using Vec = std::vector<Ptr>;

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