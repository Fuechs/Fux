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

    highlight = !markings.empty() && markings.front()->kind() == Marking::HIGHLIGHT;

    if (highlight)
        ss << renderHighlight(line, markings.front()); // render top

    // source code
    ss << renderBorder(line) << SC::RESET << CC::GRAY << (*source)[line] << '\n';
    
    size_t maxSize = getMaxSize(markings) + 1;

    Marking::Vec sorted = getSorted(markings);

    // markings
    for (size_t i = maxSize; i --> 0;) 
        for (size_t col = 1; col <= (*source)[line].size(); col++) {
            ss << renderBorder();

            if (i == maxSize) {
                
            } else {

            }
        }

    if (highlight) {        
        ss << renderHighlight(line, markings.front()); // render bottom 
        highlight = false;
    }

    return ss.str();
}

string Renderer::renderHighlight(size_t line, Marking::Ptr mark) {
    stringstream ss;
    Highlight *hl = dynamic_cast<Highlight *>(mark.get());
    
    if (hl->fstLine == line)
        ss << string(padding, ' ') << CC::RED << SC::BOLD << "|  " 
            << "┏" << string("━") * ((*source)[line].size() + 3) << '\n';
                    // adjust size of line to source code

    else if (hl->lstLine == line) {
        ss << string(padding, ' ') << CC::RED << SC::BOLD << "|  " << "┗";

        for (size_t col = 1; col <= hl->lstCol; col++)
            if (col == hl->fstCol || col == hl->lstCol) 
                ss << "┻";
            else
                ss << "━";
        
        if (!hl->message.empty())
            ss << "━ " << hl->message;
        
        ss << '\n' << SC::RESET;
    }

    return ss.str();
}

constexpr string Renderer::renderBorder(size_t line) {
    if (highlight) {
        if (!line)
            return string(padding, ' ') + CC::RED + SC::BOLD + "|  ┃  ";
        
        return string(padding - to_string(line).size() - 1, ' ')
        + CC::BLUE + SC::BOLD + to_string(line) + " |  "
        + CC::RED + "┃  ";
    }

    if (!line)
        return string(padding, ' ') + CC::RED + SC::BOLD + "|     ";
    
    return string(padding - to_string(line).size() - 1, ' ') 
        + CC::BLUE + SC::BOLD + to_string(line) + " |     ";
}

Marking::Vec Renderer::getSorted(const Marking::Vec &markings) {
    Marking::Vec sorted = markings;

                    // skip highlight marking
    for (size_t i = markings.front()->kind() == Marking::HIGHLIGHT; i < sorted.size(); i++)
        for (size_t j = i + 1; j < sorted.size(); j++)
            if (sorted[j]->getCol() < sorted[i]->getCol()) {
                Marking::Ptr temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }

    return sorted;
}

size_t Renderer::getMaxSize(Marking::Vec markings) {
    size_t ret = 0;

    for (Marking::Ptr &mark : markings) 
        ret = std::max(ret, mark->getSize());

    return ret;
}