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
: source(source), lines(lines), suggestions(suggestions), padding(0), cursor(1), highlight(false) {}

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
        ss << renderLine(line, lines[line]);

        if (!lines.contains(line + 1) && line != (--lines.end())->first)
            ss << CC::BLUE << SC::BOLD << string(padding - 4, ' ') << "... |     "
                << SC::RESET << CC::GRAY << "...\n" << SC::RESET;
        
        lines.erase(line);
    }
  
    return ss.str();
}

string Renderer::renderLine(size_t line, Marking::Vec markings) {
    stringstream ss;

    highlight = !markings.empty() && markings.front()->kind() == Marking::HIGHLIGHT;

    if (highlight && markings.front()->getLine() != 0)
        ss << renderHighlight(line, markings.front()); // render top

    // source code
    ss << renderBorder(line) << SC::RESET << CC::GRAY << (*source)[line] << '\n';
    
    size_t maxSize = getMaxSize(markings) + 1;

    Marking::Vec sorted = getSorted(markings);

    // markings
    for (size_t i = maxSize; i --> 0;) {
        if (sorted.empty() 
        || (sorted.size() == 1 && sorted[0]->kind() == Marking::HIGHLIGHT))
            break;

        ss << renderBorder();

        if (i + 1 == maxSize) 
            for (size_t j = 0; j < sorted.size(); j++) {
                next = j + 1 == sorted.size() ? nullptr : sorted[j + 1];
                ss << renderMarking(sorted[j]);

                if (next && next->kind() == Marking::COMMENT 
                && sorted[j]->kind() != Marking::COMMENT)
                    ss << '\n' << renderBorder();
            }
        else if (i == 0) {
        } else {
        }
        
        ss << '\n';
    }

    if (highlight
    && dynamic_cast<Highlight *>(markings.front().get())->lstLine == line) {        
        ss << renderHighlight(line, markings.front()); // render bottom 
        highlight = false;
    }

    return ss.str();
}

string Renderer::renderMarking(Marking::Ptr &mark) {
    switch (mark->kind()) {
        case Marking::UNDERLINE:   
            return renderUnderline(dynamic_cast<Underline *>(mark.get()));
        case Marking::COMMENT:
            return renderComment(dynamic_cast<Comment *>(mark.get()));
        default:                    
            return string();
    }
}

string Renderer::renderUnderline(Underline *ul) {
    stringstream ss;

    #define color() (ul->dashed ? CC::BLUE : CC::RED)

    ss << color();

    size_t last = ul->arrow ? std::max(ul->arrow->col, ul->end) : ul->end;

    for (; cursor <= last; cursor++) {
        if (ul->size != 0 && cursor == ul->start) 
            ss << '|';
        else if (ul->arrow && cursor == ul->arrow->col)
            ss << CC::MAGENTA << '^' << color();
        else if (ul->arrow 
        && (cursor == ul->arrow->col - 1 || cursor == ul->arrow->col + 1))
            ss << ' ';
        else if (cursor >= ul->start && cursor <= ul->end)
            ss << (ul->dashed ? '-' : '~');
        else
            ss << ' '; 
    }

    if (ul->size == 0)
        ss << ' ' << ul->message;

    return ss.str();
}

string Renderer::renderComment(Comment *ct) {
    string ret = string(ct->col - 1, ' ') + CC::GREEN + "; " + ct->message;

    if (next && next->kind() == Marking::COMMENT)
        ret += '\n' + renderBorder();
    
    return ret;
}

string Renderer::renderHighlight(size_t line, Marking::Ptr mark) {
    stringstream ss;
    Highlight *hl = dynamic_cast<Highlight *>(mark.get());
    
    if (hl->fstLine == line)
        ss << string(padding, ' ') << CC::RED << SC::BOLD << "|  " 
            << "┏" << string("━") * ((*source)[line].size() + 5) << '\n';
                    // adjust size of line to source code

    else if (hl->lstLine == line) {
        ss << string(padding, ' ') << CC::RED << SC::BOLD << "|  " << "┗━━";

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
    if (markings.empty())
        return Marking::Vec();

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
    size_t paragraphs = 0;

    for (Marking::Ptr &mark : markings) 
        if (mark->hasMessage())
            mark->setSize(paragraphs++);

    return paragraphs == 0 ? 0 : paragraphs - 1;
}