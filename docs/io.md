## Input and Output

> Note: Every program has to be in a main function, explained [here](main.md)
>
> Output something to the console
```ts
Console.Output([string]);
```

> Use variables
> 
> More to variables: [Variables](variables.md)
```ts
str name = "Fuechs"; 

Console.Output("Hello, \v\n", name);
// or
Console.Output("Hello, ");
Console.Output(name); 
// optional: Console.Newline();
// Same as printing a '\n' character

/* 
'\v' will be replaced with the variable 
in the following argument
Multiple '\v' will be replaced in the same order 
as the arguments are given
NOTE: Same goes for the Console.Input() function
*/
```

---

> Ask for an input from an user
```ts
Console.Input([string]);
```
> Assign input to a variable
```ts
str cmd = Console.Input("Number: ");
Console.NewLine();
Console.Output(cmd);
```
