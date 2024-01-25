<!--    colors 

"orange":   #fcaa68 (sandy brown)
"red":      #ec243c (imperial red)
"grey":     #595959 (davys grey)
"white":    #e0f2e9 (honeydew)
"purple":   #a390e4 (lavender floral)

-->

<div align="center">
<img width="150" src="./assets/logo/fux-material-icon-435.svg" />

## Flawless Unambiguous Programming

[![0.1-dev](https://img.shields.io/badge/0.1--dev-fcaa68?colorA=151515&style=for-the-badge)](./docs/state.md)
[![Apache License v2.0](https://img.shields.io/badge/Apache_License_v2.0-fcaa68?colorA=151515&style=for-the-badge)](./LICENSE)
[![C++][cpp-badge]](./docs/state.md)
[![LLVM][llvm-badge]](./docs/state.md)

</div>
 
###### Simple Hello World Program

```rust
main(argc: u64, argv: **c8): u64 {
    puts "Hello World!\n";
    return 0;
}
```

<div align="center">
<a href="./docs/state.md">Language State</a> ·
<a href="./docs/CONTRIBUTING.md">Contributing</a> ·
<a href="./docs/README.md">Documentation</a> ·
<a href="./docs/examples.md">Examples</a>
</div>
<br>

> __Warning__ \
> Please note that Fux is still in early development. \
> As of now only MacOS and Linux will be supported. \
> \
> **The compiler is currently undergoing a major rework. \
> See the `rework` branch for progress.**

## A Modern, Flexible Programming Language for All

Fux is a modern and flexible programming language, perfect for use in a variety of domains by both novice and expert developers. With its efficient use of LLVM, it provides performance similar to C++, while being portable and running on a wide range of platforms.[^1]

- **Easy to Write and Read** \
    Fux is designed to be easy to write and read, making it accessible for new programmers and offering improved features for experienced developers. Its syntax emphasizes readability and seamless integration of different language elements. For instance, the pipe operators (`<<` and `>>`) provide a simple and intuitive way to connect multiple expressions.

    ```rs
        ptr: *i64;
    //  ---------- Start of lifetime
        reserve i64, 10, ptr;
        ret := process(ptr, ptr + 9);
    //         ------------ ^~~ ---- Last usage of `ptr`
        free ptr;
    //  +~~~~~~~~ 
    //  Inserted by Fux at end of lifetime determined by usage
        return ret;
    ```

- **Eliminating Common C++ Issues** \
    Fux's secure memory management eliminates common issues like memory leaks. Its error system provides precise and informative error messages, making debugging easier. These improvements reduce the likelihood of bugs, enabling developers to produce high-quality code.

    [error demo](assets/errors/error-demo.png)

> __Note__
> - As an ongoing development project, Fux may introduce new features or updates that could impact its performance, stability, or compatibility.
> - Fux may have fewer resources available, such as documentation, forums, or tutorials, compared to more established programming languages.

[^1]: The usage of certain features may result in decreased program performance and slower compile times.

## Getting Started

The compiler for this language is not finished yet. \
You can compile the source code by running `make` and then run the executeable with `./fux <filepath>`. It will output the parsed tokens and AST.

[llvm-badge]: https://img.shields.io/badge/LLVM-4c1717?logo=llvm&logoColor=white&style=for-the-badge
[cpp-badge]: https://img.shields.io/badge/C++-1a3b63?logo=cplusplus&logoColor=white&style=for-the-badge