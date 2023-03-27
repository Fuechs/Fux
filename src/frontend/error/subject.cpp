/**
 * @file subject.cpp
 * @author fuechs
 * @brief subject struct implementation
 * @version 0.1
 * @date 2023-03-25
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "subject.hpp"

Marking::Marking(Kind kind, string text, size_t line, size_t start, size_t except, size_t end)
: kind(kind), text(text), line(line), start(start), except(except), end(end) {}

Marking::~Marking() { text.clear(); }

Marking &Marking::operator=(const Marking &copy) {
    kind = copy.kind;
    text = copy.text;
    line = copy.line;
    start = copy.start;
    except = copy.except;
    end = copy.end;
    return *this;
}

string Marking::print(size_t padding) {
    stringstream ss;

    switch (kind) {
        case DASH_UL:
            ss << string(padding, ' ') << SC::BOLD << CC::RED << "|    " << CC::BLUE;
            for (size_t i = 1; i <= std::max({end, except}); i++) 
                if (i == except - 1 || i == except + 1)
                    ss << " ";
                else if (i == except)
                    ss << CC::RED << "^" << CC::BLUE;
                else if (i >= start && i <= end)
                    ss << "-";
                else
                    ss << " ";
            ss << " " << text << "\n" << SC::RESET;
            break;
        case ARROW_UL:
            ss << string(padding, ' ') << SC::BOLD << CC::RED << "|    ";
            for (size_t i = 1; i <= end; i++) 
                if (i >= start && i <= end)
                    ss << "^";
                else
                    ss << " ";
            ss << " " << text << "\n" << SC::RESET;
            break;
        case POINTER:
            ss << string(padding, ' ') << SC::BOLD << CC::RED << "|    ";
            for (size_t i = 1; i < except; i++)
                ss << " ";
            ss << "|\n";

            ss << string(padding, ' ') << "|    ";
            for (size_t i = 1; i < except; i++)
                ss << " ";
            ss << text << "\n" << SC::RESET;
            break;
        case NOTE:
            ss << string(padding - 5, ' ') << CC::YELLOW << SC::BOLD << "Note |    " << text << "\n" << SC::RESET;
            break;
        case HELP:
            ss << string(padding - 5, ' ') << CC::YELLOW << SC::BOLD << "Help |    " << text << "\n" << SC::RESET;
            break;
        case REPLACE:
            assert(!"Marking::print(): Marking::Kind not implemented.");
            break;
        case INSERT:
            assert(!"Marking::print(): Marking::Kind not implemented.");
            break;
        case REMOVE:
            assert(!"Marking::print(): Marking::Kind not implemented.");
            break;
        case MULTILINE:
            ss << string(padding, ' ') << CC::RED << SC::BOLD << " \\___ " << text << "\n" << SC::RESET;
            break;
        default:
            assert(!"Marking::print(): Marking::Kind not implemented.");
    }

    return ss.str();
}

size_t Marking::pos() {
    switch (kind) {
        case MULTILINE:     return end;
        default:            return line;
    }
}

/// SUBJECT ///

Subject::Subject(Metadata meta, Marking::Vec marks) : meta(meta), marks(marks) {}

Subject::~Subject() { marks.clear(); }

Subject &Subject::operator=(const Subject &copy) {
    meta = copy.meta;
    marks = copy.marks;
    return *this;
}

string Subject::print() {
    stringstream ss;

    using LineMap = map<size_t, vector<Marking *>>; 
                    // line number    ^ there can be multiple markings per line

    // determine which lines should be printed out
    LineMap relevant_lines = {};
    for (Marking &marking : marks) 
        switch (marking.kind) {
            case Marking::MULTILINE:
                relevant_lines[marking.start].push_back(nullptr);
                relevant_lines[marking.end].push_back(&marking);
                break;
            default:
                relevant_lines[marking.line].push_back(&marking);
        }
    
    LineMap::iterator lmi = relevant_lines.begin();
    while (lmi != relevant_lines.end()) {
        if (((++lmi)->first - (--lmi)->first) > 3 || (++lmi)->first == (--lmi)->first + 1) 
            // if amount of lines between two markings is > 3 or == 0, advance
            lmi++;
        else  // else add lines inbetween to relevant_lines
            for (size_t line = lmi++->first; line < lmi->first; line++)
                relevant_lines[line].push_back(nullptr);
    }

    // determine error position
    size_t line = relevant_lines.begin()->first;
    size_t col = 1; 
    if (relevant_lines.begin()->second.front()->kind != Marking::MULTILINE)
        col = relevant_lines.begin()->second.front()->start;

    for (Marking *&mark : relevant_lines.begin()->second)
        if (mark->kind != Marking::MULTILINE)
            col = std::max({col, mark->start});

    // print error position
    ss << string(padding - 2, ' ') << CC::BLUE << SC::BOLD << ">>> " << SC::RESET
        << *meta.file << ":" << line << ":" << col << "\n"; 
    
    for (line = 1; line <= meta.lstLine; line++) {
        if (!relevant_lines.contains(line))
            continue;

        string lineStr = to_string(line);
        ss << CC::BLUE << SC::BOLD << string(padding - lineStr.size() - 1, ' ') << lineStr << " |    "
                                                                  //  ^^^^                    ^^^
                                                                  // we subtract 1 here for the space after the line number
            << SC::RESET << CC::GRAY << meta[line] << "\n" << CC::DEFAULT;

        for (Marking *&marking : relevant_lines[line])
            if (marking)
                ss << marking->print(padding);

        if (!relevant_lines.contains(line + 1) && line != (--relevant_lines.end())->first)
            ss << CC::BLUE << string(padding - 4, ' ') << "... |    " << CC::GRAY << "...\n" << CC::DEFAULT;
    }

    return ss.str();
}