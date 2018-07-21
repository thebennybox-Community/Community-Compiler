# Lists

Lists are basically arrays with support for expansion. You can even instantiate them with an array:

```dusk
var x: list<str> = ["Thing one", "Thing two"];
```

Member functions:

`add(T item)`: Add single item to a list.

`addAll(T[] item) | addAll(list<T> item)`: Add all items in array to a list.

`remove(index)`: Remove item at specified index and move all items after the index one back in the list.
