![Image missing](./fux_logo_slim_transparent.png)


![Version: 0.0.1a](https://img.shields.io/badge/Version-0.0.1a-informational)
[![Release: None](https://img.shields.io/badge/Release-None-inactive)](https://github.com/Fuechs/fuxlang/releases/latest)
![Platforms: Mac | Linux](https://img.shields.io/badge/Platforms-macOS_|_Linux-success)
[![Note: Compiler is still in development](https://img.shields.io/badge/Note-Compiler_is_still_in_development-blueviolet)](#development-status)
---
![Made with](https://img.shields.io/badge/Made-with-success?style=for-the-badge)<br>
![Fux](https://img.shields.io/badge/Fux-orange?style=for-the-badge&logo=https://github.com/Fuechs/fuxlang/fux_icon.png)
![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2b%2b)
![C](https://img.shields.io/badge/C-blue?style=for-the-badge&logo=c)
![Python](https://img.shields.io/badge/Python-lightgrey?style=for-the-badge&logo=python)
![Love](https://img.shields.io/badge/<3-blueviolet?style=for-the-badge)
---
<!--
<br>
![Made with Typescript](https://img.shields.io/badge/Typescript-lightgrey?style=for-the-badge&logo=typescript)
![Made with Java](https://img.shields.io/badge/Java-yellow?style=for-the-badge&logo=java)
![Made with Ruby](https://img.shields.io/badge/Ruby-red?style=for-the-badge&logo=ruby)
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
