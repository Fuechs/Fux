![Image missing](./art/fux_logo_slim_transparent.png)

![Version: 0.0.2a](https://img.shields.io/badge/Version-0.0.2a-informational?style=flat-square)
[![Release: None](https://img.shields.io/badge/Release-None-inactive?style=flat-square)](https://github.com/Fuechs/fuxlang/releases/latest)
![Platforms: macOS | Linux](https://img.shields.io/badge/Platforms-macOS_|_Linux-success?style=flat-square)<br>
[![Documentation](https://img.shields.io/badge/Documentation-Markdown-informational?style=flat-square)](./docs)
[![License: BSD-3-Clause](https://img.shields.io/badge/License-BSD--3--Clause-informational?style=flat-square)](./LICENSE)<br>
[![Note: Compiler is still in development](https://img.shields.io/badge/Note-Compiler_is_still_in_development-blueviolet?style=flat-square)](#development-status)
---
![Fux](https://img.shields.io/badge/Fux-fcaa68?style=for-the-badge&logo=github)
![C++](https://img.shields.io/badge/C++-00599C?logo=c%2b%2b&style=for-the-badge)
![Python](https://img.shields.io/badge/Python-3776AB?logo=python&logoColor=white&style=for-the-badge)
![Typescript](https://img.shields.io/badge/Typescript-3178C6?logo=typescript&logoColor=white&style=for-the-badge)
![Java](https://img.shields.io/badge/Java-007396?logo=java&style=for-the-badge)
![Ruby](https://img.shields.io/badge/Ruby-CC342D?&logo=ruby&style=for-the-badge)
---

<!-- FUX COLOR - #fcaa68 -->
### Fux
###### Flawless unambiguous programming
The beautiful alternative for C++ 
- so you don't shoot yourself in the foot (like I did working on Fux)
- with better errors (well, better than just `Segmentation fault`)
- where `*` has nothing to do with pointers
- and without strict syntax

> [Utilizable Packages](./src/packages)
```cpp
get * core;
get random;

struct {

    int x = 0;
    int y = 0;
    int z = 2;
    char c = 'a'; 
    
} version;  

bool run;
int profile = 0;

object choose() -> bool {

    bool return_value;
    (random->choice << true << false) >> return_value;

    return return_value;

}

object main() -> void {

    clear;

    choose >> run;

    if (!run) cout << "Not running" << endl;

    while (run) {

        switch (profile) {
            
            case (-1) profile = profile^2;
            case (0) profile++++;
            case (1) profile--; 
            case (2) profile++;
            default run = false;

        }

        cout << profile << endl;

    }

    cout << 'v';
    cout << version->x << '.' << version->y << '.' << version->z << version->c;
    endl;
    
}   
```
> Confused? This script is explained [here](./docs/other/demo.md).<br>
> Or read and execute the [script in python](./docs/other/demo.py).

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
---
