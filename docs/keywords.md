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