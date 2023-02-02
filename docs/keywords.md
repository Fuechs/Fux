# Keywords / Intrinsics

- `expr` expression
- `stmt` statement (including code blocks)

###### Table of Contents
ID | Value | Type
-|-|-
[KEY_GET](#get) | `get` | Packages
[KEY_PACKAGE](#package) | `package` | Packages
[KEY_IF](#if-else) | `if` | Control Flow
[KEY_ELSE](#if-else) | `else` | Control Flow
[KEY_WHILE](#do-while) | `while` | Control Flow, Loop
[KEY_DO](#do-while) | `do` | Control Flow, Loop
[KEY_FOR](#for-in) | `for` | Control Flow, Loop
[KEY_IN](#for-in) | `in` | Iterator, Loop
[KEY_IS](#is) | `is` | Condition
[KEY_EXCEPT](#except) | `except` | Exception Handling


### Get

- Usage: `get <package>[.<member>];`
- Effect: Insert code into program

### Package

- Usage: `package <package-name>`
- Effect: Sets name of package or declares file as part of a package

### If Else

- Usage: `if (<expr>) <stmt>`
- Effect: Executes `stmt` if `expr` evaluates to true
- Usage: `<if> else <stmt>`
- Effect: Executes `stmt` if `expr` of `if` evaluates to false

### Do While
### For In
### Is
### Except
> Soon ...