/**
 * @file renderer.cpp
 * @author fuechs
 * @brief Error Renderer Implementation
 * @version 0.1
 * @date 2023-04-26
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "renderer.hpp"

Renderer::Renderer(Source *source, LineMap lines, Suggestion::Vec suggestions)
: source(source), lines(lines), suggestions(suggestions), padding(0), cursor(1) {}

Renderer::~Renderer() {
    lines.clear();
    suggestions.clear();
}

string Renderer::render(size_t padding) {
    stringstream ss;

    this->padding = padding;

    for (size_t line = 1; !lines.empty(); line++) {
        if (!lines.contains(line))
            continue;

        cursor = 1;
        ss << renderLine(line, lines[line].first, lines[line].second);

        if (!lines.contains(line + 1) && line != (--lines.end())->first)
            ss << CC::BLUE << SC::BOLD << string(padding - 4, ' ') << "... |     "
                << SC::RESET << CC::GRAY << "...\n" << SC::RESET;
        
        lines.erase(line);
    }
  
    return ss.str();
}

string Renderer::renderLine(size_t line, Underline::Vec uls, Comment::Vec cmts) {
    string ret;

    // source code
    ret += renderBorder(line) + SC::RESET + CC::GRAY + (*source)[line] + '\n';
    
    size_t maxSize = getMaxSize(uls);

    uls = getSorted(uls);

    // underlines
    for (size_t i = 0; i <= maxSize && !uls.empty(); i++) {
        cursor = 1;
        ret += renderBorder();

        for (Underline::Ptr &ul : uls)
            ret += ul->print(padding, cursor, i); 
        
        ret += '\n';    
    }

    // comments
    for (Comment::Ptr &cmt : cmts)
        ret += renderBorder() + cmt->print();

    return ret;
}

constexpr string Renderer::renderBorder(size_t line) {
    if (line == 0)
        return string(padding, ' ') + CC::RED + SC::BOLD + "|     ";
    
    return string(padding - to_string(line).size() - 1, ' ') 
        + CC::BLUE + SC::BOLD + to_string(line) + " |     ";
}

Underline::Vec Renderer::getSorted(const Underline::Vec &markings) {
    if (markings.empty())
        return Underline::Vec();

    Underline::Vec sorted = markings;

    for (size_t i = 0; i < sorted.size(); i++)
        for (size_t j = i + 1; j < sorted.size(); j++)
            if (sorted[j]->start < sorted[i]->start) {
                Underline::Ptr temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }

    return sorted;
}

size_t Renderer::getMaxSize(Underline::Vec markings) {
    size_t paragraphs = 0;

    for (Underline::Ptr &mark : markings) 
        if (!mark->message.empty())
            mark->size = paragraphs++;

    return paragraphs != 0 ? paragraphs - 1 : 0;
}