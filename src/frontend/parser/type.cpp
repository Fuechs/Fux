/**
 * @file type.cpp
 * @author fuechs
 * @brief fux type
 * @version 0.1
 * @date 2023-01-15
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "type.hpp"

FuxType::FuxType(const FuxType &copy) { operator=(copy); }

FuxType::~FuxType() {
    access.clear();
    name.clear();
}

FuxType &FuxType::operator=(const FuxType &copy) {
    this->kind = copy.kind;
    this->pointerDepth = copy.pointerDepth;
    this->access = copy.access;
    this->name = copy.name;
    this->array = copy.array;
    this->arraySize = std::move(arraySize);
    return *this;
}

bool FuxType::operator==(const FuxType &comp) const {
    return (kind == comp.kind 
            && access == comp.access 
            && array == comp.array 
            && arraySize == comp.arraySize);
}

bool FuxType::operator!() { return kind == NO_TYPE; }

FuxType FuxType::createStd(Kind kind, int64_t pointerDepth, AccessList accessList, string name) {
    return FuxType(kind, pointerDepth, accessList, false, nullExpr, name);
}

FuxType FuxType::createRef(Kind kind, AccessList accessList, string name) {
    return FuxType(kind, -1, accessList, false, nullExpr, name); 
}

FuxType FuxType::createArray(Kind kind, int64_t pointerDepth, AccessList accessList, string name, ExprPtr &arraySize) {
    return FuxType(kind, pointerDepth, accessList, true, arraySize, name);
}

string FuxType::accessAsString() {
    stringstream ss;

    for (Access &a : access)
        switch (a) {
            case CONSTANT:  ss << "const "; break;
            case PUBLIC:    ss << "public "; break;
            default:        ss << TokenTypeValue[a] << " "; break;
        }

    return ss.str();
}

string FuxType::kindAsString() {
    switch (kind) {
        case CUSTOM:    return "'"+name+"'";
        case AUTO:      return "auto";
        case NO_TYPE:   return "no_type";
        default:        return TokenTypeValue[kind];
    }
}

void FuxType::prefix() {
    cout << accessAsString();
    
    if (pointerDepth == -1)
        return;
        
    for (size_t pd = pointerDepth; pd --> 0;)
        cout << "*";
}

void FuxType::suffix() {    
    switch (pointerDepth) {
        case -1:    cout << " -> "; break;
        default:    cout << ": "; break;
    }
    
    cout << kindAsString();   
    
    if (array) {
        cout << "[";
        if (arraySize != nullExpr)
            arraySize->debugPrint();
        cout << "]";
    }
}

void FuxType::debugPrint(const string& name) {
    prefix();
    cout << name;
    suffix();
}

bool FuxType::valid() {
    if (std::find(access.begin(), access.end(), INTERN) != access.end() 
    && std::find(access.begin(), access.end(), SAFE) != access.end())
        return false;

    if (kind == CUSTOM && name.empty())
        return false;

    // TODO: "expectation" struct for analyser
    // may look something like this:
    // Expectation e( integer, binary expression, expression yielding integer, nullExpr, ... );
    // if (array && !arraySize->analyse(e))
    //      return false;

    return pointerDepth >= -1;
}