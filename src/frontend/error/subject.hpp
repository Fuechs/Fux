/**
 * @file subject.hpp
 * @author fuechs
 * @brief Error Subject Header
 * @version 0.1
 * @date 2023-03-25
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../metadata.hpp"
#include "marking.hpp"

// suggestions to fix the error
struct Suggestion {
    using Ptr = shared_ptr<Suggestion>;
    using Vec = vector<Ptr>;

    virtual ~Suggestion();

    virtual string print(size_t padding, string line) = 0;
    virtual constexpr bool printAt(size_t line) = 0;
    virtual constexpr size_t getLine() = 0;
};

struct HelpSuggestion : public Suggestion {
    HelpSuggestion(size_t line = 0, string message = "");
    ~HelpSuggestion() override;

    string print(size_t padding, string line) override;
    constexpr bool printAt(size_t line) override;
    constexpr size_t getLine() override;

    size_t line; // print after line
    string message;     
};

struct NoteSuggestion : public Suggestion {
    NoteSuggestion(size_t line = 0, string message = "");
    ~NoteSuggestion() override;

    string print(size_t padding, string line) override;
    constexpr bool printAt(size_t line) override;
    constexpr size_t getLine() override;

    size_t line; // print after line
    string message; 
};

// markings, suggestions, etc. on a line
class LineMeta {
public:
    using Ptr = shared_ptr<LineMeta>;
    using Map = map<size_t, Ptr>;
    using Iter = Map::iterator;

    LineMeta(size_t line = 0, string content = "");
    ~LineMeta();

    string print(size_t padding);

    void addElement(Marking::Ptr &marking);
    void addElement(Suggestion::Ptr &suggestion);

    size_t getLine();
    // the column of the first maring
    size_t getCol();

private:
    size_t line; // line number
    string content; // line content
    Marking::Vec markings;
    Suggestion::Vec suggestions;
};

class Subject {
public:
    using Ptr = shared_ptr<Subject>;
    using Vec = vector<Ptr>;

    Subject(Metadata meta = {}, Marking::Vec markings = {}, Suggestion::Vec suggestions = {},
        Vec references = {}, Ptr traceback = nullptr);
    ~Subject();

    string print();

private:
    Marking::Vec markings;
    Suggestion::Vec suggestions;
    Vec references;
    Ptr traceback;
    Metadata meta;
};