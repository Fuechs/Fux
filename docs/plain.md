# Plain Documentation

> __Note__ \
> These are just plain notes so that I can remember the syntax.
> There will be insufficient explanations.


- [Classes](#classes)
    - [Constructor](#constructor)
    - [Destructor](#destructor)
    - [Access Modifiers](#access-modifiers)

## Classes

> __Warning__ \
> Outdated.

```rust
// <parent>:<name> { <body> }
SomeParent:SomeClass {}
```

### Constructor

```rust
// <parent>:<name>::(<parameters>) 
// { <constructor_body> } { <class_body> }
SomeParent:SomeClass::(someParam: i32) {
    self.someParam === someParam;
} {
    safe someParam: i32;
}

// { ... ::(<parameters>) { <body> } ... }
SomeParent:SomeClass {
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

SomeParent:SomeClass {
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
:SomeClass { ...

intern value: f64;
safe symbol: str;
fixed something: bool = true;

fixed printNumber(n: i32) {
    puts (str) n;
}

...}


SomeClass.printNumber(0); // no instance
```

