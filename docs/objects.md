#### Objects


##### Functions

Functions are recognized by the well known parantheses for functions

```cpp
get *core;

// name func
object func() -> void; 
// empty function with return type void; 
// nothing to return
```

There are the same return types as types for variables, plus `void`, for nothing.<br>
Now we want out function to either add or subtract two numbers.

```cpp
object calc(int num1, int num2, str op) {} -> int;
```

Here, we added two parameters `num1` and `num2` for the inputed numbers, set the return type to `int` and added a third parameter `op`, so the user can choose an operation to perfom.<br>
Now we just use if statements and mathematical operations to build the function.

```cpp
object calc(int num1, int num2, str op) {

    if (op == "add") return num1+num2; 
    // checks if op equals "add"
    // if then return num1+num2
    else if(op == "subtract") return num1-num2;
    // checks if op equals "subtract"
    // if then return num1-num2
    else return 0; 
    // return 0 then if unknown input is given

} -> int;
```

Finished! Now let's move to the next part to see how we can call the function.

###### Function Calls

Here's how you're supposed to call the function from the last part

```cpp
// function

int number;
number = calc << 1 << 2 << "subtract";
//              num1 num2      op
cout << number;
// Output: -1

number = calc << 1 << 2 << "qwertz";
// Number = 0;
```

Just write the name of the function and deliver arguments by connecting them with `<<` to the function.


##### Classes
