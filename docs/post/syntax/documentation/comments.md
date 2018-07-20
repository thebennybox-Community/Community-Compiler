# The Dusk Programming Language

## [Post-Bootstrap](../../README.md) -> [Syntax](../README.md) -> [Documentation](README.md) -> Comments

Comments are used to explain code and are ignored by the compiler. Should a
comment be inside a string literal it is parsed as a string instead of a
comment.

### Single-Line Comments

Single-line comments start with ``//`` and only end after a new line (``\n``).

```
some_code(); //This is a comment
"//this isn't a comment"
```

### Multi-Line Comments

Multi-line comments start with ``/*`` and end with ``*/``.

```
/*
multi
line
comment
*/

"/* not a comment */"


/*
/*

comments can't be interleaved

*/
*/ -> syntax error, "*/" is an unknown expression
```
