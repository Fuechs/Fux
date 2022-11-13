# Keywords

## Output

### puts

Outputs a string directly to the console.

```cpp
// puts <string> ;
puts "Hello World!\n";
```

### putch

Outputs a character directly to the console.

```rust
// putch <character> ;
putch '\n';
```

## Control

### Condition

[Condition Operators](./operators.md#condition)

#### if / else

```cpp
// if (<condition>) { ... }
// else if (<condition>) { ... }
// else { ... }
// and && 
// or ||
// existence ?
// not !

if (variable == 1) 
    --variable;
else if (variable == 2) {
    someArray[--variable] = someValue;
} else if (?variable)
    ++variable;
else 
    exit 1;
```

#### switch

```cpp
/* 
switch (<value>) {
    case (<value>) { ... }
    not { ... } (triggers if variable does not exist)
    default { ... }
}

cases can fall through
default & not can't
*/

switch (variable) {
    case (1) { 
        --variable; 
        break; 
    }
    case (2) { 
        someArray[--variable] = someValue; 
        break; 
    }
    not
        exit 1; 
    default
        ++variable; 
}
```

### Loops

#### for

```rust
// for ( <statement> ; <condition> ; <statement> ) { ... }
//      intialization   condition     updation

for (i: u8; i < 10; i++)
    someArray[i] = i;

```

#### for ... in

```rust
// for ( <object> in <object> ) { ... }

for (c: c8 in "Hello World!")
    putch c;
```

#### while

```rust
// for ( <condition> ) { ... }

while (i --> 0)
    putch (c8) i;
```

#### do ... while

```rust
// do { ... } while ( <condition> );

do {
    putch (c8) i--;
} while (i > 0);
```

#### break

Break out of a loop.

```rust
while (true) {
    ...
    break;
}
```

#### continue

Jump to next cycle of a loop.

```rust
while (true) {
    ...
    continue;
}
```

### return

Return a value in a function.

```cpp
// return <value> ;
return 0;
```

### exit

Exit the program with a code.

```c
// exit <value> ;
exit 1;
```

### label ... goto

Assign a location to a label and jump to it.

```arm
// label <identifier>
// goto <identifier>

label loop
... 
goto loop 
```

### except

Will catch exceptions in the preceding statement or code block.

```cpp
// <statement> except { ... }
// { ... } except { ... }

someFunction(1, '\t');
except (someException)
    io.put(someException.content());
```

## Boolean Values

### true

true, 1

```cpp
some: bool = true;
```

### false

false, 0

```cpp
some: bool = false;
```

### null

null, nothing (can be hold by any type)

```java
some: var = null;
```

## typedef

[Type Keywords](./types.md#types)

Define a type.

```cpp
// typedef <object/type> <identifier> ;
typedef u64 size;
typedef struct { ... } someType;
...
amount: size;
someThing: someType;
```

## Packages

## package

Define a package. Links all files together.

```java
// package <identifier> ;
package someApi;
```

## get

Import objects of a package. 

```kotlin
get someApi;
...
someApi.someFunction(someApi.someVar);
```

## using

Use objects of a package without prefix.

```cpp
get someApi;

// using <object/package> ;
using someApi;

someFunction(someVar); // defined in someApi
```