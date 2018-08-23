# The Dusk Programming Language

## [Post-Bootstrap](../README.md) -> [Syntax](README.md) -> Functions

Functions are defined like this:

```
fn <name>([<par_name>: <par_type>[, <par2_name>: <par2_type>]*])[: <return_type>] {

}

```

If the return type isn't specified then it is inferred. Multiple functions with
the same name can exist as long as they don't have the same parameter type count
and order.

The result of evaluating the last expression in a function will be returned, or
you can return explicitly using the `return` keyword.

### Examples

```
fn foo(arg1: i32, arg2: i32, arg3: i32) {
    // Function body
}

fn foo(arg1: i32, arg2: i32) {
    // Function body
}

fn bar(): str {
    // Function body
}

fn bar(): i32 { // Error: bar() is already defined
    // Function body
}

dec a = fn;
dec c = (a) ~> {
    // Anonymous function body
};
```

### Default Parameter Values

Parameters can be given default values to make calling them easier. To specify a
default value you put a ``=`` and a constant expression after the parameter type
similar to a declaration.

```
fn foo(bar: i32 = -1) {

}

can be called like:
foo();           // bar is -1
foo(bar = 123);  // bar is 123
```

## Short Function Return

If the function only returns and expression it can be shortened by replacing the
body with ``-> <expr>;``

```
fn foo() -> bar();

fn square(num: i32) -> num * num;
```

# External Declarations

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

### Match Statements

TODO: Explain match statements
