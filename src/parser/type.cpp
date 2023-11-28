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
#include "../ast/expr.hpp"

namespace Fux {

Type::Type(Kind kind, AccessList accessList, string name, Metadata meta) 
: kind(kind), access(accessList), name(name), meta(meta) {}

Type::~Type() {
    access.clear();
    name.clear();
}

Type &Type::operator=(const Type &copy) {
    this->kind = copy.kind;
    this->access = copy.access;
    this->name = copy.name;
    this->meta = copy.meta;
    return *this;
}

bool Type::operator==(const Type &comp) const {
    return kind == comp.kind && access == comp.access && name == comp.name;
}

bool Type::operator!() const { return kind == NO_TYPE; }

Type Type::create(Kind kind, AccessList accessList, string name, size_t pointerDepth, bool reference) {
    Type type = Type(kind, accessList, name);

    if (reference)  
        type = Reference(type);

    while (pointerDepth --> 0)
        type = Pointer(type);

    return type;
}

Pointer Type::getPointerTo() { return Pointer(*this); }

string Type::accessAsString(char delim) {
    stringstream ss;

    for (Access &a : access)
        switch (a) {
            case CONSTANT:  ss << "const" << delim; break;
            case PUBLIC:    ss << "pub" << delim; break;
            default:        ss << TokenTypeValue[a] << delim; break;
        }

    return ss.str();
}

string Type::kindAsString() {
    switch (kind) {
        case CUSTOM:    return "'"+name+"'";
        case AUTO:      return "auto";
        case NO_TYPE:   return "no_type";
        default:        return TokenTypeValue[kind];
    }
}

string Type::mangled() {
    string ret = "";
    ret += accessAsString('_');
    ret += '_';
    ret += kindAsString();
    
    return ret;
}

bool Type::valid() {
    // Type is invalid if it is both INTERN and SAFE or there's no name for a custom type provided
    return (std::find(access.begin(), access.end(), INTERN) != access.end() 
        && std::find(access.begin(), access.end(), SAFE) != access.end()) 
        || kind == CUSTOM && name.empty();
}

Pointer::Pointer(Type pointee, AccessList access, Metadata meta) {
    this->pointee = pointee;
    this->access = access;
    this->meta = meta;
}

string Pointer::mangled() {
    return pointee.mangled() + "_ptr";
}

Reference::Reference(Type reference, AccessList access, Metadata meta) {
    this->reference = reference;
    this->access = access;
    this->meta = meta;
}

string Reference::mangled() {
    return reference.mangled() + "_ref";
}

Array::Array(Type member, Expr::Ptr size, Metadata meta) : member(member), size(size) {
    this->meta = meta;
}

string Array::mangled() {
    return member.mangled() + "_arr";
}

}