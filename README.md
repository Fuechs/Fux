![Image missing](./art/fux_logo_slim_transparent.png)


![Version: 0.0.2a](https://img.shields.io/badge/Version-0.0.2a-informational)
[![Release: None](https://img.shields.io/badge/Release-None-inactive)](https://github.com/Fuechs/fuxlang/releases/latest)
![Platforms: macOS](https://img.shields.io/badge/Platforms-macOS-success)
[![Documentation](https://img.shields.io/badge/Documentation-Markdown-informational)](./docs)
[![Note: Compiler is still in development](https://img.shields.io/badge/Note-Compiler_is_still_in_development-blueviolet)](#development-status)
<br>
![Fux](https://img.shields.io/badge/Fux-fcaa68)
![C++](https://img.shields.io/badge/C++-00599C?logo=c%2b%2b)
![C](https://img.shields.io/badge/C-A8B9CC?logo=c&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?logo=python&logoColor=white)
---
<!--
<br>
![Typescript](https://img.shields.io/badge/Typescript-3178C6?logo=typescript&logoColor=white)
![Java](https://img.shields.io/badge/Java-007396?logo=java)
![Ruby](https://img.shields.io/badge/Ruby-CC342D?&logo=ruby)
-->

### Fux
###### Flawless unambiguous programming
The beautiful alternative for C++ 
- with better errors (well, better than just `Segmentation fault.`)
- where `*something` finally has nothing to do with pointers
- and without strict syntax

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

##### Fux Language Support

![Made in JSON](https://img.shields.io/badge/JSON-000000?logo=json)
![Platforms: VSC](https://img.shields.io/badge/Visual_Studio_Code-blue?logo=visualstudiocode)
![Version: 1.0](https://img.shields.io/badge/Version-1.0-informational)
![Release: None](https://img.shields.io/badge/Release-None-inactive)
---

> A Language Support Extensions for VSC is currently in work 
