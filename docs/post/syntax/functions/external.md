# The Dusk Programming Language

## [Post-Bootstrap](../../README.md) -> [Syntax](../README.md) -> [Functions](README.md) -> External Declarations

External Declarations are used to call unmanaged or native functions from e.g.
``c`` or ``c++``.

### External Modifier
A function can be marked with ``extern`` to tell the compiler that the function
is external. In this case the function body is replaced by a semicolon.

```
extern fn printf(out: str);
```

### External Block

``extern`` can also be used to create block in which external functions can be
defined. Only function heads can be defined in this body and the ``extern``
modifier applies to every function.

```
extern {
  fn printf(out: str);
  fn sqrt(num: f64): f64;
}
```
