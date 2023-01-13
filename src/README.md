# File Structure of `src/`

> Last updated on 2023-01-12

```
src
├── README.md - this file
├── backend
│   ├── compiler
│   │   ├── compiler.cpp - compiler impl.
│   │   └── compiler.hpp - compiler
│   ├── context
│   │   ├── context.cpp - context manager impl.
│   │   ├── context.hpp - context manager 
│   │   ├── fuxio.cpp - fux io operations impl. (llvm ir)
│   │   ├── fuxio.hpp - fux io operations (llvm ir)
│   │   ├── fuxmem.cpp - fux memory operations impl. (llvm ir)
│   │   ├── fuxmem.hpp - fux memory operations (llvm ir)
│   │   ├── fuxstr.cpp - fux string type impl. (llvm ir)
│   │   ├── fuxstr.hpp - fux string type (llvm ir)
│   ├── generator 
│   │   ├── genarray.hpp - "template" class for array types
│   │   ├── generator.cpp - generator & ExprAST::codegen impl.
│   │   ├── generator.hpp - generator
│   │   ├── gentype.cpp - Generator::getType & FuxArray::FuxArray impl.
│   │   └── wrapper.hpp - custom LLVMWrapper for codegen
│   └── llvmheader.hpp - includes for llvm headers & type definitions
├── examples - testing / demonstration
│   ├── main.fux
│   ├── rule110.fux - proof for turing completeness
│   └── test.fux
├── frontend
│   ├── analyser - ast semantic analyser
│   │   ├── analyser.cpp - analyser impl.
│   │   └── analyser.hpp - analyser
│   ├── error
│   │   ├── error.cpp - ErrorManager & ParseError impl.
│   │   └── error.hpp - ErrorManager & ParseError
│   ├── lexer
│   │   ├── lexer.cpp - Lexer impl.
│   │   ├── lexer.hpp - Lexer
│   │   └── token.hpp - Token impl.
│   └── parser
│       ├── ast.cpp - AST impl.
│       ├── ast.hpp - Abstract Syntax Tree
│       ├── parser.cpp - Parser impl.
│       ├── parser.hpp - Parser
│       └── type.hpp - FuxType impl.
├── fux.hpp - standard includes and definitions
├── main.cpp - main file; bootstrap & repl
├── output.ll - llvm module dump (for debug)
├── packages - fux included packages
│   └── core - core package
└── util - utility
    ├── color.hpp - ansi codes for output
    ├── debug.cpp - debug functions impl.
    ├── io.cpp - file io impl.
    ├── io.hpp - file io
    ├── source.cpp - SourceFile impl.
    ├── source.hpp - SourceFile 
    ├── threading.cpp - ThreadManager impl.
    └── threading.hpp - ThreadManager
```