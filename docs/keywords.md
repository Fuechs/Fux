# Keywords / Intrinsics

- `expr` = expression
- `stmt` = statement (including code block `{}`)

ID | Value | Parameters | Function
-|-|-|-
KEY_GET | `get` | symbol (package file) | insert code of a fux file
KEY_PACKAGE | `package` | symbol (package name) | set name of package
KEY_IF | `if` | `if (<expr>) <stmt>` | executes stmt if expr evaluates to true
KEY_ELSE | `else` | `<if> else <stmt>` | executes stmt if expr of `if` evaluates to false 
KEY_WHILE | `while` | `while (<expr>) <stmt>` | loops over stmt while expr evaluates to true
KEY_DO | `do` | `do <stmt> while (<expr>)` | execute stmt and continue loop if expr evaluates to true
KEY_FOR | `for` | 1) `for (<stmt0>; <expr0>; <expr1>) <stmt1>`<br>2) `for (<stmt0> in <expr>) <stmt1>` | 1) Executes stmt0 ('initialiser'), and loops over stmt1 ('body') and expr1 ('iterator')  while expr0 ('condition') evaluates to true.<br>2) Loops over every element of expr ('iterator') and saves value in stmt0 ('initialiser') for each run while executing stmt1 ('body') 
KEY_IN | `in` | see `KEY_FOR` | see `KEY_FOR`
KEY_IS | `is` | `<expr> is <type>` | checks wether expr is of the type `type` 
KEY_EXCEPT | `except` | `<stmt0> except <stmt1>` | runs stmt1 if code in stmt0 failed

> Soon ...