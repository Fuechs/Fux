/**
 * @file type.cpp
 * @author fuechs
 * @brief fux type representation
 * @version 0.1
 * @date 2023-04-07
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "type.hpp"

#include "ast/expr.hpp"
#include "../context.hpp"
#include "symbol.hpp"

Type::Type(Kind kind, AccessVec access, size_t ptr, bool ref, bool array, 
    Expr::Ptr size, Sym::Ptr structure, Expr::Ptr custom, Context::Ptr ctx) 
        : kind(kind), access(access), ptr(ptr), ref(ref), array(array),
            size(std::move(size)), structure(structure), custom(std::move(custom)), ctx(ctx) {}

Type::~Type() { access.clear(); }

Type &Type::operator=(Type &copy) {
    kind = copy.kind;
    access = copy.access;
    ptr = copy.ptr;
    ref = copy.ref;
    array = copy.array;
    size = std::move(copy.size);
    structure = copy.structure;
    custom = std::move(copy.custom);
    ctx = copy.ctx;
}

bool Type::operator==(const Type &comp) const {
    return kind == comp.kind
        && access == comp.access
        && ptr == comp.ptr
        && ref == comp.ref
        && array == comp.array
        && size == comp.size
        && structure == comp.structure
        && custom == comp.custom
        && ctx == comp.ctx;
}