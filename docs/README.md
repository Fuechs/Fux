# Design Documentation

<!-- 
    Copyright (c) 2020-2023, Fuechs and Contributors. 
    All rights reserved.
-->

> __Note__ \
> This is a design documentation for the current version of the Fux programming language. 
>
> This documentation is not meant to teach you programming, but rather be a reference to the syntax and how things are designed for me and you.
>
> **Status** \
> Last updated on 2023-02-06 \
> Unfinished

## Contents

> Sections that are *italic* are still in progress.

- [Introduction](#introduction)
    - [Basic Example](#basic-example)
    - [Basic IO](#basic-io)
    - [*Design Goals*](#design-goals)
- [Variables](#variables)
    - [Declaration & Defenition](#declaration--definition)
    - [*Pointer & References*](#pointer--references)
- [Types](#types)
    - [Boolean Type](#boolean-type)
    - [Integer Types](#integer-types)
    - [Character Types](#character-types)
    - [String Type](#string-type)
    - [Array Type](#array-type)
    - [Type Casts](#type-casts)
- [New Concepts](#new-concepts)
    - [Pipe Operators](#pipe-operators)
        - [Passing Arguments](#passing-arguments)
        - [Modifying Arrays](#modifying-arrays)
        - [Extending Strings](#extending-strings)
    - [Swap Operator](#swap-operator)
    - [Existence of Objects](#existence-of-objects)

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

References will point to the passed variables. Modifying the reference will thus modify the original variable that was passed to the function.
 
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

### Basic IO

#### Basic Output

```rust
// output a single character (c8)
// putch <value (c8)> ;
character: c8 = '\n';
putch character;

// output a string (str)
// puts <value (str)> ;
message: str = "Hello World!\n";
puts message;
```

#### Basic Input

```rust
// read input until newline 
// read <address (*str)> ;
input: str;
read &input; // address of 'input' = &input
```

### Design Goals

> Soon ...

## Variables

### Declaration & Definition

[`# Types`](#types)

```rust
// <identifier> [ : [ <type> ] =/=== <value> ] ;
name;
name := "fuechs"; // automatic typing
name :=== "fuechs"; // constant
name: str;
name: str = "fuechs";
name: str === "fuechs";

// redefinition
name = "fuechsss";
```

### Pointer & References

```rust
// pointer definition
// <identifier> [ : [ * <type> ] =/=== <address> ] ; 
// address 
// & <variable>

ptr: *str;
ptr: *str = &name;
ptr := &name;


someString = "a";
ptr = &someString; // redefinition

*ptr = "b";
// someString = "b";

puts (str) ptr;
// <someString address>

puts *ptr;
// "b"
```

```rust
// reference definition
// <identiifer> [ -> [ <type> ] =/=== <lvalue> ] ;
name: str = "fuechs";
nameRef -> str = name;
nameRef = "fux"; // name = "fux";
name = "some"; // nameRef = "some";

puts (str) &name;       // <name address>
puts (str) &nameRef;    // <name address>
```

## Types

> A variable declared with the `var` type can hold any type of value. (Not fixed!)
> ```rust
> someVar: var = 1;
> someVar = "1";
> someList: var[] = {1, "2", true, 4.3, 'a'};
> ```

### Boolean Type

- Keyword - `bool`
- Value - `true` or `false`

```rust
truth: bool;
truth = true;
truth = false;
```

### Integer Types

- Keyword - `iN` - signed N-bit integer `(8, 16, 32, 64)`
- Keyword - `uN` - unsigned N-bit integer `(8, 16, 32, 64)`
- Keyword - `fN` - N-bit floating point integer `(32, 64)`
- Value - signed integer, unsigned integer or floating point integer

Integers can be written in following bases:
- Hexadecimal - starting with `0x` `(0-9, a-f|A-F)`
- Decimal - starting with any digit `(0-9)`
- Octal - starting with `0o` `(0-7)`
- Binary - starting with `0b` `(0|1)`

Examples:
```rust
0xfcaa68 // hexadecimal
16558696 // decimal
0o77125150 // octal
0b111111001010101001101000 // binary
```

```rust
someNum: i32 = 1;
someFloat: f32 = 1.0;
```

### Character Types

- Keyword - `cN` - UTF-N code unit `(8, 16)`
- Value - UTF-N character enclosed in single quotes `'.'`

```rust
someChar: c8 = '\n';
...
putch someChar; // output a single character
```

### String Type

[`# Extending Strings`](#extending-strings)

- Keyword - `str`
- Value - sequence of characters enclosed in double quotes `".*"`

```rust
someString: str = "hello world";
...
puts someString; // output a string
```

### Array Type

[`# Modifying Arrays`](#modifying-arrays)

Using the operator `[]` or `[N]` you can define arrays with a fixed or unfixed size of any type. For example an array of integers.

```rust
list: i32[] = { 1, 2, 3, 4, 5 };
```

Values can be pushed and appended to the array as shown [here](#pipe-operators). Alternatively the `.push()` (push on top of array) or `.append()` (append to end of array) function can be used.

```rust
list.push(0);
// list = { 0, 1, 2, 3, 4, 5 }
//          ^
list.append(6);
// list = { 0, 1, 2, 3, 4, 5, 6 }
//                            ^
```

To pop values of the array, you can use the `.pop()` function, which removes the first element and returns its value. `.popBack()` removes the last element and returns its value.

```rust
first := list.pop(); // = 1
// list = { 2, 3, 4, 5 }
//         ^
last := list.popBack(); // = 5
// list = { 2, 3, 4 }
//                 ^
```

Specific elements at `index` can be accessed by writing

```rust
list[index];
list[0]; // first element
list[1]; // second element
...
list[-2]; // element before the last element
list[list.size() - 1]; // last element
list[-1];              // last element
```

### Type Casts

Values can be casted to specific types by writing the type in parenthesis before these.

```rust
// ( <type> ) value
puts (str) 12;
putch (c8) "\n";
```

## New Concepts

Of course, Fux also offers some quality of life features and little improvements over C++, making the overall programming experience more enjoyable.

### Pipe Operators

The pipe operators `<<` and `>>` can be used to pass around  function arguments and for example push / append values to an array. \
In any other cases, Fux will perform an assignment, rather than a special operation.

#### Passing Arguments

```rust
"someString" >> someFunction();
// same as someFunction("someString");

someFunction() << 1 << "someString";
// someFunction(1, "someString");

someFunction() << ("someString" >> someFormat(1, 3));
// someFunction(someFormat("someString", 1, 3));
```

#### Modifying Arrays

```rust
someArray: i32[];
someArray[] << 1;  // {1}
2 >> someArray[] << 3;  // {2, 1, 3}
6 >> (7 >> someArray[]) << 4 << 5; // {6, 7, 2, 1, 3, 4, 5}
//                  ^^ without brackets, Fux would 
//                     try to assign a value to someArray 
//                  someArray << {}; 
//                  => someArray = {}
```

#### Extending Strings

```rust
someString: str = "uech";
someString << 's';
// someString = "uechs";
'f' >> someString;
// someString = "fuechs";
someString << 's' << 's';
// someString = "fuechsss";

someString << "s";
//            ^ ^ fux assigns a new value because a string was used
// someString = "s"; 
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

### Existence of Objects

The existence operator `?` can be used to check wether an object exists.

```rust
someVar;
if (?someVar) { ... }
    // true
...
if (?anotherVar) { ... }
    // false - we never defined 'anotherVar'
```

Switch statements have the `not` key, which functions the same way.

```rust
switch (someVar) {
    not { ... }
}
```

---