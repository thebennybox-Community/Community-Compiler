# Functions
Functions are defined like this:
```
fn <name>([<par_name>: <par_type>[, <par2_name>: <par2_type>]*])[: <return_type>] {

}
```
If the return type isn't specified then it is inferred. Multiple methods with the same name can exist as long as they don't have the same parameter type count and order.

## Examples
```
fn foo(arg1: i32, arg2: i32, arg3: i32) {

}

fn foo(arg1: i32, arg2: i32) {

}

fn bar(): str {

}

fn bar(): i32 { //Error: bar() is already defined

}
```

## Default Parameter Values
Parameters can be given default values to make calling them easier. To specify a default value you put a ``=`` and a constant expression after the parameter type similar to a declaration.
```
fn foo(bar: i32 = -1) {

}

can be called like:
foo();     // bar is -1
foo(123);  // bar is 123
```

## Short Function Return
If the function only returns and expression it can be shortened by replacing the body with ``-> <expr>;``
```
fn foo() -> bar();

fn square(num: i32) -> num * num;
```
