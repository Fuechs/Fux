/**
 * @file suggestion.hpp
 * @author fuechs
 * @brief Error Suggestion Header
 * @version 0.1
 * @date 2023-04-26
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../fux.hpp"

struct Suggestion {
    using Ptr = shared_ptr<Suggestion>;
    using Vec = vector<Ptr>;

    virtual ~Suggestion();
};

struct HelpSuggestion : public Suggestion {
    HelpSuggestion(size_t line = 0, string message = "");
    ~HelpSuggestion() override;

    string message;     
};

struct NoteSuggestion : public Suggestion {
    NoteSuggestion(size_t line = 0, string message = "");
    ~NoteSuggestion() override;

    string message; 
};