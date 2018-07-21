# Lists

Lists are basically arrays with support for expansion. You can even instantiate them with an array:

```dusk
var x: list<str> = ["Thing one", "Thing two"];
```

They can be appended to easily with `list.add(item)`, and removal is just as easy: `list.remove(index)`.
