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
: source(source), lines(lines), suggestions(suggestions), padding(0) {}

Renderer::~Renderer() {
    lines.clear();
    suggestions.clear();
}

string Renderer::render(size_t padding) {
    stringstream ss;

    for (size_t line = 1; !lines.empty(); line++) {
        if (!lines.contains(line))
            continue;

        ss << renderLine(line, lines[line]);

        lines.erase(line);
    }
  
    return ss.str();
}

string Renderer::renderLine(size_t line, Marking::Vec markings) {
    stringstream ss;

    ss << renderBorder(padding) << SC::RESET << CC::GRAY << (*source)[line] << '\n';

    return ss.str();
}

constexpr string Renderer::renderBorder(size_t line) {
    if (!line)
        return string(padding, ' ') + CC::RED + SC::BOLD + "|     ";
    
    return string(padding - to_string(line).size() - 1, ' ') 
        + CC::BLUE + SC::BOLD + to_string(line) + " |     ";
}
    
constexpr string Renderer::renderHighlightBorder(size_t line) {
    if (!line)
        return string(padding, ' ') + CC::RED + SC::BOLD + "|  ┃  ";
    
    return string(padding - to_string(line).size() - 1, ' ')
        + CC::BLUE + SC::BOLD + to_string(line) + " |  "
        + CC::RED + "┃  ";
}