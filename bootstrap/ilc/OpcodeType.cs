﻿namespace ilc
{
    public enum OpcodeType
    {
        nop = 0x0,
        PushU8,
        PushU16,
        PushU32,
        PushI8,
        PushI16,
        PushI32,
        PushF32,
        PushF64,
        PushStr,
        PushTrue,
        PushFalse,
        LoadLoc,
        StoreLoc,
        LoadArg,
        StoreArg,
        Add,
        Sub,
        Mul,
        Div,
        Neg,
        Shl,
        Shr,
        And,
        Or,
        Xor,
        Copy,
        Read,
        Write,
        Ifeq,
        Ifgt,
        Ifst,
        Fcall,
        Ret,
        Label,
        Goto,
        Jz,
        PushI64,
        PushU64,
        Del,
        PushFn,
        AddressLoc,
        AddressArg,
        Invoke
    }
}