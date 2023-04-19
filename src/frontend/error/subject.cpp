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

string HelpSuggestion::print() { return "not implemented"; }

NoteSuggestion::NoteSuggestion(size_t line, string message) : line(line), message(message) {}

string NoteSuggestion::print() { return "not implemented"; }

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
    size_t padding = meta.lstLine + 4;
    stringstream ss;

    for (Source *&src : fux.sources)
        if (src->filePath == meta.file) {
            for (size_t line = 0; line < src->sourceCode.size(); line++) {
                string lineStr = to_string(line);
                
                ss << CC::BLUE << SC::BOLD << string(padding - lineStr.size() - 1, ' ')
                    << lineStr << " |     " << SC::RESET << CC::GRAY 
                    << (*src)[line + 1] << "\n" << SC::RESET;

                for (Marking::Ptr &mark : markings)
                    if (mark->printAt(line + 1))
                        ss << mark->print(padding, (*src)[line + 1]);
            } 

            break;
        }

    return ss.str();
}