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

string Marking::print(size_t padding, string line) {
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
        case REPLACE: {
            if (line.empty())
                assert(!"Marking::print(): Marking::REPLACE received empty line.");

            ss << string(padding, ' ') << CC::WHITE << SC::BOLD << "|    " << SC::RESET << CC::GRAY
                << line.substr(0, start - 1) << CC::WHITE << SC::BOLD << text 
                << SC::RESET << CC::GRAY << line.substr(end) << "\n" 
                << string(padding, ' ') << CC::WHITE << SC::BOLD << "|    ";
            
            size_t col = 1;
            for (; col < start; col++)
                ss << " ";
            ss << "/";
            for (col = 1; col < text.size(); col++)
                ss << "~";
            ss << "\n" << SC::RESET;
            break;
        }
        case INSERT: {
            if (line.empty())
                assert(!"Marking::print(): Marking::INSERT received empty line.");

            ss << string(padding, ' ') << CC::WHITE << SC::BOLD << "|    " << highlightedInsert(line, start - 1, text) << "\n"
                << string(padding, ' ') << CC::WHITE << SC::BOLD << "|    ";
            
            size_t col = 1;
            for (; col < start; col++)
                ss << " ";
            ss << "+";
            for (col = 2; col <= text.size(); col++)
                ss << "~";
            ss << "\n" << SC::RESET;
            break;
        }
        case DOUBLE_INSERT: {
            if (line.empty())
                assert(!"Marking::print(): Marking::DOUBLE_INSERT; received empty line.");

            StringVec inserts = split(text, '\n');

            ss << string(padding, ' ') << CC::WHITE << SC::BOLD << "|    " 
                << highlightedInsert(line, start - 1, inserts.at(0), end - 1, inserts.at(1)) << "\n"
                << string(padding, ' ') << CC::WHITE << SC::BOLD << "|    ";

            size_t col = 1;
            for (; col < start; col++)
                ss << " ";
            ss << "+";
            col++;
            for (; col < start + inserts[0].size(); col++)
                ss << "~";
            for (; col < end + inserts[0].size(); col++)
                ss << " ";
            ss << "+";
            col++;
            for (; col < end + inserts[0].size() + inserts[1].size(); col++)
                ss << "~";
            ss << "\n" << SC::RESET;
            break;
        }
        case REMOVE:
            if (line.empty())
                assert(!"Marking::print(): Marking::REMOVE received empty line.");

            ss << string(padding, ' ') << CC::WHITE << SC::BOLD << "|    " << SC::RESET << CC::GRAY 
                << line.substr(0, start - 1) << line.substr(end) << "\n" << SC::RESET;
            break;
        case MULTILINE:
            ss << string(padding, ' ') << CC::RED << SC::BOLD << " \\___ " << text << "\n" << SC::RESET;
            break;
        case HIGHLIGHT: {
            if (line.empty())
                assert(!"Marking::print(): Marking::HIGHLIGHT received empty line.");

            if (start == UINT64_MAX) 
                ss << string(padding, ' ') << CC::BLUE << SC::BOLD << "|  " << CC::RED << string(15, '_') << "\n";

            string lineStr = to_string(this->line);
            ss << string(padding - lineStr.size() - 1, ' ') << CC::BLUE << SC::BOLD << lineStr << " | " << CC::RED
                << "| " << SC::RESET << CC::GRAY << line << "\n" << SC::RESET; 
            
            if (start == 0 || start == UINT64_MAX)
                break;

            ss << string(padding, ' ') << CC::BLUE << SC::BOLD << "| " << CC::RED << "\\";
            size_t col = 1;
            for (; col < start - 1; col++)
                ss << "_";
            ss << " ^";
            col += col > 1 ? 2 : 1;
            // handle these cases, where the ' ' is not below the content
            // 9 | | free ptr;
            //   | \ ^~~~ This is a test message
            for (; col <= end; col++)
                ss << "~";
            ss << " " << text << "\n";
            break;
        }

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
    padding = std::max((size_t) 5, meta.lstLine);

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
            case Marking::HIGHLIGHT:   
                                                                                            // pass this value so we know
                                                                                            // that this is the first 
                                                                                            // highlight marking when printing
                relevant_lines[marking.except].push_back(new Marking(Marking::HIGHLIGHT, "", marking.except, UINT64_MAX));
                relevant_lines[marking.line].push_back(&marking);
                break;
            default:
                relevant_lines[marking.line].push_back(&marking);
        }
    
    LineMap::iterator lmi = relevant_lines.begin();
    while (lmi != relevant_lines.end()) {
        if (((++lmi)->first - (--lmi)->first) > 5 || (++lmi)->first == (--lmi)->first + 1) 
            // if amount of lines between two markings is > 5 or == 0, advance
            lmi++;
        else  // else add lines inbetween to relevant_lines
            for (size_t line = lmi++->first; line < lmi->first; line++) {
                if (lmi->second.front() 
                && lmi->second.front()->kind == Marking::HIGHLIGHT) {
                    bool push = true;
                    for (Marking *&marking : relevant_lines[line])
                        if (marking && marking->kind == Marking::HIGHLIGHT) {
                            push = false;
                            break;
                        }

                    if (push)
                        relevant_lines[line].push_back(new Marking(Marking::HIGHLIGHT, "", line));
                } else
                    relevant_lines[line].push_back(nullptr);
            }
    }

    // determine error position
    size_t line = relevant_lines.begin()->first;
    size_t col = UINT64_MAX; 

    for (Marking *&mark : relevant_lines.begin()->second)
        if (mark && mark->kind != Marking::MULTILINE && mark->start != 0)
            col = std::min({col, mark->start});

    // print error position
    ss << string(padding - 2, ' ') << CC::BLUE << SC::BOLD << ">>> " << SC::RESET
        << meta.file << ":" << line << ":" << (col == UINT64_MAX ? 1 : col) << "\n"; 
    
    for (line = 1; line <= meta.lstLine; line++) {
        if (!relevant_lines.contains(line))
            continue;

        bool skip = false;
        for (Marking *&marking : relevant_lines[line]) 
            if (marking && marking->kind == Marking::HIGHLIGHT) {
                skip = true;
                break;
            }
                
        if (!skip) {
            string lineStr = to_string(line);
            ss << CC::BLUE << SC::BOLD << string(padding - lineStr.size() - 1, ' ') << lineStr << " |    "
                                                                    //   ^^^^                     ^^^
                                                                    // we subtract 1 here for the space after the line number
                << SC::RESET << CC::GRAY << meta[line] << "\n" << CC::DEFAULT;
        }

        for (Marking *&marking : relevant_lines[line])
            if (marking)
                ss << marking->print(padding, meta[line]);

        if (!relevant_lines.contains(line + 1) && line != (--relevant_lines.end())->first)
            ss << CC::BLUE << string(padding - 4, ' ') << "... |    " << CC::GRAY << "...\n" << CC::DEFAULT;
    }

    return ss.str();
}