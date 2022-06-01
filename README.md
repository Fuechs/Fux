<img src="./art/logo.svg" width=300></img>

![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)
---

<!-- FUX COLORS - #fcaa68 - #ec243c -->

### Fux

###### Flawless unambiguous programming

*Safe. Fast. Simple. Whatever you want.*

#### The beautiful alternative for C++ 

- so you don't shoot yourself in the foot (like I did working on Fux)
- with better errors (well, better than just `Segmentation fault`)
- providing a large standard library

```cpp
#get <core.io>

fib(num -> int) -> int {
    return num ? < 2 : fib(num-1)+fib(num-2);
}

main() -> void {
    number -> int;
    number = (int) io.getln();
    number >> io.putln();
}
```

#### Why Fux?

...

#### Setup

To use Fux, simply execute [`./setup/setup.py`](./setup/setup.py) or type [`make setup`](./Makefile) into the terminal after pulling the repository.

#### Documentation

Resources to learn Fux can be found in [`./docs/`](./docs/).
Learning Fux is as easy as following the guide in [`./docs/guide.md`](./docs/guide.md) or using [`./docs/guide.py/`](./docs/guide.py).