#### Demo script from [README](../../README.md)

```cpp
get * core; // Imports all of core (* = everything)
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

bool run; // creates boolean run without value
int profile = 0; // defines integer profile euqals 0

/*
Defines an object of the function type 
with return type boolean
without parameters 
*/

object choose() -> bool {

    bool return_value; // creates boolean return_value without value
    (random->choice << true << false) >> return_value; 
    /*
    executes the 'choice' function of the 'random' package 
    with following arguments:
    true, false
    and applies the returned value to 'return_value'
    */

    return return_value; // returns return_value

}


// Defines the main function
object main() -> void { // sets return type to void; nothing to return

    clear; // executes (core->)clear without arguments

    choose >> run; // executes choose and applies returned value to run
    // same as 'run = choose;'

    if (!run) cout << "Not running" << endl;
    // if not run / run is false
    // print out 'Not running' and end the line
    // endl = '\n'

    while (run) { // run the code block while run is true

        switch (profile) { 
        /*
        Switch statement

        Takes one value and goes through all cases
        and compares the value, if they're the same
        the code after will be executed.

        Default case if nothing matches is defined
        with 'default'
        */
            
            case (-1) profile = profile^2;
            // if profile equals -1
            // profile equals profile*profile
            case (0) profile++++; 
            // increment profile by 2
            case (1) profile--; 
            // decrement profile
            case (2) profile++;
            // increment profile
            default run = false;
            // 'else' run equals false

        }

        cout << profile << endl;
        // Prints out profile and ends the line

    }

    cout << 'v'; // prints out 'v'
    cout << version->x << '.' << version->y << '.' << version->z << version->c; 
    // prints out 'x.y.zc', where x, y, z and c are defined in the struct 'version'
    endl; // ends the line 
    
}
```
