/**
 * @file debug.cpp
 * @author fuechs
 * @brief debug function for every class
 * @version 0.1
 * @date 2022-12-24
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "color.hpp"

#include "../frontend/lexer/token.hpp"
#include "../frontend/lexer/lexer.hpp"
#include "../frontend/ast/ast.hpp"
#include "../frontend/parser/parser.hpp"
#include "../frontend/parser/value.hpp"
#include "../frontend/analyser/analyser.hpp"
#include "../frontend/error/error.hpp"
#ifdef FUX_BACKEND
#include "../backend/context/context.hpp"
#include "../backend/generator/generator.hpp"
#include "../backend/compiler/compiler.hpp"
#include "threading.hpp"
#endif

// * LEXER

void Lexer::debugPrint() {
    if (!fux.options.debugMode)
        return;
    
    cout << debugText << "Lexer:\n";
    for (Token &token : tokens)
        cout << token.str() << "\n";
}

// * PARSER

void debugIndent(size_t indent, string message = "") {
    while (indent --> 0)
        cout << "   ";
    if (!message.empty())
        cout << message;
}

void callASTDebug(size_t indent, StmtPtr &ast) {
    debugIndent(indent);
    if (ast)
        ast->debugPrint(indent);
    else
        cout << CC::RED << "NULLSTMT" << CC::DEFAULT;
}

void callASTDebug(size_t indent, ProtoPtr &ast) {
    debugIndent(indent);
    if (ast)
        ast->debugPrint(indent);
    else
        cout << CC::RED << "NULLPROTO" << CC::DEFAULT;
}

void callASTDebug(size_t indent, ExprPtr &ast) {
    debugIndent(indent);
    if (ast)
        ast->debugPrint();
    else
        cout << CC::RED << "NULLEXPR" << CC::DEFAULT;
}

void debugBody(size_t indent, StmtPtr &ast) {
    debugIndent(indent);
    if (ast) {
        if (ast->getASTType() == AST::CodeBlockAST)
            ast->debugPrint(indent);
        else
            ast->debugPrint(indent + 1);
    } else 
        cout << CC::RED << "NULLBODY" << CC::DEFAULT;
} 

void NullExprAST::debugPrint(size_t indent) { debugIndent(indent, "null"); }

void BoolExprAST::debugPrint(size_t indent) { 
    debugIndent(indent);
    value->debugPrint(); 
}

void NumberExprAST::debugPrint(size_t indent) { 
    debugIndent(indent);
    value->debugPrint(); 
}

void CharExprAST::debugPrint(size_t indent) { 
    debugIndent(indent);
    value->debugPrint(); 
}

void StringExprAST::debugPrint(size_t indent) { 
    debugIndent(indent);
    value->debugPrint(); 
}

void VariableExprAST::debugPrint(size_t indent) { debugIndent(indent, name); }

void MemberExprAST::debugPrint(size_t indent) {
    callASTDebug(indent, base);
    cout << ".";
    callASTDebug(0, member);
}

void UnaryExprAST::debugPrint(size_t indent) {
    debugIndent(indent, "(");
    if (op == UnaryOp::SINC || op == UnaryOp::SDEC) {
        callASTDebug(0, expr);
        cout << UnaryOpValue(op);
    } else {
        cout << UnaryOpValue(op);
        callASTDebug(0, expr);
    }
    cout << ")";
}

void BinaryExprAST::debugPrint(size_t indent) {
    debugIndent(indent, "(");
    callASTDebug(0, LHS);
    if (op == BinaryOp::IDX) { // handle <expr>[<expr>] and <expr>[]
        if (!RHS) cout << "[]";
        else {        
            cout << "[";
            RHS->debugPrint();
            cout << "]";
        }
    } else {
        cout << " " << BinaryOpValue(op) << " ";
        callASTDebug(0, RHS);
    }
    cout << ")";
}

void CallExprAST::debugPrint(size_t indent) { 
    callASTDebug(indent, callee);
    cout << "(";
    for (ExprPtr &arg : args) {
        callASTDebug(0, arg);
        if (arg != args.back())
            cout << ", ";
    }
    cout << ")";
}

void TypeCastExprAST::debugPrint(size_t indent) {
    debugIndent(indent, "((");
    type.debugPrint(true);
    cout << ") ";
    expr->debugPrint();
    cout << ")";
}

void TernaryExprAST::debugPrint(size_t indent) {
    debugIndent(indent, "(");
    condition->debugPrint();
    cout << " ? ";
    thenExpr->debugPrint();
    cout << " : ";
    elseExpr->debugPrint();
    cout << ")";
}

void VariableDeclAST::debugPrint(size_t indent) {
    debugIndent(indent, symbol);
    type.debugPrint();
    if (value) {
        cout << " = ";
        value->debugPrint();
    }
}

void InbuiltCallAST::debugPrint(size_t indent) {
    debugIndent(indent, InbuiltsValue(callee));
    cout << " ";
    for (ExprPtr &arg : arguments) {
        callASTDebug(0, arg);
        if (arg != arguments.back())
            cout << ", "; 
    }
}

void IfElseAST::debugPrint(size_t indent) {
    debugIndent(indent, "if (");
    callASTDebug(0, condition);
    cout << ")\n";
    debugBody(indent, thenBody);
    if (!elseBody)
        return;
    cout << ";\n";
    debugIndent(indent, "else\n");
    debugBody(indent, elseBody);
} 

void CodeBlockAST::debugPrint(size_t indent) {
    debugIndent(indent, "{\n");
    for (StmtPtr &stmt : body) {
        callASTDebug(indent + 1, stmt);
        cout << ";\n";
    }
    debugIndent(indent*2, "}"); // i have no idea why *2 is required here, but it works
}

void WhileLoopAST::debugPrint(size_t indent) {
    if (postCondition) {
        debugIndent(indent, "do\n");
        debugBody(indent, body);
        cout << "\n";
        debugIndent(indent, "while (");
        callASTDebug(0, condition);
        cout << ")";
        return;
    }

    debugIndent(indent, "while (");
    callASTDebug(0, condition);
    cout << ")\n";
    debugBody(indent, body);
}

void ForLoopAST::debugPrint(size_t indent) {
    debugIndent(indent, "for (");
    callASTDebug(0, initial);
    if (forEach) {
        cout << " in ";
        callASTDebug(0, iterator);
    } else {
        cout << "; ";
        callASTDebug(0, condition);
        cout << "; ";
        callASTDebug(0, iterator);
    }

    cout << ")\n";
    debugBody(indent, body);
}

void PrototypeAST::debugPrint(size_t indent) {
    callASTDebug(indent, symbol);
    cout << "(";
    for (StmtPtr &param : args) {
        callASTDebug(0, param);
        if (param != args.back())
            cout << ", ";
    }
    cout << ")";
    type.debugPrint();
}

void FunctionAST::debugPrint(size_t indent) { 
    callASTDebug(indent, proto);
    cout << "\n";
    debugBody(indent, body);
}

void RootAST::debugPrint(size_t indent) {
    if (!fux.options.debugMode)
        return;

    cout << debugText << "Root AST";
    for (StmtPtr &stmt : program) {
        cout << "\n";
        callASTDebug(0, stmt);
        cout << ";";
    }
    cout << endl;
}

void Parser::debugPrint(const string message) {
    if (!fux.options.debugMode)
        return;

    cout << debugText<< "Parser";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

void ValueStruct::debugPrint() {
    switch (type.kind) {
        case FuxType::BOOL:     cout << (__bool ? "true" : "false"); break;
        case FuxType::I8:       cout << __i8; break;
        case FuxType::U8:       cout << to_string(__u8); break;
        case FuxType::C8:       cout << "'" << __c8 << "'"; break;
        case FuxType::I16:      cout << __i16; break;
        case FuxType::U16:      cout << __u16; break;
        case FuxType::C16:      cout << "'" << to_string(__c16) << "'"; break;
        case FuxType::I32:      cout << __i32; break;
        case FuxType::U32:      cout << __u32; break;
        case FuxType::F32:      cout << __f32; break;
        case FuxType::I64:      cout << __i64; break;
        case FuxType::U64:      cout << __u64; break;
        case FuxType::F64:      cout << __f64; break;
        case FuxType::STR:      cout << '"' << __str << '"'; break;
        default:                cout << "???";
    }
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

#ifdef FUX_BACKEND

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

#endif