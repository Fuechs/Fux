# Design Documentation
<!-- 
    Copyright (c) 2020-2022, Fuechs. 
    All rights reserved.
-->

> This is a design documentation for the current version of the Fux programming language. 
>
> This documentation is not meant to teach you programming, but rather be a reference to the syntax and how things are designed for me and you.
>
> **Status:** \
> Last updated on 2022-11-15 \
> Unfinished

## Contents

> Sections that are *italic* are still in progress.

- [Introduction](#introduction)
    - [Basic Example](#basic-example)
    - [*Design Goals*](#design-goals)
- [*Variables*](#variables)
- [*Types*](#types)
- [New Concepts](#new-concepts)
    - [Pipe Operators](#pipe-operators)
    - [Swap Operator](#swap-operator)

## Introduction

> Please note that the syntax is still evolving a lot and may look different in the future.

### Basic Example

```rust
// takes reference to a string
// and increments its (i32) value 
incrementValue(value -> str): void {
    value = (str) ++((i32) value);
}

numStr := "1";
incrementValue(numStr);
puts numStr;
```

> Yes this function is useless, but it simply works. Isn't that cool? :P

Fux is supposed to look similar to C++, but still feel modern and flexible enough to qualify as a modern language. \
That includes removing redundant code and adding more inbuilt features.

---

```rust
// takes reference to a string
// and increments its (i32) value
```

Let's start with the simplest thing in this example: \
Comments have to be either on a line that starts with `//`. \
Multiline comments start with `/*` and end with `*/`.

```rust
/*
    This is a comment
    - a multiline comment
*/
```

---

```rust
incrementValue(value -> str): void { ... }
```

You might have already noticed that functions and variables are defined without are preceding keyword. \
A basic function definition only needs an identifier and some operators:

```rust
incrementValue();
```

Between the paranthesis `()` belong the parameters of the function. (optional)

```rust
incrementValue(value -> str);
```

Parameters are defined just like normal variables. \
All they require is an identifier (`value`). The pointer `->` denotes a reference (to the type of `str`). \
A normal variable is defined with a colon `:`:

```rust
value: str;
```

See [`# Variables`](#variables) for more information.

References will point to the passed variables. Modifying the refrence will thus modify the original variable that was passed to the function.

```rust
originalValue: str = "1";
pointerValue -> str = originalValue;
pointerValue = "2";
// originalValue is now equal to "2"
```

See [`# Pointer & References`](#pointer--references) for more information.

```rust
incrementValue( ... ): void;
```

After the closing paranthesis `)` and a colon `:`, the type of the returned value can be defined. In this case, void means that the function will not return any value.

---

```rust
{   value = (str) ++((i32) value);  }
```

The functions body has to be defined as a code block, inside of braces `{}`. \
The code here is a little more compilcated than the rest of the code.

First of all, we redefine the reference `value`. The assigned value is an expression using type casts and the increment operator.

```rust
(str) ++((i32) value)
```

Types can be converted using `( <type> )` before the value itself. In this case, we convert the references value into a signed 32-bit integer (See [`# Integer Types`](#integer-types) for more Information). \
Using the `++` operator, we increment the value (by 1). Then it simply gets converted back to `str`.

---

```rust
numStr := "1";
```

Here we define a variable with the value "1". To save time and space, we can simply leave out the type between the colon `:` and the equals `=` to let the compiler figure out the type for us.

--- 

```rust
incrementValue(numStr);
```

The function we previously defined gets called with the `numStr` variable. Since it directly modifies the value of our variable, we do not have to save any returned values.

---

```rust
puts numStr;
```

Finally, we output the result to see if our program actually works -- and yes it does, the output is "2". \
For that, we use the `puts` keyword, which will directly output a string to the console (`putch`: Output a character to the console).

### Design Goals

> Soon ...

## Variables

> Soon...

### Declaration

### Pointer & References

## Types

> Soon...

### Boolean Type

### Integer Types

### Character Types

### String Type

## New Concepts

Of course, Fux also offers some quality of life features and little improvements over C++, making the overall programming experience more enjoyable.

### Pipe Operators

The pipe operators `<<` and `>>` can be used to pass around  function arguments and for example push / append values to an array.

```rust
"someString" >> someFunction();
// same as someFunction("someString");
```

```rust
someArray: i32[];
someArray[] << 1; // {1}
2 >> someArray[] << 3; // {2, 1, 3}
6 >> 7 >> someArray[] << 4 << 5; // {6, 7, 2, 1, 3, 4, 5}
//                 ^^ without brackets, fux would 
//                  try to assign a value to someArray 
//                  someArray << {}; 
//                  => someArray = {}
```

### Swap Operator

The swap operator `<>` can be used to swap the value of two variables. 

```rust
first: bool = true;
second: bool = false;

first <> second;
// first is now false
// second is now true
```

```rust
array1: i32[] = {0, 1, 2, 3, 4};
array2: i32[] = {5, 6, 7, 8, 9};

array1 <> array2;
// array1 = {5, 6, 7, 8, 9}
// array2 = {0, 1, 2, 3, 4}
array1[2] <> array2[2];
// array1 = {5, 6, 2, 8, 9}
// array2 = {0, 1, 7, 3, 4}
//                 ^
array1[2] <> array2[1];
// array1 = {5, 6, 1, 8, 9}
//                 ^
// array2 = {0, 2, 7, 3, 4}
//              ^
```