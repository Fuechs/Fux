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

    using LineMap = map<size_t, pair<Marking::Vec, Suggestion::Vec>>;
    //                line number      markings      suggestions

    LineMap lineMeta = {};

    // figure out which lines have markings or suggestions
    for (Marking::Ptr &mark : markings)
        lineMeta[mark->getLine()].first.push_back(mark);

    for (Suggestion::Ptr &suggest : suggestions)
        lineMeta[suggest->getLine()].second.push_back(suggest);

    LineMap::iterator lineIter;

    // previous line
    #define prev() (--lineIter)++ 

    // figure out which lines inbetween markings should be printed 
    for (lineIter = ++lineMeta.begin(); lineIter != lineMeta.end(); lineIter++)    
        // if gap is <= 3, add missing lines
        for (size_t i = prev()->first; (lineIter->first - i <= 3) && i < lineIter->first; i++)
            if (!lineMeta.contains(i))
                lineMeta[i] = {};

    #undef prev

    if (line == 0 || col == 0) {
        line = lineMeta.begin()->second.first.front()->getLine();
        col = lineMeta.begin()->second.first.front()->getCol();
    }

    ss << CC::BLUE << SC::BOLD << string(padding - 2, ' ') << ">>> " << CC::DEFAULT 
        << meta.file << ':' << line << ':' << col << '\n';

    for (Source *&src : fux.sources)
        if (src->filePath == meta.file) {
            for (size_t line = 1; line <= meta.lstLine; line++) {
                if (!lineMeta.contains(line))
                    continue;

                string lineStr = to_string(line);
                        
                ss << CC::BLUE << SC::BOLD << string(padding - lineStr.size() - 1, ' ')
                    << lineStr << " |     " << SC::RESET << CC::GRAY 
                    << (*src)[line] << '\n' << SC::RESET;

                for (Marking::Ptr &mark : lineMeta[line].first)
                    ss << mark->print(padding, (*src)[line]);

                for (Suggestion::Ptr &suggest : lineMeta[line].second)
                    ss << suggest->print(padding, (*src)[line]);
                
                if (!lineMeta.contains(line + 1) && line != (--lineMeta.end())->first)
                    ss << CC::BLUE << SC::BOLD << string(padding - 4, ' ') << "... |     "
                        << SC::RESET << CC::GRAY << "...\n" << SC::RESET;
            } 

            break;
        }


    return ss.str();
}