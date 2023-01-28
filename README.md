<!--    colors 

"orange":   #fcaa68 (sandy brown)
"red":      #ec243c (imperial red)
"grey":     #595959 (davys grey)
"white":    #e0f2e9 (honeydew)
"purple":   #a390e4 (lavender floral)

-->

<div align="center">
<img width="150" src="./assets/fux-material-icon-435.svg" />

## Flawless Unambiguous Programming

<!-- ![total lines](https://aschey.tech/tokei/github/fuechs/fux?labelColor=151515&color=fcaa68&style=for-the-badge)x -->
[![version](https://img.shields.io/badge/pre--alpha-fcaa68?colorA=151515&style=for-the-badge)](./docs/state.md)
[![Apache License v2.0](https://img.shields.io/badge/Apache_License_v2.0-fcaa68?colorA=151515&style=for-the-badge)](./LICENSE)
[![C++][cpp-badge]](./docs/state.md)
[![LLVM][llvm-badge]](./docs/state.md)

### The Beautiful Alternative for C++

</div>
 
###### Simple Hello World Program

```rust
main(argc: u64, argv: str[]): i64 {
    puts "Hello World\n!";
    return 0;
}
```

<div align="center">
<a href="./docs/state.md">Language State</a> ·
<a href="./docs/contributing.md">Contributing</a> ·
<a href="./docs/README.md">Documentation</a> ·
<a href="./docs/examples.md">Examples</a>
</div>
<br>

> __Warning__ \
> Fux is still in early development. \
> As of now only MacOS & Linux will be supported.

Fux is a static typed language that can be as low-level as Rust and as high-level as Python.

## Why Fux?

### Flexible, Portable, Fast

- At least as high performant as C++[^1][^2]
- Using LLVM to compile and run on several platforms

### Easy to Learn

- Already known environment for developers with experience in C++
- Pythonic features: automatic typing, inbuilt string type, simple arrays, safe memory management, optional dynamic types[^2][^3]

### Large Standard Library

- Fux ships with a large standard library packed with utility ready to be used
- The Core Library includes but is not limited to
    - Buffered Console I/O
    - File I/O
    - Mathematics
    - Algorithms
    - Key-Value-Maps
    - Dynamic Pointers[^3]
    - String Utility

### More Readable Code

- Various adjustments to the syntax make the code more readable and elements of the syntax merge seamlessly
- Pipe operators (`<<` `>>`) are a key feature of Fux and help with making code more readable[^4]
- Variables and functions can be defined without the usage of a single keyword[^2][^5]

[^1]: As long as not a lot of automatisation is used.

[^2]: Feature(s) might slow down the execution and / or compilation of your program.

[^3]: Feature(s) are not included for sure in the final language.

[^4]: More information [here](./docs/README.md).

[^5]: As long as automatic typing is used.

## Getting Started

The compiler for this language is not finished yet. \
You can compile the source code by running `make` and then run the executeable with `./fux <filepath>`. It will output the parsed tokens and AST.

[llvm-badge]: https://img.shields.io/badge/LLVM-4c1717?logo=llvm&logoColor=white&style=for-the-badge
[cpp-badge]: https://img.shields.io/badge/C++-1a3b63?logo=cplusplus&logoColor=white&style=for-the-badge