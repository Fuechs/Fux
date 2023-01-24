## Language State

Fux is currently still in early development. There is no working compiler available for this language. \
**Fux is not supposed to be a superset of C++.**

At the moment I'm concentrating on improving the syntax of Fux, because some aspects are still not clear. The compiler is being developed alongside this process. 

I've been inspired by [C++](https://isocpp.org/), [Python](https://python.org/), [Odin](https://odin-lang.org/), [Elixir](https://elixir-lang.org/) and [Carbon](https://github.com/carbon-language/carbon-lang).

## Roadmap

#### [...] Stage I - Turing Complete in C++

- [...] Syntax
- [&check;] Error System
- [&check;] Source Management
- [&check;] Threading
- [&check;] Lexer
- [...] Abstract Syntax Tree
- [...] Parser
    - [&check;] Value
    - [&check;] Type
    - [...] Parsing
        - [...] Statements
        - [...] Expressions
- [&cross;] Analyser
- [&cross;] Context (LLVM)
- [&cross;] Generator (LLVM)
- [&cross;] Optimizer (LLVM)
- [&cross;] Compiler (LLVM)

I consider the language turing complete once it is able to compile and run [Rule 110](../src/examples/rule110.fux). To set a realistic goal though, it is not planned that Fux will support more complex structures, such as classes, as well as pythonic features at that point. 

###### [&cross;] Stage II - Core Package

###### [&cross;] Stage III - Alpha Release