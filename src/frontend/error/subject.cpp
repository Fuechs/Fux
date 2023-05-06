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
#include "renderer.hpp"

/// SUBJECT ///

Subject::Subject(Metadata meta, Marking::Ptr primary, Marking::Ptr secondary, Marking::Vec other, 
    Suggestion::Vec suggestions, Vec references, Ptr traceback) 
: meta(meta), primary(primary), secondary(secondary), other(other), 
    suggestions(suggestions), references(references), traceback(traceback){}

Subject::~Subject() {
    other.clear();
    suggestions.clear();
    references.clear();
}

string Subject::print() {
    size_t padding = to_string(meta.lstLine).size() + 4;
    stringstream ss;

    size_t line = 0, col = 0;

    Source *src = nullptr;
    for (Source *_ : fux.sources)
        if (meta.file == _->filePath) {
            src = _;
            break;
        }

    if (primary->kind() == Marking::UNDERLINE) {
        Underline *ul = dynamic_cast<Underline *>(primary.get());
        ul->dashed = false;
        line = ul->line;
        col = ul->start;
    } else 
        internalError("Primary marking is not an underline.");

    using LineMap = map<size_t, Marking::Vec>;
//                   line number, markings

    LineMap lines = {};

    // figure out which lines have markings or suggestions
    for (Marking::Ptr &mark : other) 
        lines[mark->getLine()].push_back(mark);

    lines[primary->getLine()].push_back(primary);
    lines[secondary->getLine()].push_back(secondary);

    // previous line
    #define prev() (--lineIter)++ 

    // figure out which lines inbetween markings should be printed 
    for (LineMap::iterator lineIter = lines.begin(); lineIter != lines.end(); lineIter++) 
        if (lineIter != lines.begin())
            // if gap is <= 3, add missing lines
            for (size_t i = prev()->first; (lineIter->first - i <= 3) && i < lineIter->first; i++)
                if (!lines.contains(i))
                    lines[i] = {}; 

    #undef prev

    if (line == 0 || col == 0) {
        line = lines.begin()->second.front()->getLine();
        col = lines.begin()->second.front()->getCol();
    }

    ss << CC::BLUE << SC::BOLD << string(padding - 2, ' ') << ">>> " << CC::DEFAULT 
        << meta.file << ':' << line << ':' << col << '\n'
        << Renderer(src, lines, suggestions).render(padding);

    return ss.str();
}