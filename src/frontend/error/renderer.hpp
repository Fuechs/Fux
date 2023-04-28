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
    size_t padding, cursor;
    bool highlight; // is currently rendering a highlight
    Marking::Ptr next;

    string renderLine(size_t line, Marking::Vec markings);
    string renderMarking(Marking::Ptr &mark);
    string renderUnderline(Underline *ul);
    string renderComment(Comment *ct);
    string renderHighlight(size_t line, Marking::Ptr mark);

    // render the border left with optional line number
    constexpr string renderBorder(size_t line = 0);

    Marking::Vec getSorted(const Marking::Vec &markings);
    // assign size to each marking and return end result
    size_t getMaxSize(Marking::Vec markings);
};