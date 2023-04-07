/**
 * @file context.cpp
 * @author fuechs
 * @brief fux context
 * @version 0.1
 * @date 2023-04-07
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "context.hpp"

Context::Context() {
    debug = std::make_shared<Debug>();
    types = {};
}

Type::Ptr Context::load(Type *type) {
    for (Type::Ptr &e : types)
        if (*type == *e)
            return e;
    types.push_back(std::make_shared<Type>(type));
    return types.back();
}