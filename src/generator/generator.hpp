/**
 * @file generator.hpp
 * @author fuechs
 * @brief fux generator header
 * @version 0.1
 * @date 2022-10-24
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "../parser/ast.hpp"

namespace fux {
    
    class Generator {
    public:
        Generator(Block root = {}) 
        : root(root) {}

        Block root;
    };

}