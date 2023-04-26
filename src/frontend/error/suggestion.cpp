/**
 * @file suggestion.cpp
 * @author fuechs
 * @brief Error Suggestion Implementation
 * @version 0.1
 * @date 2023-04-26
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "suggestion.hpp"

Suggestion::~Suggestion() {}

HelpSuggestion::HelpSuggestion(size_t line, string message) : message(message) {}

NoteSuggestion::NoteSuggestion(size_t line, string message) : message(message) {}

HelpSuggestion::~HelpSuggestion() { message.clear(); }

NoteSuggestion::~NoteSuggestion() { message.clear(); }