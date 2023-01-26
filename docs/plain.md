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

    someParam: safe i32;
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

    someParam: safe i32;
}
```

### Access Modifiers

`intern` and `safe` also work in namespaces.

- `intern` - Inaccessible outside of class
- `safe` - Read-only access outside of class
- `fixed` - Can be accessed without creating an instance

```rust
class SomeClass { ...

value: intern f64;
symbol: safe str;
something: fixed bool = true;

printNumber(n: i32): fixed void {
    puts (str) n;
}

...}


SomeClass.printNumber(0); // no instance
```

