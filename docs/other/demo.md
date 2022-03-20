#### Demo script from [README](../../README.md)

```cpp
get *core; // Imports all of core (* = everything)
          // so you won't have to reference to core

get random; // Imports random


/*
Defines a struct with following elements:
integer x equals 0
integer y equals 0
integer z equals 2
character c equals 'a'
*/
struct {

    int x = 0;
    int y = 0;
    int z = 2;
    char c = 'a'; 
    
} version; // defines version with the struct as its value

bool run; // create boolean run without value
int profile = 0; // defines integer profile euqals 0

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

> What's `<<` & `>>`?
> Explained [here](../objects.md#function-calls) and [here](../variables.md#definition).
