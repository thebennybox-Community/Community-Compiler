# Syntax Specification

## Structure

Curly brace code blocks introduce a new scope and may or may not
have some form of header. Statements may appear in any scope
including the root level scope (you are not forced to use a `main`
function).

## Data Types

| Name                   | Description    | Syntax |
| ---------------------- | -------------- | ------ |
| Unsigned byte          | Use            | u8     |
| Unsigned short         | The            | u16    |
| Unsigned integer       | Internet       | u32    |
| Unsigned long          | Use            | u64    |
| Signed byte            | The            | i8     |
| Signed short           | Internet       | i16    |
| Signed integer         | Use            | i32    |
| Signed long            | The            | i64    |
| Boolean                | Internet       | bool   |
| String                 | Use            | str    |
| Single precision float | The            | f32    |
| Double precision float | Internet       | f64    |

## Statements

### Comments

Single line comments start with `//`.

Multiline comments start with `/*` and end with `*/`.

### Code Blocks

```
{
    // This is a scope and a code block
}
```

### Literals

```
10
10u8
15i32
"string"
true
false
10.00125f64
```

### Declarations

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

### Expressions

```
a = "bob";
a = b + c * 20;
a = (a * 4) + 5;
```

### Functions

```
fn foo(a : i32) : i32 {
    // Function body
}
dec a = fn;
dec c = (a) ~> {
    // Anonymous function body
};
```

### Loops

```
loop (5) {
    // Do this 5 times
}

loop (x to y) {
    // For-loop
}

loop (x = 0; x < z; x++) {
    // For-loop
}

loop (true == true) {
    // While loop
}

loop (item in list) {
    // Foreach loop
}

continue;
break;
```

### Logic

#### If Statements

```
if (x > y) {

}

if (x == a <-> b) {
    // (x == a) || (x == b)
}

if (true) {

}
else {

}

if (...) {

}
else if (...) {

}
else {

}
```

#### Return Statements

The result of evaluating the last expression in a function will be returned, or
you can return explicitly using the `return` keyword.

### Match Statements

TODO

## Paradigm

```
struct vec3 {
   x: f32
   y: f32
   z: f32
}

impl vec3 {
   get x {
      // Calculate x
      result
   }

   fn distanceTo(self: vec3, other: vec3) {
      (other - self).length()
   }

   fn length(self: vec3) {
      // ...
   }
}
```
