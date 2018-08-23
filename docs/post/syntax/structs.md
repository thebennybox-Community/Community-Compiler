# The Dusk Programming Language

## [Post-Bootstrap](../README.md) -> [Syntax](README.md) -> Structures

Structures, or `struct`s for short, can be used to group data.

```
struct <name> {
  { (var / let) <name>: <type>; }
}
```

### Examples

```
struct Foo {
  var bar: str;
}

struct BetterFoo {
  var bar: str;
  var switch: bool;
}
```

### Creation

To create structs you take the struct name and put paratheses containing the
field values after it.

#### Examples

```
Foo("Hello!")

BetterFoo("Hello World!", true)

let foo = Foo("bar");
```

### Data Access

To access the data you use a dot. E.g. ``<variable name>.<field name>``.

#### Examples

```
let foo = Foo("this is the bar field");

let string: str = foo.bar;
```

### Structure Implementations

To give structs methods you have to write an implementation. The methods inside the implementation block are like normal function definitions except that they have an implicit parameter ``self`` that is the struct.

```
impl <struct name> {
  fn method() {

  }
}

```

#### Examples

```
struct Foo {
  var counter: i32;
}

impl Foo {
  fn increase() {
    self.counter++;
  }

  fn increase(amount: i32) {
    self.counter += amount;
  }
}
```
