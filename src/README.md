# `src/` Tree

> Last updated on 2023-03-22

```
src
├── README.md - this file
├── backend
│   ├── compiler
│   │   ├── compiler.cpp - compiler impl.
│   │   └── compiler.hpp - compiler header
│   ├── context
│   │   ├── context.cpp - backend manager impl.
│   │   └── context.hpp - backend manager header
│   ├── generator
│   │   ├── codegen.cpp - StmtAST::codegen() impl.
│   │   ├── generator.cpp - generator impl.
│   │   ├── generator.hpp - generator
│   │   └── gentype.cpp - Generator::getType() impl.
│   ├── eisdrache - Eisdrache Wrapper for the LLVM API
│   │   ├── eisdrache.cpp
│   │   └── eisdrache.hpp
│   └── llvmheader.hpp - includes for llvm headers & type definitions
├── examples - example fux programs
├── frontend
│   ├── analyser 
│   │   ├── analyser.cpp - AST analyser impl.
│   │   ├── analyser.hpp - AST analyser 
│   │   ├── scope.cpp - Scope header
│   │   ├── scope.hpp - Scope impl.
│   │   ├── symbol.cpp - symbol header
│   │   └── symbol.hpp - symbol impl.
│   ├── ast
│   │   ├── ast.cpp - AST & BinaryOp/UnaryOp/Inbuilts impl.
│   │   ├── ast.hpp - Abstract Syntax Tree
│   │   ├── expr.hpp - ExprAST base
│   │   ├── op.hpp - BinaryOp/UnaryOp/Inbuilts
│   │   └── stmt.hpp - StmtAST base
│   ├── error
│   │   ├── error.cpp - ErrorManager impl.
│   │   ├── error.hpp - ErrorManager
│   │   ├── parseerror.cpp - ParseError impl.
│   │   ├── parseerror.hpp - ParseError
│   │   ├── subject.cpp - Subject & Marking 
│   │   └── subject.hpp - Subject & Marking impl.
│   ├── lexer
│   │   ├── lexer.cpp - Lexer impl.
│   │   ├── lexer.hpp - Lexer 
│   │   ├── token.cpp - Token impl. 
│   │   └── token.hpp - Token
│   ├── parser
│   │   ├── parser.cpp - Parser impl.
│   │   ├── parser.hpp - Parser
│   │   ├── type.cpp - FuxType impl.
│   │   ├── type.hpp - FuxType
│   │   ├── value.cpp - ValueStruct impl.
│   │   └── value.hpp - ValueStruct
│   ├── metadata.cpp - Metadata struct impl.
│   └── metadata.hpp - Metadata struct 
├── fux.hpp - standard includes and definitions
├── main.cpp - main file; bootstrap & repl
├── output.ll - llvm module dump (debug)
├── packages - fux included packages
│   └── core - core package
└── util - utility
    ├── color.hpp - ansi codes for output
    ├── debug.cpp - *::debugPrint() impl.
    ├── io.cpp - file io impl.
    ├── io.hpp - file io
    ├── source.cpp - SourceFile impl.
    ├── source.hpp - SourceFile
    ├── threading.cpp - Thread & ThreadManager impl.
    └── threading.hpp - Thread & ThreadManager
```