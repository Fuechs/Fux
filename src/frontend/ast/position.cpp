/**
 * @file position.cpp
 * @author fuechs
 * @brief position struct implementation
 * @version 0.1
 * @date 2023-02-26
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "position.hpp"

Position &Position::operator=(Position pos) {
    lStart = pos.lStart;
    lEnd = pos.lStart;
    colStart = pos.colStart;
    colEnd = pos.colEnd;
    return *this; 
}