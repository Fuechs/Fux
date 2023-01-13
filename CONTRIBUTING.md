# Contribution Guidelines

> __Note__ \
> These guidelines are rather incomplete and may change at any time.
> 
> Last updated on 2023-01-12

- [Introduction](#introduction)
    - [Ways to contribute](#ways-to-contribute)
- [Rules](#rules)
    - [Code of Conduct](#code-of-conduct)
    - [Security Vulnerabilities](#security-vulnerabilities)
    - [Conventions](#conventions)
- [Getting Started](#getting-started)

## Introduction

Thanks for showing interest in contributing to this project! \
This document includes information about what we are looking for, and what we are expecting from contributors. \
If any questions remain, please create an issue.

### Ways to contribute

#### Reporting an Issue

If you found

- bugs,
- problems,
- missing features,

please create an issue describing the matter. \
Do not leave out any important information that seems self-evident to you. \
**For vulnerability issues, see [here](#security-vulnerabilities).**

#### Resolving an Issue

You can help resolving issues by implementing fixes ([with these steps](#contributing-code)) or answering questions. 

#### Contributing Code

- Follow the [Conventions](#conventions).
- For large changes, create an issue or use an existing one (to get feedback).
- For small changes[^1], creating an issue is not required, but leave a comment if one already exists.
- Drastic changes[^2] should have a good reason.
- Verify that the modified code is working as expected.
- Continue as described [here](#getting-started).

[^1]: A few lines of code, spelling errors, additional information in markup / text / comments.

[^2]: Replacing e.g. a whole class, file or library. 

## Rules

- Ensure compatibility of the code with MacOS & Linux.
- For any changes regarding code, please create an issue (or use an existing issue) and describe your changes (to get feedback).
- Follow the conventions for this project.

### Code of Conduct

With contributing to this project you automatically agree to follow our [Code of Conduct](./CODE_OF_CONDUCT.md).

### Security Vulnerabilities

Read our [Security Policy](./SECURITY.md).

### Conventions

> Please note that your pull request will *not* be rejected if these conventions are *not strictly* followed.

You can get familiar with the file structure in the [`src/`](./src) folder.

- [Naming](#naming)
- [Space](#space)
- [Types](#types)
- [Using Namespaces](#using-namespaces)
- [Using Licensed Code](#using-licensed-code)
- [LLVM IR](#llvm-ir)
- [Commit Messages](#commit-messages)

C++ files should start with the following (doxygen generated) comment:

```cpp
/**
 * @file filename.suffix
 * @author your (user-)name
 * @brief brief description
 * @version X.X
 * @date yyyy-mm-dd 
 * 
 * @copyright Copyright (c) 2020-yyyy, Fux programming language and authors. All rights reserved.
 * 
 */
```

#### Naming

- Class, function and variable names are in CamelCase. For classes, the first letter is capitalized.
- Boolean variables can have a preceding `is`.

``` cpp
void doSomething();
bool isRunning;
size_t id;
class SomeClass;
```

#### Space

- One tab should be equivalent to 4 spaces.
- The code should not include any unnecessary indentation or spaces. See the code below.
- If possible, you can leave out code blocks.

```cpp
// In functions or loops!
// Not this:
if (isRunning) {
    // do stuff
}

// This:
if (!isRunning)
    return; // or break, continue, ...

// do stuff

// leave out code blocks 
for (...)
    for (...)
        return;
```

- If functions just have on short statement, it is ok to define everything in one line.

```cpp
// please note that this is just an example
int add(int x, int y) { return x + y; }

int main(int argc, char **argv) {
    int result = add(argc, 2);
    return result;
}
```

- Do not introduce any unrequired classes.
- When creating a class, the header file should only include the declaration with comments describing individual elements (There are some exceptions to this, e.g. small classes like FuxType).
- If no body is required, constructors can be defined in the header file (using `: param(param), ... {}`) .
- The definitions should be in the same order as the declarations in the header file.
- The layout is as follows:

```
class ... 
    public ...
        Constructors
        Destructor
        Functions
        Variables
    private ...
        Variables
        Functions
```

```cpp
// someclass.hpp
class SomeClass {
public:
    SomeClass();
    ~SomeClass();

    string getName();

    string name;
};

// someclass.cpp

SomeClass::SomeClass() { ... }

SomeClass::~SomeClass() { ... }

string SomeClass::getName() { ... }
```

#### Types

- For any unsigned sizes the `size_t` type should be used.

```cpp
size_t id;
```

- For often used vector types, you'll usually find type definitions with `typedef`. The name of the type can be abbreviated and should end with `List` (`Groups` for a vector of vectors).
- The same goes for dynamic pointers, which end with `Ptr`.

```cpp
typedef vector<Type> TypeList;
typedef vector<SourceFile *> FileList;
typedef vector<FileList> FileGroups;
...
FileList requiredFiles = ...;
```

- The asterisk denoting pointers should be seperated from the type. (e.g. `Type *`).
- If possible, References are preferred.

```cpp
SourceFile *mainFile;

void modifyValue(int &value) { ... }

int myValue = 0;
modifyValue(myValue);
```

- (Especially regarding AST classes) Use references in the constructor for dynamic pointers and call `std::move` in the constructor.

```cpp
SomeConstructor(ExprPtr &LHS, ExprPtr &RHS)
: LHS(std::move(LHS)), RHS(std::move(RHS)) {}
```

#### Using Namespaces

All includes from the standard library are in `src/fux.hpp`. In this file is also `using namespace std;`. If you need to use `std::move` or similiar functions though, please use it with the prefix to make the call qualified.

#### Using Licensed Code

If you add any licensed code (for example a utility header), include the license at the top of the file as a comment.

#### LLVM IR

All names are in snake_case.

Predefined or internal functions start with `Fux_`.
Functions that are part of a module, class or type have the name as an additional prefix  (`...CLASS_create_default`).

#### Commit Messages

Commit messages do not have to follow any conventions.

## Getting Started

> **Working on your first Pull Request?** You can learn how from this *free* series [How to Contribute to an Open Source Project on GitHub](https://kcd.im/pull-request)

After completing all necessary steps (and getting positive feedback), you can start with forking the repository. \
After making all changes to your fork, create a pull request to the development (or experimental) branch (right now, this is the `main` branch). This does not apply to patches for released versions. You have to give us your written agreement that your code will be licensed under the [Apache License v2.0 with LLVM Exceptions](./LICENSE). 
Your pull request will be reviewed and, if accepted, merged.