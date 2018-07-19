# Literals
There are four types of literals: boolean, number, character and string literals.

## Booleans
A boolean literal is either ``true`` or ``false``.

## Numbers

#### Simple
A number is a sequence of digits and underscores. An underscore is only allowed inbetween numbers.

valid:  
```
123
1
0
123_456_789
0_0
```
invalid:
```
_123
123_
0___0
```

#### Decimal
Decimals can be expressed by putting a dot and another number behind it.
```
123.456
0.0001
0.000_000_000_1
```

#### Typed
The type and size of a number can be specified at the end of the number. If not specified it's either i32 or f32 depending on if the number is a decimal.

Valid types are:
```
i8, i16, i32, i64
u8, u16, u32, u64
f32, f64
```
The number specifies the size in bits and the letter the type: ``i`` for signed integers, ``u`` for unsigned integers and ``f`` for floating point numbers.
Examples:
```
123i16
345.45f64
255u8
0.000_000_1f32
```

## Strings
Strings start and end with quotation marks ("). Special characters can also be used with "escaping".

Examples:
```
"very cool string"
"they can even be empty"
""
```

### Escape Sequences
There are special escape sequences that are replaced with a special characters.

| Sequence     | Description                     |  
| ------------ | ------------------------------- |  
| \a           | Alert (Beep)                    |  
| \b           | Backspace                       |
| \f           | Formfeed                        |
| \n           | Newline                         |
| \r           | Carriage Return                 |
| \t           | Horizontal Tab                  |
| \v           | Vertical Tab                    |
| \\\\         | Backslash                       |
| \\\"         | Double quotation mark           |
| \\\'         | Single quotation mark           |
| \u*hhhh*     | Unicode below 10000 hexadecimal |
| \U*hhhhhhhh* | Unicode character               |

```
"hello\nworld!"
```

## Characters
Characters are similar to strings. They use single quotation marks and support escape sequences. However characters can only represent one character. Characters are of the unsigned integer type and take on the smallest size they can represent.

Valid:
```
'a' -> u8
'b'
'\''
'\u1234' -> u16
```

Invalid:
```
'abc'
```
