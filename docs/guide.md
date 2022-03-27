### Full Guide for Fux
---
> This guide expects some C++ knowledge

#### Contents

1. [Getting started](#getting-started)
2. [Input and Output](#input-and-output)
    - [The Core Package](#the-core-package)
    - [Console Out](#console-out)
    - [Console In](#console-in)
3. [Objects](#objects)
    - [Definition](#definition)
    - [Types](#types)
    - [Variables](#variables)
    - [Functions](#functions)
        - [Parameters](#parameters)
        - [Return Types](#return-types)
    - [Classes](#classes)
        - [Inheritance](#inheritance)

---

#### Getting started

To get ready for learning Fux, you need to download the latest release and execute the setup for your OS

###### With Makefile

```
make setup
```

###### With Python

```
python3.10 setup/setup.py
```

By now, everything should be ready for programming in Fux.

---

> Since we already went through the basic Hello World program [here](./helloworld.md), we'll move on to the next topic.

#### Input and Output

##### The Core Package

```cpp
core -> package {}
```

##### Console Out

```cpp
cout(str messages[]) -> void {}
```

##### Console In

```cpp
cin() -> str {}
```

---

#### Objects

##### Definition

```cpp
name;
```

##### Types

```cpp
name -> type;
```

##### Variables

```cpp
name -> str = "Fuechs";
```

##### Functions

```cpp
main() -> void {}
```

##### Classes

```cpp
Animal:Fox {}
```

