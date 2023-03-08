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

FuxType::~FuxType() {
    access.clear();
    name.clear();
}

// FuxType::FuxType(const FuxType &copy) {}

FuxType &FuxType::operator=(const FuxType &copy) {
    this->kind = copy.kind;
    this->pointerDepth = copy.pointerDepth;
    this->access = copy.access;
    this->name = copy.name;
    this->array = copy.array;
    this->sizeID = copy.sizeID;
    return *this;
}

bool FuxType::operator==(const FuxType &comp) const {
    return (kind == comp.kind 
            && access == comp.access 
            && array == comp.array 
            /* && sizeID == comp.sizeID (shouldn't be required to be equal as of now)*/ );
}

bool FuxType::operator!() { return kind == NO_TYPE; }

FuxType FuxType::createStd(Kind kind, size_t pointerDepth, bool reference, AccessList accessList, string name) {
    return FuxType(kind, pointerDepth, reference, accessList, false, -1, name);
}

FuxType FuxType::createRef(Kind kind, size_t pointerDepth, AccessList accessList, string name) {
    return FuxType(kind, pointerDepth, true, accessList, false, -1, name); 
}

FuxType FuxType::createArray(Kind kind, size_t pointerDepth, bool reference, AccessList accessList, string name, _i64 sizeID) {
    return FuxType(kind, pointerDepth, reference, accessList, true, sizeID, name);
}

FuxType FuxType::createPrimitive(Kind kind, size_t pointerDepth, bool array, string name) {
    return FuxType(kind, pointerDepth, false, AccessList(), array, -1, name);
}


string FuxType::accessAsString() {
    stringstream ss;

    for (Access &a : access)
        switch (a) {
            case CONSTANT:  ss << "const "; break;
            case PUBLIC:    ss << "pub "; break;
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

void FuxType::debugPrint(bool primitive) {
    if (primitive) {
        for (size_t pd = pointerDepth; pd --> 0;)
            cout << "*";
        cout << kindAsString();
        if (array)
            cout << "[]";
        return;
    }

    cout << (reference ? " -> " : ": ");
    cout << accessAsString();

    for (size_t pd = pointerDepth; pd --> 0;)
        cout << "*";
    
    cout << kindAsString();
    
    if (array) {
        cout << "[";
        if (sizeID > -1)
            cout << SC::UNDERLINE << CC::YELLOW 
                << sizeID << SC::RESET << CC::DEFAULT;
        cout << "]";
    }
}

bool FuxType::valid() {
    if (std::find(access.begin(), access.end(), INTERN) != access.end() 
    && std::find(access.begin(), access.end(), SAFE) != access.end())
        return false;

    if (kind == CUSTOM && name.empty())
        return false;

    // OUTDATED: arraySize is now sizeID
    // TODO: "expectation" struct for analyser 
    // may look something like this:
    // Expectation e( integer, binary expression, expression yielding integer, nullExpr, ... );
    // if (array && !arraySize->analyse(e))
    //      return false;

    return pointerDepth >= -1;
}