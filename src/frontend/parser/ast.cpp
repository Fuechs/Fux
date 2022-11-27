/**
 * @file ast.cpp
 * @author fuechs
 * @brief fux abstract syntax tree 
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "ast.hpp"

void AST::addSub(AST* sub) {
    body.push_back(sub);
}

void AST::copyPosition(AST *from) {
    pos = from->pos;
}

void AST::copyPosition(Token from) {
    pos.first = from.line;
    pos.last = from.line;
    pos.start = from.start;
    pos.end = from.end;
}

void AST::debugPrint(size_t indent, bool all) {

    if (!fux.options.debugMode)
        return;

    string border(30, '-');
    stringstream debug;

    debugIndent(debug, indent);
    debug << border << "\n";
    
    debugIndent(debug, indent);
    debug 
        << ColorCode::MAGENTA 
        << "Type: (" << type << ") " << NodeTypeString[type] << "\n"
        << ColorCode::DEFAULT;

    debugIndent(debug, indent);  
    debug << "Parent: ";

    if (parent != nullptr)
        debug << NodeTypeString[parent->type] << " (Line " << parent->pos.first << ":" << parent->pos.start << ")\n";
    else
        debug << "none\n";


    debugIndent(debug, indent);
    debug << "Line " << pos.first << ":" << pos.start << " to " << pos.last << ":" << pos.end << "\n";
    
    debugIndent(debug, indent);
    debug << "Body Size: " << body.size() << "\n";
    
    debugIndent(debug, indent);
    if (type == AST_VARIABLE_DECl)
        debug << "ValueType: " << TypeString[valueType] << "\n";
    else 
        debug << "Value: " << value << "\n";
    
    cout << debug.str();

    if (!all) {
        cout << border << endl;
        return;
    }
    
    for (AST *sub : body)
        sub->debugPrint(indent + 1);
}

void AST::debugIndent(stringstream &debug, size_t indent) {
    while (indent --> 0)
        debug << "    ";
}

void AST::debugLiteral() {
    if (!fux.options.debugMode)
        return;

    switch (type) {
        case AST_ROOT:
            cout << ColorCode::RED << "-- ROOT --\n" << ColorCode::DEFAULT;
            for (AST *sub : body)
                sub->debugLiteral();
            cout << endl;
            break;

        case AST_NULL_LITERAL:
        case AST_NUMERIC_LITERAL:
        case AST_IDENTIFIER:
        case AST_BINARY_OPERATOR:
            cout << value;
            break;
        
        case AST_BINARY_EXPR:
            cout << "(";
            body[1]->debugLiteral(); // lhs
            cout << " ";
            body[0]->debugLiteral(); // op
            cout << " ";
            body[2]->debugLiteral(); // rhs
            cout << ")";   
            break; 
        
        case AST_VARIABLE_DECl:
            body[0]->debugLiteral(); // name
            if (valueType == VAR)
                cout << " := ";
            else if (valueType == CONSTANT)
                cout << " :=== ";
            body[1]->debugLiteral(); // value
            cout << ";";
            break;

        default:
            break;       
    }
}