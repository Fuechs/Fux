![Image missing](./fux_logo.png)
--- 
[![Image missing](https://img.shields.io/github/release/fuxlang/_?logo=airplayaudio)](https://github.com/Fuechs/fuxlang/releases/latest)
[![Image missing](https://img.shields.io/github/license/Fuechs/fuxlang?logo=markdown)](./LICENSE.md)<br>
![Image missing](https://img.shields.io/badge/Note-Compiler%20is%20still%20in%20development-blueviolet)
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
