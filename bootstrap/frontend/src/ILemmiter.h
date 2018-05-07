#ifndef SRC_ILEMMITER_H
#define SRC_ILEMMITER_H

#include <stdio.h>
#include <vector>
#include <string.h>

#define NOOP 0xFF
//**Push Numeric**
#define PU08 0x00
#define PU16 0x01
#define PU32 0x02
#define PU64 0x03
#define PI08 0x04
#define PI16 0x05
#define PI32 0x06
#define PI64 0x07
#define PF32 0x08
#define PF64 0x09
//**Push Misc**
#define PTRU 0x10
#define PFLS 0x11
#define PSTR 0x12
#define PFUN 0x13
#define PLBL 0x14
//**Stack Operations**
#define CAST 0x20
#define DELE 0x21
#define SWAP 0x22
#define DUPE 0x23
//**Comparison**
#define CMPE 0x30
#define CMPG 0x31
#define CPGE 0x32
#define CMPL 0x33
#define CPLE 0x34
//**Functions**
#define FUNC 0x40
#define RETN 0x41
#define CALL 0x42
#define CALS 0x43
//**Jump**
#define LABL 0x50
#define JUMP 0x51
#define JEQZ 0x52
#define JNEZ 0x53
#define JGTZ 0x54
#define JGEZ 0x55
#define JLTZ 0x56
#define JLEZ 0x57
//**Memory Operations**
#define LLOC 0x60
#define SLOC 0x61
#define ADRL 0x62
#define LARG 0x63
#define SARG 0x64
#define ADRA 0x65
#define LGLO 0x66
#define SGLO 0x67
#define ADRG 0x68
#define READ 0x69
#define WRIT 0x6A
//**Integer Arithmetic**
#define IADD 0x70
#define ISUB 0x71
#define IMUL 0x72
#define IDIV 0x73
#define IMOD 0x74
#define INEG 0x75
//**Float Arithmetic**
#define FADD 0x80
#define FSUB 0x81
#define FMUL 0x82
#define FDIV 0x83
#define FMOD 0x84
#define FNEG 0x85
//**Bitwise**
#define BSHL 0x90
#define BSHR 0x91
#define BAND 0x92
#define BWOR 0x93
#define BXOR 0x94

//DataType Encoding
#define U8 0b0000
#define U16 0b0001
#define U32 0b0010
#define U64 0b0011
#define I8 0b0100
#define I16 0b0101
#define I32 0b0110
#define I64 0b0111
#define F32 0b1000
#define F64 0b1001
#define BOOL 0b1010
#define STR 0b1011
#define PTR 0b1100

class ILemmiter {
  public:
    void no_operation();
    void push_u8(unsigned char x);
    void push_u16(unsigned short x);
    void push_u32(unsigned int x);
    void push_u64(unsigned long x);
    void push_i8(char x);
    void push_i16(short x);
    void push_i32(int x);
    void push_i64(long x);
    void push_f32(float f);
    void push_f64(double d);
    void push_true();
    void push_false();
    void push_str(char *str);
    void push_function(char *lbl);
    void push_label(char *lbl);
    void cast(unsigned char type);
    void _delete();
    void swap();
    void duplicate();
    void compare_equal();
    void compare_greater_than();
    void compare_greateror_equal();
    void compare_less_than();
    void compare_lessor_equal();
    void function(char *lbl);
    void _return();
    void call(char *lbl);
    void call_stack(unsigned int return_type, unsigned int args[],
                    unsigned int count);
    void label(char *lbl);
    void jump(char *lbl);
    void jump_equal_zero(char *lbl);
    void jump_not_equal_zero(char *lbl);
    void jump_greater_than_zero(char *lbl);
    void jump_greater_equal_zero(char *lbl);
    void jump_less_than_zero(char *lbl);
    void jump_less_equal_zero(char *lbl);
    void load_local(char *lbl);
    void store_local(char *lbl);
    void address_local(char *lbl);
    void load_argument(char *lbl);
    void store_argument(char *lbl);
    void address_argument(char *lbl);
    void load_global(char *lbl);
    void store_global(char *lbl);
    void address_global(char *lbl);
    void read();
    void write();
    void integer_add();
    void integer_subtract();
    void integer_multiply();
    void integer_divide();
    void integer_remainder();
    void integer_negate();
    void float_add();
    void float_subtract();
    void float_multiply();
    void float_divide();
    void float_remainder();
    void float_negate();
    void bitwise_left_shift();
    void bitwise_right_shift();
    void bitwise_and();
    void bitwise_or();
    void bitwise_xor();

  private:
    std::vector<unsigned char> stream;

    void w(unsigned char x) {
        stream.push_back(x);
    }

    void w(unsigned short x) {
        w((unsigned char)((x >> 8) & 0xFF));
        w(x);
    }

    void w(unsigned int x) {
        w((x >> 24) & 0xFF);
        w((x >> 16) & 0xFF);
        w((x >> 8) & 0xFF);
        w(x);
    }

    void w(unsigned long x) {
        w((x >> 56) & 0xFF);
        w((x >> 48) & 0xFF);
        w((x >> 40) & 0xFF);
        w((x >> 32) & 0xFF);
        w((x >> 24) & 0xFF);
        w((x >> 16) & 0xFF);
        w((x >> 8) & 0xFF);
        w(x);
    }

    void w(char x) {
        w((unsigned char)x);
    }

    void w(char *x) {
        w(strlen(x));
        for(int i = 0; i < strlen(x); i++) { w(x[i]); }
    }

    void w(short x) {
        w((char)((x >> 8) & 0xFF));
        w(x);
    }

    void w(int x) {
        w((x >> 24) & 0xFF);
        w((x >> 16) & 0xFF);
        w((x >> 8) & 0xFF);
        w(x);
    }

    void w(long x) {
        w((x >> 56) & 0xFF);
        w((x >> 48) & 0xFF);
        w((x >> 40) & 0xFF);
        w((x >> 32) & 0xFF);
        w((x >> 24) & 0xFF);
        w((x >> 16) & 0xFF);
        w((x >> 8) & 0xFF);
        w(x);
    }

    void w(float x) {
        unsigned char const *p = reinterpret_cast<unsigned char const *>(&x);

        for(int i = 0; i != sizeof(float); ++i) { w(p[i]); }
    }

    void w(double x) {
        unsigned char const *p = reinterpret_cast<unsigned char const *>(&x);
        for(int i = 0; i != sizeof(double); ++i) { w(p[i]); }
    }
};

#endif