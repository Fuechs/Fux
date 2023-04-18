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

string Subject::print() { return "not implemented"; }