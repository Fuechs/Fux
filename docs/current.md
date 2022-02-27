## Fux Documentation

- [Hello World](#hello-world)
- [Builtin Functions](#builtin-functions-and-their-function-calls)

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

> Builtin functions are simple and usually aren't used in fux, these are just for maintening purposes
> Examples:
> - #print message // Outputs an message to the console 
> - #repl string, char1, char2, times // Replaces certain Characters in a string

```
#print "message";
```

---
