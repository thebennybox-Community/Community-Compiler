# The Dusk Programming Language

## [Post-Bootstrap](../README.md) -> [Syntax](README.md) -> Misc

## Scope

Curly brace code blocks introduce a new scope and may or may not
have some form of header. Statements may appear in any scope
including the root level scope (you are not forced to use a `main`
function).

## Statements

### Code Blocks

```
{
    // This is a scope and a code block
}
```

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
