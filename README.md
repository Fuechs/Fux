![Image missing](./fux_logo_slim_transparent.png)


![Version: 0.0.1a](https://img.shields.io/badge/Version-0.0.1a-informational)
[![Release: None](https://img.shields.io/badge/Release-None-inactive)](https://github.com/Fuechs/fuxlang/releases/latest)
![Platforms: Mac | Linux](https://img.shields.io/badge/Platforms-macOS_|_Linux-success)
[![Note: Compiler is still in development](https://img.shields.io/badge/Note-Compiler_is_still_in_development-blueviolet)](#development-status)
---
[![Made with C++](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)]()
[![Made with C](https://forthebadge.com/images/badges/made-with-c.svg)]()
[![Made with Python](https://forthebadge.com/images/badges/made-with-python.svg)](https://python.org)
---
<!--
<br>
[![Made with Typescript](https://forthebadge.com/images/badges/made-with-typescript.svg)](https://forthebadge.com)
[![Made with Java](https://forthebadge.com/images/badges/made-with-java.svg)](https://forthebadge.com)
[![Made with Ruby](https://forthebadge.com/images/badges/made-with-ruby.svg)](https://forthebadge.com)
-->

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

#### Setup

[![Setup: Currently not available](https://img.shields.io/badge/Setup-Currently_not_available-inactive)](./setup.py)
---

> In later releases an easy setup<br> 
> in form of a python file and/or Makefile<br>
> will be available for users.


#### Development Status

[![Compiler: Early Development](https://img.shields.io/badge/Compiler-Early_Development-inactive)](#compiler)
[![Syntax: Not finished](https://img.shields.io/badge/Syntax-Not_finished-yellow)](#syntax)
[![Core: Not finished](https://img.shields.io/badge/Core-Not_finished-yellow)](./src/packages/core)
---

##### Compiler

> The Compiler for Fux is currently in early development.<br>
> It will probably take a while until it works.

##### Syntax

> Fux is currently in the concept phase.<br>
> The Syntax is still in development and could change anytime,<br>
> due to software limitations for example.
