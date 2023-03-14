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
[KEY_FOR](#for) | `for` | Control Flow, Loop
[KEY_IN](#in) | `in` | Iterator, Loop, Condition
[KEY_IS](#is) | `is` | Condition
[KEY_EXCEPT](#except) | `except` | Exception Handling
[KEY_SWITCH](#switch) | `switch` | Condition
[KEY_CASE](#switch) | `case` | Condition
[KEY_DEFAULT](#switch) | `default` | Condition
[KEY_NOT](#switch) | `not` | Condition
[KEY_BREAK](#break) | `break` | Control Flow
[KEY_RETURN](#return) | `return` | Return (Control Flow)
[KEY_TRHOW](#throw) | `throw` | Exception Handling, Control Flow
[KEY_EXIT](#exit) | `exit` | Intrinsic, Exception Handling, Control Flow
[KEY_PUTS](#puts) | `puts` | Intrinsic, I/O
[KEY_PUTCH](#putch) | `putch` | Intrinsic, I/O
[KEY_READ](#read) | `read` | Intrinsic, I/O
[KEY_DELETE](#delete) | `delete` | Intrinsic, Memory 
[KEY_RESERVE](#reserve) | `reserve` | Intrinsic, Memory
[KEY_FREE](#free) | `free` | Intrinsic, Memory
[KEY_SAFE](#safe) | `safe` | Access Modifier
[KEY_INTERN](#intern) | `intern` | Access Modifier
[KEY_FINAL](#final) | `final` | Access Modifier
[KEY_FIXED](#fixed) | `fixed` | Access Modifier
[KEY_ASYNC](#async) | `async` | Access Modifier, Networking

### Get

- Usage: `get <package>[.<member>];`
- Effect: Insert code into program

```dart
get core.io;
```

### Package

- Usage: `package <package-name>`
- Effect: Sets name of package or declares file as part of a package

```go
package core;
```

### If Else

- Usage:
    1. `if (<expr>) <stmt>`
    2. `<if> else <stmt>`
- Effect: 
    1. Executes `stmt` if `expr` evaluates to true
    2. Executes `stmt` if `expr` of `if` evaluates to false

```cpp
if (x > 10) {
    puts "Large";
    return 0;
} else
    puts "Small";
```

### Do While

- Usage:
    1. `while (<expr>) <stmt>`
    2. `do <stmt> while (<expr>)` 
- Effect:
    1. Loops over `stmt` while `expr` evaluates to true
    2. Executes `stmt` and continues looping over it while `expr` evaluates to true

```cpp
do 
    read &input;
while (!input.empty());
```

### For

- `initialiser` statement
- `condition` expression
- `iterator` expression
- Usage: `for (<initialiser>; <condition>; <iterator>) <stmt>`
- Effect:
    1) Executes `initialiser` 
    2) Loops over `stmt` and `iterator` while `condition` evaluates to true

```rust
for (i: u64 = 0; i < array.size(); i++)
    array[i] = 0;
```

### In

- `initialiser` statement
- `iterator` expression
- Usage:
    1. `for (<initialiser> in <iterator>) <stmt>`
    2. `<expr> in <expr1>`
- Effect: 
    1.  For every element of `iterator`
        1) Stores element in variable declared in `initialiser`
        2) Executes `stmt`
    2. Checks wether `expr1` contains `expr`

```rust
if (10 in array)
    puts "Array contains 10.";

for (i -> u64 in array) // '->' is a reference
    if (i == 10) {
        puts "Array contains 10.";
        break;
    }
```

### Is

- Usage: `<expr> is <type>`
- Effect: Checks wether the value of `expr` is of type `type`

```dart
if (x is u64)
    ...
else if (x is str)
    ...
```

### Except

- Usage `<stmt> except <stmt1>`
- Effect: Exceutes `stmt1` if execution of `stmt` failed

```cpp
x <> y; 
except 
    throw SomeErrorType;
```

### Switch 

- `object` expression
- `value` expression
- Usage:

```cpp
switch (<object>) {
    case (<value>)  <stmt>; break; // falls through without break
    case (<value>)  <stmt>; break; 
    in (<expr>)     <stmt>; break; // checks wether object is an element of expr
    is (<type>)     <stmt>; break; // object is of type `type`
    not             <stmt>; break; // object does not exist (operator '?')
    default         <stmt>; break; // default case, executes if no case executes
}
```

- Effect: Checks value of `object` and runs case depending on that

### Break

- Usage: `break`
- Effect:
    1. Stops any loop
    2. Stops fall through in switch

```cpp
break;
```

### Return

- Usage: `return <expr>`
- Effect: Return a value and exit from a function

```cpp
return 0;
```

### Throw

- Usage: `throw <expr>`
- Effect: Throws exception `expr` and exits program

```cpp
throw SomeErrorType;
```

### Exit

- Usage: `exit <expr>`
- Effect: Exits program with code `expr`

```c
exit 1;
```

### Puts

- Usage: `puts <expr>`
- Effect: Outputs string literal (`*c8`) `expr` to stdout

```c
puts "Hello World!\n";
```

### Putch

- Usage: `putch <expr>`
- Effect: Outputs character (`c8`) `expr` to stdout

```c
putch '\n';
```

### Read

- Usage: `read <expr>`
- Effect: Reads string literal (`*c8`) from stdin and stores it at address (`**c8`) `expr`

```rust
input: *c8;
read &input;
```

### Delete

- Usage: `delete <expr>`
- Effect: Deletes object at address `expr`

```cpp
delete &input;
``` 

### Reserve

- `size` expression
- `pointer` expression
- Usage: `reserve <type>, <size>, <pointer>`
- Effect: Reserves / Allocates `size` amount of memory of type `type` and saves address in `pointer`

```rust
ptr: *i64; 
reserve i64, 10, ptr; 
```

### Free

- Usage: `free <expr>`
- Effect: Frees reserved / allocated memory at address `expr`

```c
free ptr;
```

### Safe

- Usage: `safe <type>`
- Effect: Object of this type can't be modified but read outside of the class / namespace it's declared in.

```rust
name: safe *c8;
```

### Intern

- Usage: `intern <type>`
- Effect: Object of this type can't be accessed outside of the class / namespace it's declared in.

```rust
name: intern *c8;
```

### Final

- Usage: `final <type>`
- Effect: Value of object of this type can't be modified after assignment.

```rust
value: final *i8;
value = 1;
```

### Fixed

- Usage: `fixed <type>`
- Effect: Object of this type can be accessed without creating an instance of the class it's declared in.

```rust
name: fixed *c8 = "fux";
```

### Async

- `function` statement
- `call` expression
- Usage:
    1) `<function> async <type>`
    2) `async <call>`
- Effect:
    1) Declares `function` as asynchronus
    2) Waits until asynchronus `call` 

```rust
someFunction(): async i64; // async function
async someFunction(); // async call
someFunction(); // normal call
```