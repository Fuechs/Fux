# Plain Documentation

> __Note__ \
> These are just plain notes so that I can remember the syntax.
> There will be insufficient explanations.


- [Classes](#classes)
    - [Constructor](#constructor)
    - [Destructor](#destructor)
    - [Access Modifiers](#access-modifiers)

## Classes

```rust
// class <name> : <parent> { <body> }
class SomeClass : SomeParent {}
```

### Constructor

```rust
// { ... ::(<parameters>) { <body> } ... }
class SomeClass : SomeParent {
    ::(someParam: i32) {
        self.someParam === someParam;
    }

    safe someParam: i32;
}
```

### Destructor

Can be called with `delete <address>, <arguments>;`

```rust
// { ... #(<parameters>) { <body> } ... }

class SomeClass : SomeParent {
    #() {
        delete &someParam;
    }

    safe someParam: i32;
}
```

### Access Modifiers

`intern` and `safe` also work on namespaces.

- `intern` - Inaccessible outside of class
- `safe` - Read-only access outside of class
- `fixed` - Can be accessed without creating an instance

```rust
class SomeClass { ...

intern value: f64;
safe symbol: str;
fixed something: bool = true;

fixed printNumber(n: i32) {
    puts (str) n;
}

...}


SomeClass.printNumber(0); // no instance
```

