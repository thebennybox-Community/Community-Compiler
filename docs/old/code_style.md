# Code Style

## General

#### Semicolon
The semicolon after a statement/expression isn't separated by a space. Following statements/expression are separated by a space.
```
function();

a = 1; b = 3; c = 2;
```

#### Bodies
The the starting bracket of a body is always placed on the line the body header is on separated from it by a space. The ending bracket is on a new line.

```
if (true) {

}

struct {
    //stuff
}
```

#### Indentation
Code should be indented by four spaces

```
impl Test {
    fn function() {
        do_something();
    }
}
```


## Literals

#### Numbers
Numbers should written like normal numbers. They can also contain underscores to help readability.  
```
12
65
1_000_000
1.25
0.000_000_1
```

#### Strings
You can't do anything wrong here.
```
"Hey there!"
```

#### Booleans
You'll create a compiler error if you do it wrong.
```
true
false
```

## Declarations
The equal sign has spaces on both sides, the colon specifying the type however only has one afterwards.
```
dec a;
dec a = 0;
dec a: str = "cool stuff";
```

The same goes for function return types and parameter types.
```
fn test(parameter: str): i32 {

}
```

## Statements

#### If
There are spaces between ``if`` and the brackets and between the brackets and the body.

```
if (true) {

}
```

The else is placed in the same line the if body ends and should also be separated by a space on both sides

```
if (false) {

} else {

}
```

An ``else if`` construct is done like this:

```
if (false) {

} else if (true) {

}
```

#### Loops
Loops have a similar style to if statements

```
loop (true) {

}
```

#### Match
Same for ``match``

```
match (a) {

}
```

## Naming
Functions and variables should be written in snake\_case
```
fn do_stuff() { ... }

dec really_cool_value;
```

Structs should be named with PascalCase

```
struct NiceStruct { ... }
```

## Operators
Every operator should be surrounded by spaces
```
3 * 4
6 / 5
```

The only exception are unary operators
```
i++

!boolean
```

## Arrays
Arrays initializer have no spaces after/before their braces.

```
[9]
[3, 5, 1]
```
The comma is followed by a space
