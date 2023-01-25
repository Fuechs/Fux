# Examples

### Arrays

```rust
array: i64[]; // {}

main(argc: u64, argv: str[]): i64 {
    array[] << 1; // {1}
    array[] << 3; // {1, 3}
    return 0;
}
```

### Reading Files

```rust
get core.file;
using file;

main(argc: u64, argv: str[]): i64 {
    filePath := "path/to/file.txt";
    file := File(filePath, 'r');
    contents := file.read();
    return 0;
}
```

### Formatted Print

```rust
get core.io;

main(argc: u64, argv: str[]): i64 {
    res := 1;
    io.putf("Result: %i", res);
    return 0;
}
```

### Fibonacci

```rust
get core.io;

fib(num: i64): i64
    return num < 2 ? num : fib(num-1) + fib(num-2);


main(argc: u64, argv: str[]): i64 {
    io.getint() >> fib() >> io.putln();
    return 0;
}
```