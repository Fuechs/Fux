/**
 * @file context.hpp
 * @author fuechs
 * @brief fux context
 * @version 0.1
 * @date 2023-04-07
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "fux.hpp"
#include "frontend/type.hpp"
#include "frontend/symbol.hpp"
#include "util/debug.hpp"

class Context {
public:
    using Ptr = std::shared_ptr<Context>;

    Context();

    // get pointer to a type in this context
    // if the type does not exist yet, it will be added into the vector
    Type::Ptr load(Type *type);

    // get the source code (split in lines) of a source file
    StringVec &source(const std::string &fileName);

private:
    Debug::Ptr debug;
    Type::Vec types;
};