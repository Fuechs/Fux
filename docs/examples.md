# Examples

- [Arrays](#arrays)
- [Reading File](#reading-files)
- [Formatted Print](#formatted-print)
- [Buffered IO](#buffered-io)
- [Fibonacci](#fibonacci)
- [Rule 110](#rule-110)

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

### Buffered IO

```rust
get core.io;

someFunction(msg: str): str;

main(argc: u64, argv: str[]): i64 {
    io.putln("Output: ") 
    << someFunction() << io.getln("Input: ");
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

### Rule 110

```rust
main(argc: u64, argv: str[]): i64 {
    board: u8[100];
    board[-2] = 1;

    for (i: u8; i < board.size() - 2; i++) {
        for (value -> u8 in board)
            putch value ? '*' : ' ';
        putch '\n';

        pattern: u8 = (board[0] <| 1) | board[1];
        for (j: u8; j < board.size() - 1; j++) {
            pattern = ((pattern <| 1) & 7) | board[j + 1];
            board[j] = (110 |> pattern) & 1;
        }
    }

    return 0;
}
```