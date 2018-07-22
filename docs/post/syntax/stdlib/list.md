# The Dusk Programming Language

## [Post-Bootstrap](../../README.md) -> [Syntax](../README.md) -> [Standard Library](README.md) -> Lists

Lists are basically arrays with support for expansion. You can even instantiate them with an array:

```dusk
var x: List<str> = ["Thing one", "Thing two"];
```

Member functions:

`add(item: T)`: Add single item to a list.

`addAll(item: T[]) | addAll(item: list<T>)`: Add all items in array to a list.

`remove(index)`: Remove item at specified index and move all items after the index one back in the list.

`size()`: Get current size of list.
