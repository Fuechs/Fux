## Objects

- [Classes](#classes)
    - [Inheritance](#inheritance)
    - [Initial function](#initial-function)
- [Functions](#functions)
    - [Return types](#return-types)
    - [Parameters](#parameters)

### Classes

> Basic class definition
```ts
object Core {}
```

#### Inheritance
```ts
object Core:SubClass {}

// Multiple classes
object Core:AnotherClass:SubClass {}
```
> Everything after `object` and before `:` will be recognized as a parent class

#### Initial function
> Parameters are defined in the inital function
```ts
object Core:Doc {

    object object:INIT() {
        skip();
        return (self);
    }

}
```
> The return type has to be `object` and the functions needs to return `self` (= the class, an object), **except** you want to return something else.<br>
> The name **has** to be `INIT`, without any exception.<br>
> Parameters for the class can be defined in the parameters for the `INIT` function:
```ts
    object: str:INIT(int arg, str msg) return(msg);

}
```

### Functions

> Basic Addition:

```ts
object int:Add(int num1, int num2) {
    return (num1+num2);
}
```

> Don't forget the brackets! These determine that the object is a function<br>

#### Return types

> There are at least as many return types as variable types:
```ts
0 // void
int // integer (int)
str // string (str)
object // object
bool // boolean (bool)
```

