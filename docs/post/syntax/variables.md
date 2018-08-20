# The Dusk Programming Language

## [Post-Bootstrap](../README.md) -> [Syntax](README.md) -> Variables

Declarations declare the type of a variable and can optionally initialize them. Declarations are either immutable or mutable.

The general syntax for declarations is as following:

```
dec <name>: <type>
dec <name>: <type> = <expression>
dec <name> = <expression>
```
The first declaration doesn't initialize the variable which has to be done before it's being used.

In the second declaration the variable is also initialized. A compiler error is thrown should the variable type and the expression return value type not be the same or have an implicit conversion.

In the third declaration the type of the variable is implicitly set to the return type of the expression.

Note that ``dec`` is not an actual keyword for declarations and ``let`` or ``var`` should be used instead.

### Immutable And Mutable

For immutable declarations the ``dec`` is replaced by a ``let`` and for mutable it is replaced by ``var``. Immutable variables have to be initialized when declared. ``let a: i32`` is therefore invalid.

### Examples

```
let pi = 3.14159265359
var count: i32;
var width: f32 = 1.5;
```

### Misc notes

```
dec a : i32;
dec a : i32 = 10;
dec a = 10i32;
dec a = "string";
dec a = true;

// When no explicit type is provided for a number, i32 will be assumed
dec a = 10;

// When no explicit type is provided for a floating point number, f32 will be
// assumed
dec a = 10.0128;
```
