/**
 * @file renderer.hpp
 * @author fuechs
 * @brief Error Renderer Header
 * @version 0.1
 * @date 2023-04-26
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "marking.hpp"
#include "suggestion.hpp"
#include "../../util/source.hpp"

using LineMap = map<size_t, Marking::Vec>;

/**
 * @brief This class formats and outputs each error.
 * 
 */
class Renderer {
public:
    Renderer(Source *source, LineMap lines, Suggestion::Vec suggestions);
    ~Renderer();

    string render(size_t padding);

private:
    Source *source;
    LineMap lines;
    Suggestion::Vec suggestions;
    size_t padding;
    bool highlight; // is currently rendering a highlight

    string renderLine(size_t line, Marking::Vec markings);

    string renderHighlight(size_t line, Marking::Ptr mark);

    // render the border left with optional line number
    constexpr string renderBorder(size_t line = 0);

    Marking::Vec getSorted(const Marking::Vec &markings);
    size_t getMaxSize(Marking::Vec markings);
};