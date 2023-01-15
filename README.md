<!--    colors 

"orange":   #fcaa68 (sandy brown)
"red":      #ec243c (imperial red)
"grey":     #595959 (davys grey)
"white":    #e0f2e9 (honeydew)
"purple":   #a390e4 (lavender floral)

-->

<p align="center">
    <img src="./assets/fux-material-icon.svg" width=25% />
</p>

![total lines](https://aschey.tech/tokei/github/fuechs/fux?labelColor=151515&color=fcaa68&style=for-the-badge)
![top language](https://img.shields.io/github/languages/top/fuechs/fux?labelColor=151515&color=fcaa68&style=for-the-badge)
![version](https://img.shields.io/badge/version-alpha-fcaa68?colorA=151515&style=for-the-badge)
[![license](https://img.shields.io/badge/license-Apache%20License%20v2.0-fcaa68?labelColor=151515&style=for-the-badge)](./LICENSE)

> __Warning__ \
> Fux is still in early development. \
> As of now only MacOS & Linux will be supported.

###### Flawless Unambiguous Programming

## The Beautiful Alternative for C++

```rust
main(): i64 {
    puts "Hello World!\n";
    return 0;
}
```

<p align="center">
    <a href="#examples"><img src="https://img.shields.io/badge/-Examples-fcaa68?style=for-the-badge"/></a>
    <a href="#state"><img src="https://img.shields.io/badge/-Language_State-fcaa68?style=for-the-badge" /></a>
    <a href="./docs/README.md"><img src="https://img.shields.io/badge/-Documentation-fcaa68?style=for-the-badge" /></a>
    <a href="./CONTRIBUTING.md"><img src="https://img.shields.io/badge/-Contributing-fcaa68?style=for-the-badge" /></a>
</p>

## Why Fux?

### Flexible, Portable, Fast

- At least as high performant as C++[^1][^4]
- Using LLVM to compile and run on several platforms

### Easy to Learn

- Already known environment for developers with experience in C++
- Pythonic features: automatic typing, inbuilt string type, simple arrays, safe memory management, optional dynamic types[^4][^5]

### Large Standard Library

- Fux ships with a large standard library packed with utility ready to be used
- The Core Library includes but is not limited to[^3]
    - Buffered Console I/O
    - File I/O
    - Mathematics
    - Algorithms
    - Key-Value-Maps
    - Dynamic Pointers[^5]
    - String Utility

### More Readable Code

- Variable and function types are declared after the name, making reading the code natural
- The pipe operators (`<<`, `>>`) are a core feature of Fux and make it easy to avoid nesting functions[^3]
- Variables, functions and classes can be defined without the usage of a single keyword[^2][^4]

[^1]: As long as not a lot of automatisation is used.

[^2]: As long as automatic typing is used.

[^3]: More information [here](./docs/README.md).

[^4]: Feature(s) might slow down the execution and / or compilation of your program.

[^5]: Feature(s) are not included for sure in the final language.

## State 

### Roadmap

- **Stage I** --- [`Turing Complete in C++`](./src/examples/rule110.fux)
    - [...] [`Syntax`](./docs/)
    - [&check;] [`Error System`](./src/frontend/error/)
    - [&check;] [`Source Management`](./src/util/source.hpp)
    - [&check;] [`Threading`](./src/util/threading.hpp)
    - [&check;] [`Lexer`](./src/frontend/lexer/)
    - [...] [`AST`](./src/frontend/ast/)
    - [...] [`Parser`](./src/frontend/parser/)
        - [&check;] [`Value`](./src/frontend/parser/value.hpp)
        - [&check;] [`Type`](./src/frontend/parser/type.hpp)
        - [...] [`Parsing`](./src/frontend/parser/parser.hpp)
    - [&cross;] [`Analyser`](./src/frontend/analyser/)
        - [&cross;] [`Expectation`](./src/frontend/analyser/expectation.hpp)
    - [...] [`Context (LLVM)`](./src/backend/context/)
        - [...] [`Memory`](./src/backend/context/fuxmem.hpp)
        - [...] [`String`](./src/backend/context/fuxstr.hpp)
        - [...] [`Input / Output`](./src/backend/context/fuxio.hpp)
    - [...] [`Generator (LLVM)`](./src/backend/generator/)
        - [...] [`Array Types`](./src/backend/generator/genarray.hpp)
        - [...] [`Code Generation`](./src/backend/generator/generator.cpp)
    - [&cross;] [`Optimizer (LLVM)`](./src/backend/)
    - [&cross;] [`Compiler (LLVM)`](./src/backend/compiler/)
- **Stage II** --- [`Core Package`](./src/packages/core/)
- **Stage III** --- `Alpha Release`
- *Soon ...*

> __Note__ \
> The Fux programming language is under the [Apache License v2.0 with LLVM Exceptions](./LICENSE).       

Fux is currently still in early development. There is no working compiler available for this language. \
**Fux is not supposed to be a superset of C++.**

As of now, I am focusing on refining the syntax of Fux, since some aspects are not yet clear. The compiler is being developed alongside this process. 

I have been inspired by [C++](https://isocpp.org/), [Python](https://python.org/), [Odin](https://odin-lang.org/), [Elixir](https://elixir-lang.org/) and [Carbon](https://github.com/carbon-language/carbon-lang).

## Contributing

More information [here](./CONTRIBUTING.md).

## Examples

### Arrays

```rust
array: i32[]; // {}
array[] << 1; // {1}
array[] << 3; // {1, 3}
```

### Reading Files

```rust
get core.file;

using file;
...
filePath := "path/to/file.txt";
file: File(filePath, 'r');
contents := file.read();
```

### Formatted Print

```rust
get core.io;
...
res := 1;
io.putf("Result: %i", res);
```

### Fibonacci

```rust
get core.io;

fib(num: i32): i32 {
    if (num < 2)
    	return num;    
    return fib(num-1) + fib(num-2);
}

main(): i64 {
    io.getint() >> fib() >> io.putln();
    return 0;
}
```

---