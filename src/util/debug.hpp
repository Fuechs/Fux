/**
 * @file debug.hpp
 * @author fuechs
 * @brief debug logs header
 * @version 0.1
 * @date 2023-04-07
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../fux.hpp"
#include "../frontend/ast/stmt.hpp"

class Debug {
public:
    using Ptr = std::shared_ptr<Debug>;

    // check wether stmt is a nullptr,
    // if not call stmt->debugPrint()
    void operator()(Stmt::Ptr &ast);

    size_t indent = 0;
};