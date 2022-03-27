#### Hello World

```cpp
get core;

main() -> void {
    core->cout << "Hello World!" << core->endl;
}
```
As you can see, the syntax of Fux is somewhat similar to C++, so we'll just focus on the new things.

The keyword `get` is used for importing packages which are available [here](../src/packages/).<br>
You're also able to [create your own packages](./packages.md).

Here, the package `core` is imported, which holds classes and functions which are required for Fux programming.<br>
To avoid having to reference to the packages import them like here shown:

```cpp
get * core;
```

Next an object with the name `main` is defined, and declared as a function with the parantheses `()`.<br>
The `->` points to the return type of the function, `void` = nothing to return.

Inside the code block, is a simple output. `core->cout` is called with the argument `"Hello World"` and `core->endl` = `'\n'`.<br>
It generates following output:

```
Hello World

```
---
That's it!<br>
If you're interested in Fux, you can read the Documenation and learn how it works.