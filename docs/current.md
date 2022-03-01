## Fux Documentation

- [Hello World](#hello-world)
- [IO](#io)
	- [Output](#output)
	- [Input](#input)
- [Maths](#maths)
- [Variables](#variables)
	- [Definition](#definition)
		- [Types](#types)
			- [Integer](#integer)
			- [String](#string)
			- [Boolean](#boolean)
			- [Object](#object)
- [Builtin Functions](#builtin-functions)
- [Objects](#objects)
	- [Name and type](#name-and-type)
	- [Classes](#classes)
		- [Inheritance](#inheritance)
	- [Functions](#functions)
		- [Arguments](#arguments)
		- [Return types](#return-types)
		- [Function calls](#function-calls)
- [Comments](#comments)


- - -

### Hello World

```

object [void]Main() {

	.Name < "MAIN_HW";
	.Type < FUNCTION;
	
	Console.Print < "Hello World!";
}

``` 

- - -

### IO

#### Output

```
Console.Print < "Message";
Console.Print < "Hello, @" < name;
```

#### Input

```
str userInput < (Console.Input < "Input: ");
```

- - -

### Maths


```

```

- - -
### Variables

#### Definition

```
[type] name < [definition];
``` 

##### Types

> There are many types for the declaration of variables
>
> The standard type `var` can be used when the type is unknown before the definition

```
var empty;
```

##### Integer

```
int num < 18;
int anotherNum < 9.7;
int thirdNum < -145352535.325265;
```

##### String

```
str message < "Hello";
str name < "Fuechs";
str text < "Hello, @ \n" < name;
```

##### Boolean

> Short: `bool`

```
bool False < !true;
bool _False < false;
bool True < true;
bool empty < void;
```

> Don‘t:  `bool true < true;`

##### Object

> An object variable does not hold any value
> 
> You could call it a condition or a type to describe something
>
> And also, IT NEEDS TO BE IN CAPS

```
object DEBUG;

… {
	return < DEBUG;
}

if (randomFunction == DEBUG) {}
```

##### Lists

```
int randomNum[];

randomNum += 9;
// Now: randomNum = [9]
randomNum < [randomNum, 6];
// Now: randomNum = [[9], 6]

```

- - -

### Builtin Functions

> Soon

- - -

### Objects

> Yes, objects can be more than just a variable that describes a condition
> 
> It was honestly just an extra I thought of on the fly

#### Name and Type

> `.Name` **is not necessary**, just for your own use
> 
> Can be printed by using ClassName.Name

```
.Name < [string];
.Type < [type];
```

> `.Type` tells Fux which type of object you‘re writing, it **is necessary**

#### Classes

```
object MyClass {

	.Name < "MY_CLASS";
	.Type < CLASS; // The `CLASS` type is also a object variable

	object Init {

		.Type < INIT; // Init functions have to be the type `INIT`

	}

	// Functions …
}
```

##### Inheritance

```
object MyClass < OtherClass {}
```

#### Functions

```
object [int]MyFunction {

	.Name < "MY_FUNCTION";
	.Type < FUNCTION;
	.Args < [int]a;

	return < a*2; 

}
```

##### Arguments

```
.Args < [type]name < [type]name < …;
```

> The variables defined `.Args` are the parameters of your function, which should be provided with the function call

##### Return types

> Return types are in the `[]` defined and set what the function returns.
>
> Here‘s a list:
> - `int`
> - `str`
> - `bool`
> - `object`
> - `void` 
```
object [object]Function {

	.Type < FUNCTION;

	object < SUCCES;
	return < SUCCES;

}
```

##### Function calls

```
int x < 15;
int y < (MyFunction < x);
```

- - -

### Comments

> Just do:

```
// Single line comment
/* 
Multi line 
comment
*/
``` 

- - -
