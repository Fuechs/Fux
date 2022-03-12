## Packages

> Using packages is easy
> <br>Just import them
```ts
get core;

get (core, maths);
```
> Or use own packages
```ts
get "./packs/base.fux" as raw_base;
// Multiple aliases
get core as (Core, CORE);

package Base = raw_base.Base();
```