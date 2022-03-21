#### Hello World

```cpp
get core;

object main() -> core->void {
    core->cout << "Hello World!" << core->endl;
}
```
As you can see, the syntax of Fux is somewhat similar to C++, so we'll just focus on the new things.

The keyword `get` is used for importing packages which are available [here](../src/packages/).<br>
You're also able to [create your own packages](./packages.md).

Here, the package `core` is imported, which holds many vital data for Fux programming.