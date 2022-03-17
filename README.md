![Image missing](./fux_logo_slim_transparent.png)


![Version: 0.0.1a](https://img.shields.io/badge/Version-0.0.1a-informational)
[![Release: None](https://img.shields.io/badge/Release-None-inactive?logo=airplayaudio)](https://github.com/Fuechs/fuxlang/releases/latest)
![Platforms: Mac](https://img.shields.io/badge/Platforms-macOS_|_Linux-success)
[![Note: Compiler is still in development](https://img.shields.io/badge/Note-Compiler%20is%20still%20in%20development-blueviolet)](#development-status)
---
### Fux
###### Flawless unambiguous programming
The beautiful alternative for C++ 
- without any debugging horror
- and where `*something` finally has nothing to do with pointers

```cpp
get *core;

object calc(int num) {

    if (num < 1) num++;
    else if (num == 10) {
        num++++;
        num = num*num-num;
    }
    return num;

} -> int;

object main() {

    int user_num;
    cout << "Please input a number: ";
    (int)cin >> user_num;
    except {
        error << "Not a number\n";
        exit << 1;
    }
    cout << (calc << user_num) << "\n"; 

} -> void;
```
> Confused? This script is explained [here](./docs/other/demo.md).

#### Made with
![C++](https://img.shields.io/badge/-C++-ff69b4?logo=c%2B%2B)
![C](https://img.shields.io/badge/-C-important?logo=c)
![Pythn](https://img.shields.ip/badge/-Pythom-blue?logo=python)

#### Setup

[![Setup: Currently not available](https://img.shields.io/badge/Setup-Currently_not_available-inactive?logo=python)](.)
---

> In later releases an easy setup<br> 
> in form of a python file and/or Makefile<br>
> will be available for users.


#### Development Status

[![Compiler: Early Development](https://img.shields.io/badge/Compiler-Early_Development-inactive?logo=c%2B%2B)](#compiler)
[![Syntax: Not finished](https://img.shields.io/badge/Syntax-Not_finished-yellow)](#syntax)
[![Core: Not finished](https://img.shields.io/badge/Core-Not_finished-inactive)](./src/packages/core)
---

##### Compiler

> The Compiler for Fux is currently in early development.<br>
> It will probably take a while until it works.

##### Syntax

> Fux is currently in the concept phase.<br>
> The Syntax is still in development and could change anytime,<br>
> due to software limitations for example.
