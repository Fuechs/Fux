# Design Documentation

> Documentation for the current version of the Fux programming language. 
>
> **Status:** \
> Last updated on 2022-11-14 \
> Unfinished

## Contents

- [Introduction](#introduction)


## Introduction

> Please note that the syntax is still evolving a lot and may look different in the future.

```rust
// takes reference to a string
// and increments its (i32) value 
incrementValue(value -> str): void {
    value = (str) (++(i32) value);
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

> Soon ...

---