/**
 * @file lifetime.cpp
 * @author fuechs
 * @brief object lifetime
 * @version 0.1
 * @date 2023-04-04
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "lifetime.hpp"

#ifdef FUX_ANALYSER

Lifetime::~Lifetime() { uses.clear(); }

// TODO: implement Lifetime::free()
void Lifetime::free() {
    switch (begin->getASTType()) {
        default:
            assert(!"Lifetime::free(): Object not implemented.");
    }
}

#endif