# Types

## Data Types

Name | Size | Default Value | Description
-|-|-|-
`void` | / | / | no value
`bool` | 1 bit | `false` | boolean (`true` or `false`)
`i8` | 1 byte | `0` | signed 8-bit integer
`u8` | 1 byte | `0` | unsigned 8-bit integer
`c8` | 1 byte | `0xFF` |  UTF-8 code unit
`i16` | 2 bytes | `0` | signed 16-bit integer
`u16` | 2 bytes | `0` | unsigned 16-bit integer
`c16` | 2 bytes | `0xFFFF` | UTF-16 code unit
`i32` | 4 bytes | `0` | signed 32-bit integer
`u32` | 4 bytes | `0` | unsigned 32-bit integer
`f32` | 4 bytes | `0.0` | 32-bit floating point integer
`i64` | 8 bytes | `0` | signed 64-bit integer
`u64` | 8 bytes | `0` | unsigned 64-bit integer
`f64` | 8 bytes | `0.0` | 64-bit floating point integer
`str` | / | `""` | sequence of characters
`var` | / | `null` | can hold any type of value
`enum` | / | `{}` | enumeration of identifiers
`struct` | / | `{}` | structure of objects

## Storage Modifiers

### Constants

The value of a constant variable can't be modified.

```rust
// <identifier> : <type> === <value> ; 
//                       ^^^
//                       constant definition 

name: str === "fuechs";
name:     === "fuechs";
```

### Safe

Can be used in classes and namespaces. \
Makes variables accessible but not modifiable.

```rust
// safe <definition>

safe name: str = "fuechs";
```

### Intern

Can be used in classes and namespaces. \
Makes variables completely private.

```rust
// intern <definition>

intern name: str = "fuechs";
```

### Final

Can be declared without value. \
Turns into constant after assignment.

```rust
// final <definition>

final name: str;
...
name = "fuechs"; // can't be modified anymore.

// alternative
name: str = "fuechs";
...
name === "fuechsss"; // late constant definition
```