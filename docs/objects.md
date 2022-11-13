# Objects

## Variables

[Types & Storage Modifiers](./types.md)

```rust
// <identifier> ;
name;
// <identifier> :        = <value> ;
name := "fuechs";
// <identifier> : <type> ;
name: str;
// <identifier> : <type> = <value> ;
name: str = "fuechs";

/// references
// <identifier> -> <type> ;
// <identifier> -> <type> = <object> ;
namePtr -> str;
namePtr -> str = name;

// change reference's value
// <reference> = <value> ;
namePtr = "fuechsss";

// redefine reference
// <reference> << <object> ;
namePtr << otherVariable; 
otherVariable >> namePtr;

```

## Functions

```rust
// <identifier> ( <parameters> ) { ... }
// <identifier> ( <parameters> ) : <type> { ... }
someFunction();
someFunction(): i32;
someFunction(someParameter -> str): i32; // takes reference

// working example
incrementValue(value -> str) { 
    value = (str) ++( (i32) value );
}

// overloading possible
incrementValue(value -> i32) { ++value; }
```

## Classes

```rust
// inherits from <parent>
// <parent> : <identifier> ;
// <parent> : <identifier> { ... } 
:SomeClass;
SomeParent:SomeClass { ... }
SomeParent:AnotherParent:SomeClass { ... }

// initializer
// automatically defines variables that class owns
// ::(<parameters>);
::(name: str, value: i32);
// own initializer definition
// ::(<parameters>) { ... }
::(name: str, value: i32) {
    self.name :=== name;
    safe self.val := value;
}
// overloading possible

// shorthand definition
// <parent>:<identifier>::(<parameters>);

// define initializer without class body
// <parent>:<identifier>::(<parameters>) { ... };

// define both
// <parent>:<identifier>::(<parameters>) { ... } { ... }

SomeParent:SomeClass::(name: str, value: i32) {
    self.name :=== name;
    safe self.val := value;
} {
    setVal(val: i32) {
        if (self.val != val)
            self.val = val;
    }
}

// use initizalier
// <identifier> : <class> ( <arguments> ) ;
variable: SomeClass();
variable: SomeClass("fuechs", 0);
```

### Templates

```cpp
// template < <identifier> > ;
template<T>
// template < <identifier>, <identifier>, ... > ;
template<K, V>
template<A, B, C, D>

// usage
template<T>
:List::(arr: T[] = {});

// initialize
// <identifier> : <class> < <type> > ( <arguments> ) ;
list: List<bool>();
list: List<bool>({true, false});
```

## Namespaces

```rust
// <identifier> { ... }
stringOperations {
    incrementValue(value -> str) {
        value = (str) ++( (i32) value );
    }

    final someVariable: str;
    ...
}
```