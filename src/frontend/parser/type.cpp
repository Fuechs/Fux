/**
 * @file type.cpp
 * @author fuechs
 * @brief fux type
 * @version 0.1
 * @date 2023-01-15
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#include "type.hpp"

FuxType::FuxType(const FuxType &copy) { operator=(copy); }

FuxType::~FuxType() {
    access.clear();
    name.clear();
    delete &arraySize;
}

FuxType &FuxType::operator=(const FuxType &copy) {
    this->kind = copy.kind;
    this->pointerDepth = copy.pointerDepth;
    this->access = copy.access;
    this->array = copy.array;
    this->arraySize = std::move(arraySize);
    return *this;
}

bool FuxType::operator==(const FuxType &comp) {
    return (kind == comp.kind 
            && access == comp.access 
            && array == comp.array 
            && arraySize == comp.arraySize);
}

bool FuxType::operator!() { return kind == NO_TYPE; }

constexpr FuxType FuxType::createStd(Kind kind, int64_t pointerDepth, AccessList accessList, string name) {
    return FuxType(kind, pointerDepth, accessList, false, nullExpr, name);
}

constexpr FuxType FuxType::createRef(Kind kind, AccessList accessList, string name) {
    return FuxType(kind, -1, accessList, false, nullExpr, name); 
}

constexpr FuxType FuxType::createArray(Kind kind, int64_t pointerDepth, AccessList accessList, string name, ExprPtr &arraySize = nullExpr) {
    return FuxType(kind, pointerDepth, accessList, true, arraySize, name);
}

string FuxType::prefix() {
    switch (pointerDepth) {
        case -1:    return " -> ";
        case 0:     return ": ";
        default: {
            stringstream ss;
            ss << ": ";
            for (size_t pd = pointerDepth; pd --> 0;)
                ss << "*";
            ss << " ";
            return ss.str();
        }
    }
}

string FuxType::suffix() {
    stringstream ss;

    if (kind == CUSTOM)
        ss << "'" << name << "'";
    else
        ss << KindString[kind];
        
    if (array) 
        ss << "[" << arraySize << "]";
        
    ss << " (";
    for (Access &a : access)
        ss << AccessString[a] << ",";
    ss << ") ";
    return ss.str();
}

constexpr string FuxType::str() { return prefix() + suffix(); }

bool FuxType::valid() {
    if (find(access, INTERN) != access.end() && find(access, SAFE) != access.end())
        return false;

    if (kind == CUSTOM && name.empty())
        return false;

    if (array && arraySize == nullExpr)
        return false;

    return pointerDepth >= -1;
}