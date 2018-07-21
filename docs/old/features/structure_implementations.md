# Structure Implementations
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
