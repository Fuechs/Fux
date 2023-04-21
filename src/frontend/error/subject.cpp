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

string HelpSuggestion::print(size_t padding) {
    stringstream ss;

    ss << CC::YELLOW << SC::BOLD 
        << (padding <= 5 ? "" : string(padding - 5, ' ')) 
        << "Help |     " << message << '\n' << SC::RESET;

    return ss.str();
}

bool HelpSuggestion::printAt(size_t line) { return this->line == line; }

NoteSuggestion::NoteSuggestion(size_t line, string message) : line(line), message(message) {}

string NoteSuggestion::print(size_t padding) {
    stringstream ss;

    ss << CC::YELLOW << SC::BOLD 
        << (padding <= 5 ? "" : string(padding - 5, ' ')) 
        << "Help |     " << message << '\n' << SC::RESET;

    return ss.str();
}

bool NoteSuggestion::printAt(size_t line) { return this->line == line; }

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

    ss << CC::BLUE << SC::BOLD << string(padding - 2, ' ') 
        << ">>> " << CC::DEFAULT << meta.file << ':' << meta.fstLine << ':' << meta.fstCol << '\n';

    for (Marking::Ptr &mark : markings) 
        if (mark->kind() == Marking::UNDERLINE) {
            dynamic_cast<Underline *>(mark.get())->dashed = false;
            break;
        }

    for (Source *&src : fux.sources)
        if (src->filePath == meta.file) {
            for (size_t line = 1; line <= src->sourceCode.size(); line++) {
                string lineStr = to_string(line);
                
                ss << CC::BLUE << SC::BOLD << string(padding - lineStr.size() - 1, ' ')
                    << lineStr << " |     " << SC::RESET << CC::GRAY 
                    << (*src)[line] << "\n" << SC::RESET;

                for (Marking::Ptr &mark : markings)
                    if (mark->printAt(line)) 
                        ss << mark->print(padding, (*src)[line]);

                for (Suggestion::Ptr &suggest : suggestions)
                    if (suggest->printAt(line))
                        ss << suggest->print(padding);
            } 

            break;
        }

    return ss.str();
}