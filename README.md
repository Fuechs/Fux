![Image missing](./art/fux_logo_slim_transparent.png)

![Version: 0.0.2a](https://img.shields.io/badge/Version-0.0.2a-informational)
[![Release: None](https://img.shields.io/badge/Release-None-inactive)](https://github.com/Fuechs/fuxlang/releases/latest)
![Platforms: macOS | Linux](https://img.shields.io/badge/Platforms-macOS_|_Linux-success)<br>
[![Documentation](https://img.shields.io/badge/Documentation-Markdown-informational)](./docs)
[![Note: Compiler is still in development](https://img.shields.io/badge/Note-Compiler_is_still_in_development-blueviolet)](#development-status)
[![License: BSD-3-Clause](https://img.shields.io/badge/License-BSD-3-Clause-informational)](./LICENSE)
---

![Fux](https://img.shields.io/badge/Fux-fcaa68)
![C++](https://img.shields.io/badge/C++-00599C?logo=c%2b%2b)
![C](https://img.shields.io/badge/C-A8B9CC?logo=c&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?logo=python&logoColor=white)
---

<!--
![Typescript](https://img.shields.io/badge/Typescript-3178C6?logo=typescript&logoColor=white)
![Java](https://img.shields.io/badge/Java-007396?logo=java)
![Ruby](https://img.shields.io/badge/Ruby-CC342D?&logo=ruby)
-->

### Fux
###### Flawless unambiguous programming
The beautiful alternative for C++ 
- so you don't shoot yourself in the foot (like I did working on Fux)
- with better errors (well, better than just `Segmentation fault.`)
- where `*something` finally has nothing to do with pointers
- and without strict syntax

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

##### Website

![Made with HTML5](https://img.shields.io/badge/HTML5-E34F26?logo=html5&logoColor=white)
![Made with CSS3](https://img.shields.io/badge/CSS3-CC6699?logo=css3&logoColor=white)
![Made with Javascript](https://img.shields.io/badge/Javascript-F7DF1E?logo=javascript&logoColor=white)
![Status Unknown](https://img.shields.io/badge/Status-Unknown-inactive)
---

> A Website for Fux is currently in work

##### Fux Language Support

![Made in JSON](https://img.shields.io/badge/JSON-000000?logo=json)
![Platforms: VSC](https://img.shields.io/badge/Visual_Studio_Code-blue?logo=visualstudiocode)
![Version: 1.0](https://img.shields.io/badge/Version-1.0-informational)
![Release: None](https://img.shields.io/badge/Release-None-inactive)
---

> A Language Support Extensions for VSC is currently in work 
