/**
 * @file debug.cpp
 * @author fuechs
 * @brief debug function for every class
 * @version 0.1
 * @date 2022-12-24
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "color.hpp"

#include "../frontend/lexer/token.hpp"
#include "../frontend/lexer/lexer.hpp"
#include "../frontend/parser/ast.hpp"
#include "../frontend/parser/parser.hpp"
#include "../frontend/analyser/analyser.hpp"
#include "../frontend/error/error.hpp"
#include "../backend/generator/generator.hpp"
#include "../backend/compiler/compiler.hpp"
#include "threading.hpp"

// * LEXER

void Token::debugPrint() {
    if (!fux.options.debugMode)
        return;

    cout    
        << debugText
        << "<Token, " << TokenTypeString[type] << ", '"  
        << value << "', "
        << "Line " << line << ":" << start << ">\n";
    // <Token, TYPE, 'value', Line ?:?>
}

void Lexer::debugPrint() {
    if (!fux.options.debugMode)
        return;
    
    cout << debugText << "Lexer:\n";
    for (Token &token : tokens)
        token.debugPrint();
}

// * PARSER

void RootAST::debugPrint() {
    if (!fux.options.debugMode)
        return;

    cout << debugText << "Root AST";
    for (ExprPtr &sub : program) {
        cout << "\n";
        sub->debugPrint();
    }
    cout << endl;
}

void NumberExprAST::debugPrint() { cout << value; }

void VariableExprAST::debugPrint() { cout << name; }

void BinaryExprAST::debugPrint() {
    cout << "(";
    LHS->debugPrint();
    cout << " " << op << " ";
    RHS->debugPrint();
    cout << ")";
}

void ComparisonExprAST::debugPrint() { return; }
void LogicalExprAST::debugPrint() { return; }

void CallExprAST::debugPrint() { 
    cout << callee << "( ";
    for (ExprPtr &element : args) {
        element->debugPrint();
        cout << ", ";
    }
    cout << ");";
}

void VariableDeclAST::debugPrint() { return; }

void PrototypeAST::debugPrint() {
    cout << name << "( ";
    for (auto &param : args)
        cout << fuxType::TypeString[param.second] << ", ";
    cout << "): " << fuxType::TypeString[type] << ";";
}

void FunctionAST::debugPrint() { 
    cout << proto->getName() << "( ";
    for (auto &param : proto->getArgs()) 
        cout << param.first << ": " << fuxType::TypeString[param.second] << ", ";
    cout << "): "<< fuxType::TypeString[proto->getType()] << " {\n";
    for (ExprPtr &stmt : body) {
        cout << "\t";
        stmt->debugPrint();
        cout << "\n";
    }
    cout << "}";
}

void Parser::debugPrint(const string message) {
    if (!fux.options.debugMode)
        return;

    cout << debugText<< "Parser";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

// * ANALYSER

void Analyser::debugPrint(const string message) {
    if (!fux.options.debugMode)
        return;

    cout << debugText << "Analyser";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

// * ERROR

void ErrorManager::debugPrint() { return; }

// * GENERATOR

void Generator::debugPrint(const string message) {
    if (!fux.options.debugMode)
        return;

    cout << debugText << "Generator";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

// * COMPILER

void Compiler::debugPrint(const string message) {
    if (!fux.options.debugMode)
        return;
        
    cout << debugText << "Compiler";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

// * THREADING

void fuxThread::Thread::debugPrint(const string message) {  
    if (!fux.options.debugMode)
        return;
            
    cout << debugText << "Thread '" << name << "' (" << id << ")";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

void fuxThread::ThreadManager::debugPrint(const string message) {
    if (!fux.options.debugMode)
        return;
        
    cout << debugText << "ThreadManager";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}