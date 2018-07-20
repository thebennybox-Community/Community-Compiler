# Structures
Structures, or structs for short, can be used to group data.
```
struct <name> {
  { (var / let) <name>: <type>; }
}
```

#### Examples
```
struct Foo {
  var bar: str;
}

struct BetterFoo {
  var bar: str;
  var switch: bool;
}
```

## Creation
To create structs you take the struct name and put paratheses containing the field values after it.

#### Examples
```
Foo("Hello!")

BetterFoo("Hello World!", true)

let foo = Foo("bar");
```

## Data Access
To access the data you use a dot. E.g. ``<variable name>.<field name>``.

#### Examples
```
let foo = Foo("this is the bar field");

let string: str = foo.bar;
```
