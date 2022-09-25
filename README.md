<img src="./art/logo.svg" width=300></img>

![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)
---

<!-- FUX COLORS - #fcaa68 - #ec243c -->

### Fux

###### Flawless unambiguous programming

 *Fast. Safe. Simple. Whatever you want.* 

#### The beautiful alternative for C++ 
- so you don't shoot yourself in the foot (like I did working on Fux)
- with better errors (well, better than just `Segmentation fault`)
- providing a large standard library


```cpp
#get <core.io>

fib(num: int): int {
    return num < 2 ? num : fib(num-1) + fib(num-2);
}

main(): void {
    number: int;
    number = io.getln("Fibonacci of: ");
    fib(number) >> io.putln();
}
```

#### Why Fux?

You wan't it simple? 

```cpp
name = "Fuechs";
```

Object-Oriented?

```cpp
#get <core.io>

:Main {

    main(argc -> int, argv -> str[]) -> void {
        io.putf("%i/5 arguments", argc);
    }

}
```

Low-Level? (x86 and Arm)

```cpp
#get <core.asm>

main() -> void {
    "
    org 0x100
    mov dx, msg
    mov ah, 9
    int 0x21
    mov ah, 0x4c
    int 0x21
    msg db 'Goodbye World!', 0x0d, 0x0a, '$
    ", 0 >> assembly();
}
```

Whatever this is?

```cpp
typedef struct Position {
    x -> double;
    y -> double;
    z -> double;
} Position;

typedef struct Cube {
    title -> char[10];
    width -> double;
    height -> double;
    depth -> double;
    position -> Position;
} Cube;

cube -> Cube;
cube.title = "tutorial";
cube.width = 2;
cube.height = 2;
cube.depth  2;
cube.position.x = 0;
cube.position.y = 2.5;
cube.position.z = 0;
```

Well, you can use Fux however you want (Even though I do not recommend the Assembly one).


#### Setup

To use Fux, simply execute [`./setup/setup.py`](./setup/setup.py) or type [`make setup`](./Makefile) into the terminal after pulling the repository.


#### Documentation

Resources to learn Fux can be found in [`./docs/`](./docs/).
Learning Fux is as easy as following the guide in [`./docs/guide.md`](./docs/guide.md) or using [`./docs/guide.py/`](./docs/guide.py).

---

##### Disclaimers

![forthebadge](https://forthebadge.com/images/badges/contains-technical-debt.svg)
![forthebadge](https://forthebadge.com/images/badges/contains-tasty-spaghetti-code.svg)

Oh, and don't read the code.
You don't want to. Believe me. Please. I beg you.

---

