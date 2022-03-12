## Unlock

> With `&UNLOCK`, you're able to unlock parts of fux for a file, which allows for deep fux programming<br>
> Why is this necessary? 
> <br>You can (e.g.) customize fux by unlocking VITAL
```ts
&UNLOCK@VITAL // Unlocks vital data for this file
```
> First most of these statements are in caps.<br>
> You can unlock certain parts in the depths of fux,<br>
> by giving an location (e.g. `VITAL`) after an `@`, which represents 'at'.
---
> Now, to use this vital data, you need to know what's in there<br>
> You could create a new variable type<br>
> (if you're creative enough)
```ts
&UNLOCK@VITAL
&VITAL@CPPFUX
```
> The vital part is adressed with `&VITAL`, most of the times, and whats behind, again, with an `@`<br>
> In this secnario, we implemented CPPFUX into the file,<br>
> so that following classes (or packages) can inherit from it

```ts
&UNLOCK@VITAL
&VITAL@CPPFUX

get core; /* Don't forget: 
you still need core if you don't want to define everything new.
But if you want, you need to implement objects: &VITAL@OBJECT */

object CPPFUX:custom { // I'll call this type 'custom'

    instruction (custom [&VAR_NAME][ = [&VAR_VALUE]]) -> &VAR@VITAL@INT // This variable type will store integers
    /*
    Instructions can be complicated on the first look, 
    but they aren't.
    You need to define, how you're new type needs to be defined:
    '[]' stores values or optional instructions
    after that, you add an '->' and call a function for example, 
    or a vital function to create a variable
    &VAR_NAME and &VAR_VALUE will be stored in memory for 
    variable creation later
    */

    // Now you are free to create custom functions for the 'custom' variable type
    object 0:Info() {
        Console.Output(
            "Variable stored at \v\n\v: \v\n",
            &VAR_LOCATION, &VAR_NAME, &VAR_VALUE
        )
    }

}
```
> Here's how you'll use it
```ts
// class defined here...

custom Profile = 0;
Profile.Info();
/* Output:
Variable stored at ...
Profile: 0
*/
```

> With `&LOCK`, you can lock parts again for the rest of the file

```ts
&UNLOCK@VITAL

// ...

&LOCK@ALL // or &LOCK@VITAL

// ...