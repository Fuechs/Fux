## Fux Documentation

- [Hello World](#hello-world)
- [Builtin Functions](#builtin-functions-and-their-function-calls)
- [Declaring an Object](#declaring-an-object)
  - [Classes](#classes)
  - [Functions](#functions)

---

### Hello World

```
get > core > end

object main {

  .name > MAIN_HELLOWORLD > end // Not necessary, just for clarification when using 'Console.Output > main > end'
  .type > FUNCTION > end
  
  Console.Output > "Hello World!\n" > end 
  // or #print "Hello World!\n";
  author > "Fuechs" > end
  Console.Output > "Fux by @\n" > author > end
  // not possible with #print because of the use of variables

}

```

---

### Builtin functions and their function calls

> Builtin functions are simple and are usually not used in fux, these are just for maintening purposes or for the core/base packages<br>
> Examples:
> - `#print message` outputs an message to the console 
> - `#repl string, char1, char2, times` replaces certain Characters in a string
> - `#sys_c file_dir, \*args;` compiles and executes a c file with following arguments  

```
#print "message";

#repl "Hell? W?rld", "?", "o", 2;  // Replaces "?" with "o" x2 (0 = As often as possible)

#sys_c "src/main.c", "compile", "src/test/main.fux";
```

---

### Declaring an object

> Instead of `class`  `func` as keywords, there are only objects, which can be either classes or functions. <br>
> You still need to declare what an object represents:<br>
```
{.type > CLASS > end}
{.type > FUNCTION > end}
```

#### Classes

```
object Class {

  .name > CLASS_DOCS > end
  .type > CLASS > end

}
```

> You can define more objects (classes and functions) in a Class, for example:<br>

```
  object SubClass {
  
    .name > SUB_CLASS_DOCS > end
    .type > CLASS > end
  
  }
  
}
```

> Classes can also inherit from other classes:<br>

```
object Class > AnotherClass {}
```

#### Functions

> Functions get another keyword, besides `.name` and `.type`: `.args`<br>
> These are defined as shown here:<br>

```
{.args > string > end}
{.args > string > vars[] > end}
```

> Basic Function: Addition

```
object Addition {

  .name > ADD_DOCS > end
  .type > FUNCTIONS > end
  .args > a > b > end
  
  return > a + b

}

sum > Addition > 15 > 3 > end
Console.Output > sum > end // core package required for Console.Output function ('get > core > end')
```

---
