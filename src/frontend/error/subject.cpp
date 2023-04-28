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

// string LineMeta::print(size_t padding) {
//     stringstream ss;

//     string lineStr = to_string(line);

//     if (!markings.empty() && markings.front()->kind() == Marking::HIGHLIGHT) {
//         Highlight *hl = dynamic_cast<Highlight *>(markings.front().get());

//         if (hl) {
//             for (size_t i = hl->fstLine; i <= hl->lstLine; i++)
//                 hl->content.push_back((*src)[i]);
            
//             // ss << hl->print(padding, "");
        
//             return ss.str();
//         }
//     }

//     ss << CC::BLUE << SC::BOLD << string(padding - lineStr.size() - 1, ' ')
//         << line << " |     " << SC::RESET << CC::GRAY << (*src)[line] 
//         << '\n' << SC::RESET;

//     size_t paragraphs = 0;

//     for (Marking::Ptr &mark : markings) {
//         mark->setSize(paragraphs);
//         // ss << mark->print(padding, (*src)[line]);
//         paragraphs += (size_t) mark->hasMessage();
//     }
    
//     return ss.str();
// }
/// SUBJECT ///

Subject::Subject(Metadata meta, Marking::Vec markings, Suggestion::Vec suggestions, Vec references, Ptr traceback) 
: meta(meta), markings(markings), suggestions(suggestions), references(references), traceback(traceback){}

Subject::~Subject() {
    markings.clear();
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

    // make first underline marking use '~' instead of '-'
    for (Marking::Ptr &mark : markings) 
        if (mark->kind() == Marking::UNDERLINE) { 
            Underline *ul = dynamic_cast<Underline *>(mark.get());
            if (ul) {
                ul->dashed = false;
                line = mark->getLine();
                col = mark->getCol();
                break;
            }
        }

    using LineMap = map<size_t, Marking::Vec>;
//                  line number, markings

    LineMap lines = {};

    // figure out which lines have markings or suggestions
    for (Marking::Ptr &mark : markings) 
        lines[mark->getLine()].push_back(mark);

    // previous line
    #define prev() (--lineIter)++ 

    // figure out which lines inbetween markings should be printed 
    for (LineMap::iterator lineIter = lines.begin(); lineIter != lines.end(); lineIter++) {

        if (lineIter != lines.begin())
            // if gap is <= 3, add missing lines
            for (size_t i = prev()->first; (lineIter->first - i <= 3) && i < lineIter->first; i++)
                if (!lines.contains(i))
                    lines[i] = {}; 

        // fill highlight gaps
        if (lineIter->second.front()->kind() == Marking::HIGHLIGHT 
        && lineIter->second.front()->getLine() != 0) {
            Highlight *hl = dynamic_cast<Highlight *>(lineIter->second.front().get());

            for (size_t i = hl->fstLine + 1; i <= hl->lstLine; i++)        
                if (hl->lstLine == i)
                    lines[i].insert(lines[i].begin(), make_unique<Highlight>(0, i, hl->fstCol, hl->lstCol, hl->message));
                else
                    lines[i].insert(lines[i].begin(), make_unique<Highlight>());
        }
    }

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