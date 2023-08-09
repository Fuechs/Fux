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
#include "io.hpp"

#include "../lexer/token.hpp"
#include "../lexer/lexer.hpp"
#include "../ast/ast.hpp"
#include "../parser/parser.hpp"
#include "../parser/value.hpp"
#include "../analyser/analyser.hpp"
#include "../error/error.hpp"
#ifdef FUX_BACKEND
#include "../context/context.hpp"
#include "../generator/generator.hpp"
#include "../compiler/compiler.hpp"
#include "threading.hpp"
#endif

// * LEXER

void Lexer::debugPrint() {
    if (!fux.debug)
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

void callASTDebug(size_t indent, Stmt::Ptr &ast) {
    debugIndent(indent);
    if (ast)
        ast->debugPrint(indent);
    else
        cout << CC::RED << "NULLSTMT" << CC::DEFAULT;
}

void callASTDebug(size_t indent, Expr::Ptr &ast) {
    debugIndent(indent);
    if (ast)
        ast->debugPrint();
    else
        cout << CC::RED << "NULLEXPR" << CC::DEFAULT;
}

void debugBody(size_t indent, Stmt::Ptr &ast) {
    debugIndent(indent);
    if (ast) {
        if (ast->getASTType() == AST::BlockStmt)
            ast->debugPrint(indent);
        else
            ast->debugPrint(indent + 1);
    } else 
        cout << CC::RED << "NULLBODY" << CC::DEFAULT;
} 

void NullExpr::debugPrint(size_t indent) { 
    debugIndent(indent, "null"); 
}

void BoolExpr::debugPrint(size_t indent) { 
    debugIndent(indent);
    value->debugPrint(); 
}

void NumberExpr::debugPrint(size_t indent) { 
    debugIndent(indent);
    value->debugPrint(); 
}

void CharExpr::debugPrint(size_t indent) { 
    debugIndent(indent);
    value->debugPrint(); 
}

void StringExpr::debugPrint(size_t indent) { 
    debugIndent(indent);
    value->debugPrint(); 
}

void RangeExpr::debugPrint(size_t indent) {
    callASTDebug(indent, begin);
    cout << "...";
    callASTDebug(0, end);
}

void ArrayExpr::debugPrint(size_t indent) {
    debugIndent(indent, "{");
    for (Expr::Ptr &expr : elements) {
        callASTDebug(0, expr);
        if (expr != elements.back()) 
            cout << ", ";
    }
    cout << "}";
}

void SymbolExpr::debugPrint(size_t indent) { 
    debugIndent(indent, name); 
}

void CallExpr::debugPrint(size_t indent) { 
    if (asyncCall) 
        debugIndent(indent, "async ");
    callASTDebug(asyncCall ? 0 : indent, callee);
    cout << "(";
    for (Expr::Ptr &arg : args) {
        callASTDebug(0, arg);
        if (arg != args.back())
            cout << ", ";
    }
    cout << ")";
}

void MemberExpr::debugPrint(size_t indent) {
    debugIndent(indent, "(");
    callASTDebug(indent, parent);
    cout << ".";
    debugIndent(0, member);
    cout << ")";
}

void UnaryExpr::debugPrint(size_t indent) {
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

void BinaryExpr::debugPrint(size_t indent) {
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

void TypeCastExpr::debugPrint(size_t indent) {
    debugIndent(indent, "((");
    type.debugPrint(true);
    cout << ") ";
    expr->debugPrint();
    cout << ")";
}

void TernaryExpr::debugPrint(size_t indent) {
    debugIndent(indent, "(");
    condition->debugPrint();
    cout << " ? ";
    thenExpr->debugPrint();
    cout << " : ";
    elseExpr->debugPrint();
    cout << ")";
}

void NoOperationStmt::debugPrint(size_t indent) { 
    debugIndent(indent, "noop"); 
} 

void VariableStmt::debugPrint(size_t indent) {
    debugIndent(indent, symbol);
    type.debugPrint();
    
    if (value) {
        cout << " = ";
        value->debugPrint();
    }
}

void InbuiltCallStmt::debugPrint(size_t indent) {
    debugIndent(indent, InbuiltsValue(callee));
    
    if (!arguments.empty()) {
        cout << " ";
        for (Expr::Ptr &arg : arguments) {
            callASTDebug(0, arg);
            if (arg != arguments.back())
                cout << ", "; 
        }
    }
}

void IfElseStmt::debugPrint(size_t indent) {
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

void BlockStmt::debugPrint(size_t indent) {
    debugIndent(indent, "{\n");
    for (Stmt::Ptr &stmt : body) {
        callASTDebug(indent + 1, stmt);
        cout << ";\n";
    }
    debugIndent(indent*2, "}"); // i have no idea why *2 is required here, but it works
}

void WhileStmt::debugPrint(size_t indent) {
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

void ForStmt::debugPrint(size_t indent) {
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

void FunctionStmt::debugPrint(size_t indent) { 
    debugIndent(indent, symbol);
    cout << "(";
    for (Parameter::Ptr &param : parameters) {
        cout << param->symbol;
        param->type.debugPrint();
        if (param->value) {
            cout << " = ";
            callASTDebug(0, param->value);
        }

        if (param != parameters.back())
            cout << ", ";
    }
    cout << ")";
    type.debugPrint();
    cout << "\n";
    debugIndent(indent, "[\n");
    for (Stmt::Ptr &local : locals) {
        callASTDebug(indent + 1, local);
        cout << ";\n";
    }
    debugIndent(indent, "]\n");
    debugBody(indent, body);
}

void EnumStmt::debugPrint(size_t indent) {
    debugIndent(indent, "enum "+symbol+" {\n");
    for (const string &e : elements)
        debugIndent(indent + 1, e+",\n");
    debugIndent(indent, "}");
}

void MacroStmt::debugPrint(size_t indent) {
    debugIndent(indent, "macro "+symbol+" {\n");

    for (Case *&_case : cases) {
        debugIndent(indent + 1, "(");

        for (Arg &arg : _case->args) {
            cout << arg.symbol << ": ";

            switch (arg.type) {
                case TYPE:      cout << "type"; break;
                case IDENT:     cout << "ident"; break;
                case EXPR:      cout << "expr"; break;
                case STMT:      cout << "stmt"; break;
                case BLOCK:     cout << "block"; break;
                case WILDCARD:  cout << "*"; break;
                default:        cout << CC::RED << "NONE" << CC::DEFAULT; break;
            }

            if (arg.array) {
                if (arg.variadic)
                    cout << "[...]";
                else
                    cout << "[]";
            }

            
            if (&arg != &_case->args.back())
                cout << ", ";
        }
        
        cout << ") ->\n";
        callASTDebug(indent + 1, _case->ret);
        cout << ";\n";
    }

    debugIndent(indent, "}");
}

void Root::debugPrint(size_t indent) {
    if (!fux.debug)
        return;

    cout << debugText << "Root AST";
    for (Stmt::Ptr &stmt : program) {
        cout << "\n";
        callASTDebug(0, stmt);
        cout << ";";
    }
    cout << endl;
}

void Stmt::debugPrint(size_t indent) { return; }

void Parser::debugPrint(const string message) {
    if (!fux.debug)
        return;

    cout << debugText<< "Parser";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

void ValueStruct::debugPrint() {
    cout << std::setprecision(20); // display all digits of a float
    switch (type.kind) {
        case FuxType::BOOL:     cout << (__bool ? "true" : "false"); break;
        case FuxType::C8:       cout << "'" << unescapeSequences(&__c8) << "'"; break;
        case FuxType::C16:      cout << "'" << to_string(__c16) << "'"; break;
        case FuxType::I64:      cout << __i64; break;
        case FuxType::U64:      cout << __u64; break;
        case FuxType::F64:      cout << __f64; break;
        case FuxType::LIT:      cout << '"' << unescapeSequences(__lit) << '"'; break;
        default:                cout << "???";
    }
}

// * ANALYSER

#ifdef FUX_ANALYSER

void Analyser::debugPrint(const string message) {
    if (!fux.options.debugMode)
        return;

    cout << debugText << "Analyser";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

#endif

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