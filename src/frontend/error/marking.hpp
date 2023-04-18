/**
 * @file marking.hpp
 * @author fuechs
 * @brief Error Marking Header
 * @version 0.1
 * @date 2023-04-18
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"

struct Marking {
    using Ptr = shared_ptr<Marking>;
    using Vec = vector<Ptr>;
};