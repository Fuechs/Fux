/**
 * @file debug.cpp
 * @author fuechs
 * @brief debug function for every class
 * @version 0.1
 * @date 2022-12-24
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#include "color.hpp"

#include "../frontend/lexer/token.hpp"
#include "../frontend/lexer/lexer.hpp"
#include "../frontend/parser/ast.hpp"
#include "../frontend/parser/parser.hpp"
#include "../frontend/analyser/analyser.hpp"
#include "../frontend/error/error.hpp"
#include "../backend/context/context.hpp"
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
        cout << ";";
    }
    cout << endl;
}

void NumberExprAST::debugPrint() {
    switch (value.type.kind) {
        case FuxType::I8:       cout << value.__i8; break;
        case FuxType::U8:       cout << value.__u8; break;
        case FuxType::C8:       cout << "'" << value.__c8 << "'"; break;
        case FuxType::I16:      cout << value.__i16; break;
        case FuxType::U16:      cout << value.__u16; break;
        case FuxType::C16:      cout << "'" << value.__c16 << "'"; break;
        case FuxType::I32:      cout << value.__i32; break;
        case FuxType::U32:      cout << value.__u32; break;
        case FuxType::F32:      cout << value.__f32; break;
        case FuxType::I64:      cout << value.__i64; break;
        case FuxType::U64:      cout << value.__u64; break;
        case FuxType::F64:      cout << value.__f64; break;
        case FuxType::STR:      cout << '"' << value.__str << '"'; break;
        default:                cout << "?";
    }
}

void VariableExprAST::debugPrint() { cout << name; }

void BinaryExprAST::debugPrint() {
    cout << "(";
    LHS->debugPrint();
    cout << " " << op << " ";
    RHS->debugPrint();
    cout << ")";
}

void ComparisonExprAST::debugPrint() { 
    cout << "(";
    LHS->debugPrint();
    cout << " " << comp << " ";
    RHS->debugPrint();
    cout << ")";
}

void LogicalExprAST::debugPrint() { 
    cout << "(";
    LHS->debugPrint();
    cout << " " << logical << " ";
    RHS->debugPrint();
    cout << ")";
 }

void CallExprAST::debugPrint() { 
    cout << callee << "( ";
    for (ExprPtr &element : args) {
        element->debugPrint();
        cout << ", ";
    }
    cout << ")";
}

void VariableDeclAST::debugPrint() {
    cout << symbol << type.str();
    if (value) {
        cout << " = ";
        value->debugPrint();
    }
}

void PutsCallAST::debugPrint() {
    cout << "puts ";
    argument->debugPrint();
}

void IfElseAST::debugPrint() {
    cout << "if (";
    condition->debugPrint();
    cout << ") ";
    thenBody->debugPrint();
    cout << " else ";
    elseBody->debugPrint();
}

void PrototypeAST::debugPrint() {
    cout << name << "( ";
    for (auto &param : args)
        cout << param.second.suffix() << ", ";
    cout << ")" << type.str();
}

void FunctionAST::debugPrint() { 
    cout << proto->getName() << "( ";
    for (auto &param : proto->getArgs()) 
        cout << param.first << param.second.str() << ", ";
    cout << ")"<< proto->getType().str() << "{\n";
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

// * CONTEXT

void FuxContext::debugPrint(const string message) {
    if (!fux.options.debugMode)
        return;
    
    cout << debugText << "FuxContext";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

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