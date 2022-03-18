![Image missing](./art/fux_logo_slim_transparent.png)


![Version: 0.0.2a](https://img.shields.io/badge/Version-0.0.2a-informational)
[![Release: None](https://img.shields.io/badge/Release-None-inactive)](https://github.com/Fuechs/fuxlang/releases/latest)
![Platforms: macOS](https://img.shields.io/badge/Platforms-macOS-success)
[![Documentation](https://img.shields.io/badge/Documentation-Markdown-informational)](./docs)
[![Note: Compiler is still in development](https://img.shields.io/badge/Note-Compiler_is_still_in_development-blueviolet)](#development-status)
---

<!--![Made with](https://img.shields.io/badge/Made-with-green?style=for-the-badge)<br>-->
![Fux](https://img.shields.io/badge/Fux-fcaa68)
![C++](https://img.shields.io/badge/C++-00599C?logo=c%2b%2b)
![C](https://img.shields.io/badge/C-A8B9CC?logo=c&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?logo=python&logoColor=white)
---

<!--
<br>
![Typescript](https://img.shields.io/badge/Typescript-3178C6?style=for-the-badge&logo=typescript&logoColor=white)
![Java](https://img.shields.io/badge/Java-007396?style=for-the-badge&logo=java)
![Ruby](https://img.shields.io/badge/Ruby-CC342D?style=for-the-badge&logo=ruby)
-->

[![forthebadge](data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIxMjMuNjgiIGhlaWdodD0iMzUiIHZpZXdCb3g9IjAgMCAxMjMuNjggMzUiPjxyZWN0IGNsYXNzPSJzdmdfX3JlY3QiIHg9IjAiIHk9IjAiIHdpZHRoPSI1NC44OSIgaGVpZ2h0PSIzNSIgZmlsbD0iI0ZDQUE2OCIvPjxyZWN0IGNsYXNzPSJzdmdfX3JlY3QiIHg9IjUyLjg5IiB5PSIwIiB3aWR0aD0iNzAuNzkiIGhlaWdodD0iMzUiIGZpbGw9IiNFRjIzM0MiLz48cGF0aCBjbGFzcz0ic3ZnX190ZXh0IiBkPSJNMTUuNzAgMjJMMTQuMjIgMjJMMTQuMjIgMTMuNDdMMTkuNjQgMTMuNDdMMTkuNjQgMTQuNjZMMTUuNzAgMTQuNjZMMTUuNzAgMTcuMjBMMTkuMTMgMTcuMjBMMTkuMTMgMTguMzhMMTUuNzAgMTguMzhMMTUuNzAgMjJaTTIzLjY2IDE5LjE2TDIzLjY2IDE5LjE2TDIzLjY2IDEzLjQ3TDI1LjE0IDEzLjQ3TDI1LjE0IDE5LjE4UTI1LjE0IDIwLjAzIDI1LjU3IDIwLjQ4UTI2LjAxIDIwLjkzIDI2Ljg1IDIwLjkzTDI2Ljg1IDIwLjkzUTI4LjU2IDIwLjkzIDI4LjU2IDE5LjEzTDI4LjU2IDE5LjEzTDI4LjU2IDEzLjQ3TDMwLjA0IDEzLjQ3TDMwLjA0IDE5LjE3UTMwLjA0IDIwLjUzIDI5LjE3IDIxLjMyUTI4LjMwIDIyLjEyIDI2Ljg1IDIyLjEyTDI2Ljg1IDIyLjEyUTI1LjM5IDIyLjEyIDI0LjUyIDIxLjMzUTIzLjY2IDIwLjU1IDIzLjY2IDE5LjE2Wk0zNS43MCAyMkwzMy45OCAyMkwzNi42MiAxNy43MEwzNC4wNCAxMy40N0wzNS43NSAxMy40N0wzNy41NCAxNi41NUwzOS4zNCAxMy40N0w0MS4wNSAxMy40N0wzOC40NyAxNy43MEw0MS4xMSAyMkwzOS4zOCAyMkwzNy41NCAxOC44N0wzNS43MCAyMloiIGZpbGw9IiMwMDAwMDAiLz48cGF0aCBjbGFzcz0ic3ZnX190ZXh0IiBkPSJNNzMuNDYgMjJMNjcuMDggMjJMNjcuMDggMTMuNjBMNjkuNDYgMTMuNjBMNjkuNDYgMjAuMTFMNzMuNDYgMjAuMTFMNzMuNDYgMjJaTTc5LjMwIDIyTDc2Ljg3IDIyTDgwLjU4IDEzLjYwTDgyLjkzIDEzLjYwTDg2LjY0IDIyTDg0LjE4IDIyTDgzLjUxIDIwLjM3TDc5Ljk2IDIwLjM3TDc5LjMwIDIyWk04MS43NCAxNS45M0w4MC42NSAxOC42MUw4Mi44MiAxOC42MUw4MS43NCAxNS45M1pNOTMuMTMgMjJMOTAuODAgMjJMOTAuODAgMTMuNjBMOTIuNzUgMTMuNjBMOTYuNDYgMTguMDdMOTYuNDYgMTMuNjBMOTguNzkgMTMuNjBMOTguNzkgMjJMOTYuODQgMjJMOTMuMTMgMTcuNTJMOTMuMTMgMjJaTTEwMy41MiAxNy44MEwxMDMuNTIgMTcuODBRMTAzLjUyIDE2LjU0IDEwNC4xMiAxNS41NFExMDQuNzIgMTQuNTUgMTA1Ljc5IDEzLjk5UTEwNi44NiAxMy40MyAxMDguMjAgMTMuNDNMMTA4LjIwIDEzLjQzUTEwOS4zOCAxMy40MyAxMTAuMzEgMTMuODNRMTExLjI1IDE0LjIyIDExMS44NyAxNC45N0wxMTEuODcgMTQuOTdMMTEwLjM2IDE2LjMzUTEwOS41MiAxNS40MCAxMDguMzQgMTUuNDBMMTA4LjM0IDE1LjQwUTEwOC4zMyAxNS40MCAxMDguMzIgMTUuNDBMMTA4LjMyIDE1LjQwUTEwNy4yNCAxNS40MCAxMDYuNTggMTYuMDZRMTA1LjkyIDE2LjcxIDEwNS45MiAxNy44MEwxMDUuOTIgMTcuODBRMTA1LjkyIDE4LjUwIDEwNi4yMiAxOS4wNFExMDYuNTIgMTkuNTkgMTA3LjA2IDE5Ljg5UTEwNy42MCAyMC4yMCAxMDguMzAgMjAuMjBMMTA4LjMwIDIwLjIwUTEwOC45OCAyMC4yMCAxMDkuNTggMTkuOTNMMTA5LjU4IDE5LjkzTDEwOS41OCAxNy42MkwxMTEuNjggMTcuNjJMMTExLjY4IDIxLjEwUTExMC45NiAyMS42MSAxMTAuMDIgMjEuODlRMTA5LjA5IDIyLjE3IDEwOC4xNSAyMi4xN0wxMDguMTUgMjIuMTdRMTA2LjgzIDIyLjE3IDEwNS43OCAyMS42MVExMDQuNzIgMjEuMDUgMTA0LjEyIDIwLjA1UTEwMy41MiAxOS4wNiAxMDMuNTIgMTcuODBaIiBmaWxsPSIjRkZGRkZGIiB4PSI2NS44OSIvPjwvc3ZnPg==)](https://forthebadge.com)
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
