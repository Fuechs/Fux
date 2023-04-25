/**
 * @file subject.cpp
 * @author fuechs
 * @brief Error Subject Implementation
 * @version 0.1
 * @date 2023-03-25
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "subject.hpp"
#include "../../util/source.hpp"

/// SUGGESTION ///

Suggestion::~Suggestion() {}

HelpSuggestion::HelpSuggestion(size_t line, string message) : line(line), message(message) {}

string HelpSuggestion::print(size_t padding, string line) {
    stringstream ss;

    ss << CC::YELLOW << SC::BOLD 
        << (padding <= 5 ? "" : string(padding - 5, ' ')) 
        << "Help |     " << message << '\n' << SC::RESET;

    return ss.str();
}

constexpr bool HelpSuggestion::printAt(size_t line) { return this->line == line; }

constexpr size_t HelpSuggestion::getLine() { return line; }

NoteSuggestion::NoteSuggestion(size_t line, string message) : line(line), message(message) {}

string NoteSuggestion::print(size_t padding, string line) {
    stringstream ss;

    ss << CC::YELLOW << SC::BOLD 
        << (padding <= 5 ? "" : string(padding - 5, ' ')) 
        << "Help |     " << message << '\n' << SC::RESET;

    return ss.str();
}

constexpr bool NoteSuggestion::printAt(size_t line) { return this->line == line; }

constexpr size_t NoteSuggestion::getLine() { return line; }

HelpSuggestion::~HelpSuggestion() { message.clear(); }

NoteSuggestion::~NoteSuggestion() { message.clear(); }

/// LINE META ///

LineMeta::LineMeta(size_t line, Source *src) 
: line(line), src(src), markings({}), suggestions({}) {}

LineMeta::~LineMeta() {
    markings.clear();
    suggestions.clear();
}

string LineMeta::print(size_t padding) {
    stringstream ss;

    string lineStr = to_string(line);

    if (!markings.empty() && markings.front()->kind() == Marking::HIGHLIGHT) {
        Highlight *hl = dynamic_cast<Highlight *>(markings.front().get());

        if (hl) {
            for (size_t i = hl->fstLine; i <= hl->lstLine; i++)
                hl->content.push_back((*src)[i]);
            
            ss << hl->print(padding, "");
        
            return ss.str();
        }
    }

    ss << CC::BLUE << SC::BOLD << string(padding - lineStr.size() - 1, ' ')
        << line << " |     " << SC::RESET << CC::GRAY << (*src)[line] 
        << '\n' << SC::RESET;

    size_t paragraphs = 0;

    for (Marking::Ptr &mark : markings) {
        mark->setSize(paragraphs);
        ss << mark->print(padding, (*src)[line]);
        paragraphs += (size_t) mark->hasMessage();
    }
    
    for (Suggestion::Ptr &suggest : suggestions)
        ss << suggest->print(padding, (*src)[line]);

    return ss.str();
}

void LineMeta::addElement(Marking::Ptr &marking) {
    markings.push_back(marking);
}

void LineMeta::addElement(Suggestion::Ptr &suggestion) {
    suggestions.push_back(suggestion);
}

size_t LineMeta::getLine() { return line; }

size_t LineMeta::getCol() { return markings.front()->getCol(); }

/// SUBJECT ///

Subject::Subject(Metadata meta, Marking::Vec markings, Suggestion::Vec suggestions, Vec references, Ptr traceback) 
: meta(meta), markings(markings), suggestions(suggestions), references(references), traceback(traceback){}

Subject::~Subject() {
    markings.clear();
    suggestions.clear();
    references.clear();
}

string Subject::print() {
    size_t padding = to_string(meta.lstLine).size() + 4;
    stringstream ss;

    size_t line = 0, col = 0;

    Source *src = nullptr;
    for (Source *_ : fux.sources)
        if (meta.file == _->filePath) {
            src = _;
            break;
        }

    // make first underline marking use '~' instead of '-'
    for (Marking::Ptr &mark : markings) 
        if (mark->kind() == Marking::UNDERLINE) { 
            Underline *ul = dynamic_cast<Underline *>(mark.get());
            if (ul) {
                ul->dashed = false;
                line = mark->getLine();
                col = mark->getCol();
                break;
            }
        }

    // using LineMap = map<size_t, pair<Marking::Vec, Suggestion::Vec>>;
    //                line number      markings      suggestions

    LineMeta::Map lines = {};

    // figure out which lines have markings or suggestions
    for (Marking::Ptr &mark : markings) {
        if (!lines.contains(mark->getLine()))
            lines[mark->getLine()] = make_shared<LineMeta>(mark->getLine(), src);

        lines[mark->getLine()]->addElement(mark);
    }

    for (Suggestion::Ptr &suggest : suggestions) {
        if (!lines.contains(suggest->getLine()))
            lines[suggest->getLine()] = make_shared<LineMeta>(suggest->getLine(), src);

        lines[suggest->getLine()]->addElement(suggest);
    }

    // previous line
    #define prev() (--lineIter)++ 

    // figure out which lines inbetween markings should be printed 
    for (LineMeta::Iter lineIter = ++lines.begin(); lineIter != lines.end(); lineIter++)    
        // if gap is <= 3, add missing lines
        for (size_t i = prev()->first; (lineIter->first - i <= 3) && i < lineIter->first; i++)
            if (!lines.contains(i))
                lines[i] = make_shared<LineMeta>(i, src);

    #undef prev

    if (line == 0 || col == 0) {
        line = lines.begin()->second->getLine();
        col = lines.begin()->second->getCol();
    }

    ss << CC::BLUE << SC::BOLD << string(padding - 2, ' ') << ">>> " << CC::DEFAULT 
        << meta.file << ':' << line << ':' << col << '\n';

    for (size_t line = 1; line <= meta.lstLine; line++) {
        if (!lines.contains(line))
            continue;
        
        ss << lines[line]->print(padding);

        if (!lines.contains(line + 1) && line != (--lines.end())->first)
            ss << CC::BLUE << SC::BOLD << string(padding - 4, ' ') << "... |     "
                << SC::RESET << CC::GRAY << "...\n" << SC::RESET;
            
        lines.erase(line);

        if (lines.empty())
            break;
    }

    return ss.str();
}