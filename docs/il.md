# IL Code

This IL is stack backed.

## Opcodes

ID   | Name           | A0              | A1              | Description
---- | -------------- | --------------- | --------------- | ---------------------------------------------------------------------------------------------------
0x00 | No Operation   | -               | -               | Place holder
0x01 | Push u8        | u8              | -               | Push an u8 onto the stack
0x02 | Push u16       | u16             | -               | Push an u16 onto the stack
0x03 | Push u32       | u32             | -               | Push an u32 onto the stack
0x04 | Push i8        | i8              | -               | Push an i8 onto the stack
0x05 | Push i16       | i16             | -               | Push an i16 onto the stack
0x06 | Push i32       | i32             | -               | Push an i32 onto the stack
0x07 | Push f32       | f32             | -               | Push an f32 onto the stack
0x08 | Push f64       | f64             | -               | Push an f64 onto the stack
0x09 | Push str       | str             | -               | Push an string onto the stack
0x0A | Push true      | -               | -               | Push an true onto the stack
0x0B | Push false     | -               | -               | Push an false onto the stack
0x0C | Load Local     | Local Offset    | -               | Push local onto stack from the offset, note the offset is in bytes
0x0D | Store Local    | Local Offset    | -               | Pop Value from stack and store it at the local, offset is in bytes
0x0E | Load Argument  | Argument Offset | -               | Push argument onto stack from the offset, offset is in bytes
0x0F | Store Argument | Argument Offset | -               | Pop Value from stack and store it at the argument, offset in bytes
0x10 | Add            | -               | -               | Pop A; Pop B; Push A + B
0x11 | Subtract       | -               | -               | Pop A; Pop B; Push A - B
0x12 | Multiply       | -               | -               | Pop A; Pop B; Push A * B
0x13 | Divide         | -               | -               | Pop A; Pop B; Push A / B
0x14 | Negate         | -               | -               | Pop A; Push -A
0x15 | Shift Left     | -               | -               | Pop A; Pop B; Push A << B
0x16 | Shift Right    | -               | -               | Pop A; Pop B; Push A >> B
0x17 | And            | -               | -               | Pop A; Pop B; Push A & B
0x18 | Or             | -               | -               | Pop A; Pop B; Push A
0x19 | Xor            | -               | -               | Pop A; Pop B; Push A ^ B
0x1A | Copy           | -               | -               | Pop A; Push A; Push A;
0x1B | Read           | -               | -               | Pop A; Push `*A`
0x1C | Write          | -               | -               | Pop A; Pop B; `*A = B`
0x1D | Equal          | -               | -               | Pop A; Push A == 0
0x1E | Greater Than   | -               | -               | Pop A; Push A > 0
0x1F | Smaller Than   | -               | -               | Pop A; Push A < 0
0x20 | Call Function  | Label           | Argument Type[] | Pop num of arguments and creates stack frame then jumps to label; A1 is only req for external calls
0x21 | Return         | -               | -               | return, the return value should be on the stack
0x22 | Label          | Name            | Argument Type[] | behaves like a nop, but just stores meta data type[0] = return type
0x23 | goto           | label           | -               | jumps unconditionally to label
0x24 | jz             | label           | -               | jumps conditionally Pop A; if(A == 0) goto label
0x25 | Push i64       | i64             | -               | Push an i64 onto the stack
0x26 | Push u64       | u64             | -               | Push an u64 onto the stack
0x27 | del            | -               | -               | pop A;
0x28 | Push fn        | label           | -               | Push an fn pointer onto the stack
0x29 | Invoke         | Argument Type[] | -               | Pop A; fcall A;

## Binary File Format

Data Encoding:

Type   | Format
------ | ----------
true   | u8
false  | u8
string | u32 u8[]
type[] | u32 type[]

Opcode Bin:

Field | Data Type       | Optional
----- | --------------- | --------
ID    | u8              | false
A0    | opcode specific | true
A1    | opcode specific | true

## Text Assembly

This lang is not case senstive

ID   | Name
---- | ----------
0x00 | nop
0x01 | Push.u8
0x02 | Push.u16
0x03 | Push.u32
0x04 | Push.i8
0x05 | Push.i16
0x06 | Push.i32
0x07 | Push.f32
0x08 | Push.f64
0x09 | Push.str
0x0A | Push.true
0x0B | Push.false
0x0C | Load.Loc
0x0D | Store.Loc
0x0E | Load.arg
0x0F | Store.arg
0x10 | Add
0x11 | Sub
0x12 | Mul
0x13 | Div
0x14 | Neg
0x15 | shl
0x16 | shr
0x17 | Adn
0x18 | Or
0x19 | Xor
0x1A | Copy
0x1B | Read
0x1C | Write
0x1D | ifeq
0x1E | ifgt
0x1F | ifst
0x20 | fcall
0x21 | ret
0x22 | Label:
0x23 | goto
0x24 | jz
