# If Conditionals
If conditionals check a condition and execute code should it be true.
```
if (<condition>) {
  //code is only being executed when <condition> is true
}
```

In addition to that there is also ``else``. It should be put right after the if block and it is being executed should the condition be false.
```
if (<condition>) {

} else {
  //only being executed when <condition> is false
}
```

#### Omit Braces
You can omit braces when using ``if``/``else``. Only the next statement is included into the body.

```
if(<condition>)
  gets_executed_if_true();
gets_executed_anyway();
```
