#### Demo script from [README](../../README.md)

```cpp
get *core; // Imports all of core (* = everything)

/*
Defines a new object of the function type
called 'calc'
with one paramter: 'num'
*/
object calc(<< int num) {  
// '<<' sets the paramter as something 
// that will be delivered to the function

    if (num < 1) num++; 
    // Increments num by one if num is smaller than 1
    else if (num == 10) { // if num equals to 10
        num++++; // increments num by 2
        num = num*num-num;
        // multiplies num by num, subtracts num,
        // and applies new value to num
    }
    return num; // returns the new value of num

} -> int; // sets return type to integer

/* 
Defines main function of the program
with no paramters
*/
object main() {

    int user_num; // Defines new empty variable of the type integer
    cout << "Please input a number: "; // Output into console
    (int)cin >> user_num; 
    /* 
    scans console for input, 
    converts the input to integer
    and applies the value to user_num 
    */
    except { 
        error << "Not a number\n"; // Puts out an error
        exit << 1; // Exits the program with the error id 1
    }
    // The except statement will be executed 
    // if the last statement could'nt execute
    cout << (calc << user_num) << "\n";
    /* 
    calls the 'calc' function with the argument 'user_num',
    returned value will be printed to console 
    with a newline character
    */

} -> void; //sets return type to void; nothing to return


/*
'<<' if something is delivered for the function
'>>' if the function delivers for something
*/
```