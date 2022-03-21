![Image missing](./art/fux_logo_slim_transparent.png)

![Version: 0.0.2a](https://img.shields.io/badge/Version-0.0.2a-informational?style=flat-square)
[![Release: None](https://img.shields.io/badge/Release-None-inactive?style=flat-square)](https://github.com/Fuechs/fuxlang/releases/latest)
![Platforms: macOS | Linux](https://img.shields.io/badge/Platforms-macOS_|_Linux-success?style=flat-square)<br>
[![Documentation](https://img.shields.io/badge/Documentation-Markdown-informational?style=flat-square)](./docs)
[![License: BSD-3-Clause](https://img.shields.io/badge/License-BSD--3--Clause-informational?style=flat-square)](./LICENSE)<br>
[![Note: Compiler is still in development](https://img.shields.io/badge/Note-Compiler_is_still_in_development-blueviolet?style=flat-square)](#development-status)
---
![Fux](https://img.shields.io/badge/Fux-fcaa68?style=for-the-badge&logo=data:image/png;base64,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)
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
