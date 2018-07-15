# Full example

This is a complete example of all language features of Dusk.

```
extern {
  fn format(x: str);
  fn printf(x: str);
  fn sqrt(x: f64);
}

struct vec3 {
  x: f32
  y: f32
  z: f32
}

impl vec32 {
  fn length() : f32 {
    f32(sqrt(self.x*self.x + self.y*self.y + self.z*self.z));
  }
  
  fn toString() : str {
    format("Vec3 {X: %d, Y: %d, Z: %d}", self.x, self.y, self.z);
  }
}

infix fn +(a: vec32, b: vec32) {
  vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

fn main() {
  let a = vec3(1, 0, 0);
  let b = vec3(0, 1, 0);
  let c = a + b;
  printf("Length of vector a (%s) is %d", a.toString(), a.length());
  printf("Length of vector b (%s) is %d", b.toString(), b.length());
  printf("Length of vector a+b (%s) is %d", c.toString(), c.length());
  
  if (a.length() + b.length() > c.length()) {
    printf("The whole is less than the sum of it's parts!");
  } else if (a.length() + b.length() == c.length()) {
    printf("The whole is the same as the sum of it's parts!");
  } else {
    printf("The whole is greater than the sum of it's parts!");
  }
}
```
