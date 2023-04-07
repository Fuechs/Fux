/**
 * @file symbol.hpp
 * @author fuechs
 * @brief fux symbols
 * @version 0.1
 * @date 2023-04-07
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../fux.hpp"

class Sym {
public:
    using Ptr = std::shared_ptr<Sym>;
    using Vec = std::vector<Ptr>;

    std::string name;
    Sym::Ptr parent;
};