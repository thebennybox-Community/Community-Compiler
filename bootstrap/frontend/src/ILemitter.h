#ifndef SRC_ILEMMITER_H
#define SRC_ILEMMITER_H

#include <stdio.h>
#include <string.h>
#include <vector>

#define NOOP 0xFF
//**Push Numeric**
#define PU08 (unsigned char)0x00
#define PU16 (unsigned char)0x01
#define PU32 (unsigned char)0x02
#define PU64 (unsigned char)0x03
#define PI08 (unsigned char)0x04
#define PI16 (unsigned char)0x05
#define PI32 (unsigned char)0x06
#define PI64 (unsigned char)0x07
#define PF32 (unsigned char)0x08
#define PF64 (unsigned char)0x09
#define PTRU (unsigned char)0x10
#define PFLS (unsigned char)0x11
#define PSTR (unsigned char)0x12
#define PFUN (unsigned char)0x13
#define PLBL (unsigned char)0x14
#define CAST (unsigned char)0x20
#define DELE (unsigned char)0x21
#define SWAP (unsigned char)0x22
#define DUPE (unsigned char)0x23
#define CMPE (unsigned char)0x30
#define CMPG (unsigned char)0x31
#define CPGE (unsigned char)0x32
#define CMPL (unsigned char)0x33
#define CPLE (unsigned char)0x34
#define FUNC (unsigned char)0x40
#define RETN (unsigned char)0x41
#define CALL (unsigned char)0x42
#define CALS (unsigned char)0x43
#define LABL (unsigned char)0x50
#define JUMP (unsigned char)0x51
#define JEQZ (unsigned char)0x52
#define JNEZ (unsigned char)0x53
#define JGTZ (unsigned char)0x54
#define JGEZ (unsigned char)0x55
#define JLTZ (unsigned char)0x56
#define JLEZ (unsigned char)0x57
#define LLOC (unsigned char)0x60
#define SLOC (unsigned char)0x61
#define ADRL (unsigned char)0x62
#define LARG (unsigned char)0x63
#define SARG (unsigned char)0x64
#define ADRA (unsigned char)0x65
#define LGLO (unsigned char)0x66
#define SGLO (unsigned char)0x67
#define ADRG (unsigned char)0x68
#define READ (unsigned char)0x69
#define WRIT (unsigned char)0x6A
#define IADD (unsigned char)0x70
#define ISUB (unsigned char)0x71
#define IMUL (unsigned char)0x72
#define IDIV (unsigned char)0x73
#define IMOD (unsigned char)0x74
#define INEG (unsigned char)0x75
#define FADD (unsigned char)0x80
#define FSUB (unsigned char)0x81
#define FMUL (unsigned char)0x82
#define FDIV (unsigned char)0x83
#define FMOD (unsigned char)0x84
#define FNEG (unsigned char)0x85
#define BSHL (unsigned char)0x90
#define BSHR (unsigned char)0x91
#define BAND (unsigned char)0x92
#define BWOR (unsigned char)0x93
#define BXOR (unsigned char)0x94

// DataType Encoding
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

class ILemitter {
public:
    std::vector<unsigned char> stream;

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
    void push_str(const char *str);
    void push_function(const char *lbl);
    void push_label(const char *lbl);
    void cast(unsigned const char type);
    void _delete();
    void swap();
    void duplicate();
    void compare_equal();
    void compare_greater_than();
    void compare_greateror_equal();
    void compare_less_than();
    void compare_lessor_equal();
    void function(const char *lbl);
    void _return();
    void call(const char *lbl);
    void call_stack(
        unsigned int return_type, unsigned int args[], unsigned int count);
    void label(const char *lbl);
    void jump(const char *lbl);
    void jump_equal_zero(const char *lbl);
    void jump_not_equal_zero(const char *lbl);
    void jump_greater_than_zero(const char *lbl);
    void jump_greater_equal_zero(const char *lbl);
    void jump_less_than_zero(const char *lbl);
    void jump_less_equal_zero(const char *lbl);
    void load_local(const char *lbl);
    void store_local(const char *lbl);
    void address_local(const char *lbl);
    void load_argument(const char *lbl);
    void store_argument(const char *lbl);
    void address_argument(const char *lbl);
    void load_global(const char *lbl);
    void store_global(const char *lbl);
    void address_global(const char *lbl);
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
    void w(unsigned char x) {
        stream.push_back(x);
        // printf("%hhu ", x);
    }

    void w(unsigned short x) {
        w((unsigned char)((x >> 8) & 0xFF));
        w((unsigned char)x);
    }

    void w(unsigned int x) {
        w((unsigned char)((x >> 24) & 0xFF));
        w((unsigned char)((x >> 16) & 0xFF));
        w((unsigned char)((x >> 8) & 0xFF));
        w((unsigned char)x);
    }

    void w(unsigned long x) {
        w((unsigned char)((x >> 56) & 0xFF));
        w((unsigned char)((x >> 48) & 0xFF));
        w((unsigned char)((x >> 40) & 0xFF));
        w((unsigned char)((x >> 32) & 0xFF));
        w((unsigned char)((x >> 24) & 0xFF));
        w((unsigned char)((x >> 16) & 0xFF));
        w((unsigned char)((x >> 8) & 0xFF));
        w((unsigned char)x);
    }

    void w(char x) {
        w((unsigned char)x);
    }

    void w(const char *x) {
        unsigned int z = strlen(x);
        w(z);
        for(int i = 0; i < z; i++) {
            w((unsigned char)x[i]);
        }
    }

    void w(short x) {
        w((char)((x >> 8) & 0xFF));
        w((char)x);
    }

    void w(int x) {
        w((unsigned char)((x >> 24) & 0xFF));
        w((unsigned char)((x >> 16) & 0xFF));
        w((unsigned char)((x >> 8) & 0xFF));
        w((unsigned char)x);
    }

    void w(long x) {
        w((unsigned char)((x >> 56) & 0xFF));
        w((unsigned char)((x >> 48) & 0xFF));
        w((unsigned char)((x >> 40) & 0xFF));
        w((unsigned char)((x >> 32) & 0xFF));
        w((unsigned char)((x >> 24) & 0xFF));
        w((unsigned char)((x >> 16) & 0xFF));
        w((unsigned char)((x >> 8) & 0xFF));
        w((unsigned char)x);
    }

    void w(float x) {
        unsigned char const *p = reinterpret_cast<unsigned char const *>(&x);

        for(int i = 0; i != sizeof(float); ++i) {
            w(p[i]);
        }
    }

    void w(double x) {
        unsigned char const *p = reinterpret_cast<unsigned char const *>(&x);
        for(int i = 0; i != sizeof(double); ++i) {
            w(p[i]);
        }
    }
};

#endif