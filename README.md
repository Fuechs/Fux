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
> Fux is still in early development

###### Flawless Unambiguous Programming

## The Beautiful Alternative For C++ 

<p align="center">
    <img src="./assets/snippets/helloworld.png" width=60%/>
    <br>
    <a href="#examples"><img src="https://img.shields.io/badge/-Examples-fcaa68?style=for-the-badge"/></a>
    <a href="#state"><img src="https://img.shields.io/badge/-Language_State-fcaa68?style=for-the-badge" /></a>
    <a href="./docs/README.md"><img src="https://img.shields.io/badge/-Documentation-fcaa68?style=for-the-badge" /></a>
</p>

## Why Fux?

### Flexible, Portable, Fast

- At least as high performant as C++[^1]
- Using LLVM to compile and run on several platforms

### Easy To Learn

- Already known environment for developers with experience in C++
- Pythonic features: automatic typing, inbuilt string type, simple arrays, safe memory managment

### Large Standard Library

- Fux ships with a large standard library packed with utility ready to be used
- The Core Library includes but is not limited to[^3]
    - Console I/O
    - File I/O
    - Mathematics
    - Algorithms

### More Readable Code

- Variable and function types are declared after the name, making reading the code natural
- The pipe operators (`<<`, `>>`) are a core feature of Fux and make it easy to avoid nesting functions[^3]
- Variables, functions and classes can be defined without the usage of a single keyword[^2]

[^1]: As long as the compiler does not have to use garbage collection and take over a lot of work from you.

[^2]: As long as automatic typing is used.

[^3]: More information [here](./docs/README.md).

## State 

### Roadmap

- [&check;] `Error System`
- [&check;] `Lexer`
- [...] `Parser`
- [&cross;] `AST Analysis`
- [&cross;] `IR Generation (LLVM)`
- [&cross;] `Optimization (LLVM)`
- [&cross;] `Compilation (LLVM)`
- [...] `Runtime`
- [...] `Syntax`
- [...] `Standard Library`

Fux is currently still in early development. There is no working compiler avaiable for this language. 

As of now, I am focusing on refining the syntax of Fux, since some aspects are not yet clear.
I have been inspired by [C++](https://isocpp.org/), [D](https://dlang.org/), [Python](https://python.org/), [Odin](https://odin-lang.org/) and [Elixir](https://elixir-lang.org/).

## Examples

### Arrays

<img src="./assets/snippets/arrays.png" width=60%>

### Reading Files

<img src="./assets/snippets/readfile.png" width=60%>

### Formatted Print

<img src="./assets/snippets/format.png" width=60%>

### Fibonacci

<img src="./assets/snippets/fibonacci.png" width=60%>

---