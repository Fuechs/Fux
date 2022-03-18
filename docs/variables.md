#### Variables

##### Definition

```cpp
get core;

core->int number = 1;
core->str name = "Fuechs";
core->bool debug = true;
core->var list[]; // empty list
core->var something; // empty variable
```
As you can see, there are following storage types for variables:
- (int) Integer (Numbers of any kind: 0, 1.3253252235, -135)
- (str) String
- (bool) Boolean (`true`, `false` or `core->void`)
- (var) Unknown type (if you don't know yet, or for lists with multiple types)

Where are also other ways to give a variable its value

```cpp
int num;
function_with_return >> num;
// Assigns returned int of the function to the variable

int list_of_num[] = [0, 1];
function_with_return >> list_of_num[];
// Appends a third value to the list

str name = "Füchs";
"Fuechs" >> name;
/* 
    Defines name new, same as
    name = "Fuechs";
*/