# The Dusk Programming Language

## [Post-Bootstrap](../README.md) -> [Syntax](README.md) -> Conditionals

### If Conditionals

If conditionals check a condition and execute code should it be true.

```
if (<condition>) {
  //code is only being executed when <condition> is true
}
```

In addition to that there is also ``else``. It should be put right after the if
block and it is being executed should the condition be false.

```
if (<condition>) {

} else {
  //only being executed when <condition> is false
}
```

#### Omit Braces

You can omit braces when using ``if``/``else``. Only the next statement is
included into the body.

```
if(<condition>)
  gets_executed_if_true();
gets_executed_anyway();
```

#### If-statement examples

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

### For-loop examples

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
