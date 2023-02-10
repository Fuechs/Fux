# `src/` Tree

> Last updated on 2023-01-27

```
src
├── README.md - this file
├── backend
│   ├── compiler
│   │   ├── compiler.cpp
│   │   └── compiler.hpp 
│   ├── context
│   │   ├── context.cpp - context manager impl.
│   │   ├── context.hpp - context manager
│   │   ├── fuxio.cpp - fux io operations impl. (llvm ir)
│   │   ├── fuxio.hpp - fux io operations (llvm ir)
│   │   ├── fuxmem.cpp - fux memory operations impl (llvm ir)
│   │   ├── fuxmem.hpp - fux memory operations (llvm ir)
│   │   ├── fuxstr.cpp - fux string type impl. (llvm ir)
│   │   └── fuxstr.hpp - fux string type (llvm ir)
│   ├── generator
│   │   ├── codegen.cpp - StmtAST::codegen() impl.
│   │   ├── genarray.hpp - "template" class for array types (llvm ir)
│   │   ├── generator.cpp - generator impl.
│   │   ├── generator.hpp - generator
│   │   ├── gentype.cpp - Generator::getType & FuxArray impl. (llvm ir)
│   │   ├── wrapper.cpp - LLVMWrapper impl.
│   │   └── wrapper.hpp - custom LLVMWrapper for StmtAST::codegen()
│   └── llvmheader.hpp - includes for llvm headers & type definitions
├── examples - example fux programs
├── frontend
│   ├── analyser 
│   │   ├── analyser.cpp - AST analyser impl.
│   │   ├── analyser.hpp - AST analyser 
│   │   └── expectation.hpp - expectation struct
│   ├── ast
│   │   ├── ast.cpp - AST & BinaryOp/UnaryOp/Inbuilts impl.
│   │   ├── ast.hpp - Abstract Syntax Tree
│   │   ├── expr.hpp - ExprAST base
│   │   ├── op.hpp - BinaryOp/UnaryOp/Inbuilts
│   │   ├── position.hpp - Position struct
│   │   └── stmt.hpp - StmtAST base
│   ├── error
│   │   ├── error.cpp - ErrorManager impl.
│   │   ├── error.hpp - ErrorManager
│   │   ├── note.cpp - ErrorNote impl.
│   │   ├── note.hpp - ErrorNote
│   │   ├── parseerror.cpp - ParseError impl.
│   │   └── parseerror.hpp - ParseError
│   ├── lexer
│   │   ├── lexer.cpp - Lexer impl.
│   │   ├── lexer.hpp - Lexer 
│   │   ├── token.cpp - Token impl. 
│   │   └── token.hpp - Token
│   └── parser
│       ├── parser.cpp - Parser impl.
│       ├── parser.hpp - Parser
│       ├── type.cpp - FuxType impl.
│       ├── type.hpp - FuxType
│       ├── value.cpp - ValueStruct impl.
│       └── value.hpp - ValueStruct
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