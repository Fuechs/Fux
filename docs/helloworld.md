#### Hello World

```cpp
// Hello World program
get core;

object main() {

    core->cout << "Hello World!\n";

} -> core->void;
```

> `// Hello World program`

Everything written after a `//` or inside a comment block (`/*...*/`)<br>
will be recognized as a comment and ignored by Fux.

> `get core;`

`get` is a keyword that imports packages into the script, here,<br>
[`core`](../src/packages/) is being imported.<br>
Core is an official package that provides various important things,<br>
such as storage types.

> `object main() {} -> void;`

With the keyword `object`, a new object is defined,<br>
here of the type function, which is marked by the `()`.<br>

Before the parantheses, is the name of the object defined.<br>

Inside of the `{}`, is the code to be executed then called.<br>
After the braces, with a `->`, the return type is set to void (nothing to return).<br>

The main function will be called if you run your fux program.<br>
It needs to be named 'main' and have a return type of 'void'.<br>

> `{ core->cout << "Hello World!\n"; }`

Inside of the function is a function call.<br>

`core` is the package we imported,<br>
and from core, we want the `cout` function,<br>
to output something to the console.<br>

To do this,<br> 
we need to reference to core when calling `cout`.<br>
`core->cout` shows Fux, that we want to call the function `cout`,<br>
which is defined in the core package.

> If this is to much work, you can import like this:<br>
> `get *package;` `get *core;`<br>
> Now everything in the package is accessible<br> 
> without having to reference to the package.

With `<<` we deliver a value *to* the function,<br>
in this case `"Hello World!\n"` (`'\n'` is a newline character).<br>

With `>>` we get a value *of* the function to a variable or another object.<br>
A good example is the function `cin`.<br>
We want the user to input his name, so we can greet him:
```cpp
get *core;

object main() {
    str name;

    cout << "Please input your name: ";
    cin >> name << endl;
    cout << "Hello, " << name << "!\n"; // or add << endl;

} -> void;
```
- [Objects](../objects.md)
- [Variables](../variables.md)
- [Input and output](../io.md)
