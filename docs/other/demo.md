#### Demo script from [README](../../README.md)

```cpp
get *core; // Imports all of core (* = everything)

/*
Defines a new object of the function type
called 'calc'
with one paramter: 'num'
*/
object calc(int num) { 

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

}
```

