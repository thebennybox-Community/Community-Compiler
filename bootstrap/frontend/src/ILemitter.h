#ifndef SRC_ILEMMITER_H
#define SRC_ILEMMITER_H

#include <vector>
#include <map>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "AstDefs.h"

#define NOOP (uint8_t)0xFF
//**Push Numeric**
#define PU08 (uint8_t)0x00
#define PU16 (uint8_t)0x01
#define PU32 (uint8_t)0x02
#define PU64 (uint8_t)0x03
#define PI08 (uint8_t)0x04
#define PI16 (uint8_t)0x05
#define PI32 (uint8_t)0x06
#define PI64 (uint8_t)0x07
#define PF32 (uint8_t)0x08
#define PF64 (uint8_t)0x09
#define PTRU (uint8_t)0x10
#define PFLS (uint8_t)0x11
#define PSTR (uint8_t)0x12
#define PFUN (uint8_t)0x13
#define PLBL (uint8_t)0x14
#define CAST (uint8_t)0x20
#define DELE (uint8_t)0x21
#define SWAP (uint8_t)0x22
#define DUPE (uint8_t)0x23
#define CMPE (uint8_t)0x30
#define CMPG (uint8_t)0x31
#define CPGE (uint8_t)0x32
#define CMPL (uint8_t)0x33
#define CPLE (uint8_t)0x34
#define FUNC (uint8_t)0x40
#define RETN (uint8_t)0x41
#define CALL (uint8_t)0x42
#define CALS (uint8_t)0x43
#define LABL (uint8_t)0x50
#define JUMP (uint8_t)0x51
#define JEQZ (uint8_t)0x52
#define JNEZ (uint8_t)0x53
#define JGTZ (uint8_t)0x54
#define JGEZ (uint8_t)0x55
#define JLTZ (uint8_t)0x56
#define JLEZ (uint8_t)0x57
#define LLOC (uint8_t)0x60
#define SLOC (uint8_t)0x61
#define ADRL (uint8_t)0x62
#define LARG (uint8_t)0x63
#define SARG (uint8_t)0x64
#define ADRA (uint8_t)0x65
#define LGLO (uint8_t)0x66
#define SGLO (uint8_t)0x67
#define ADRG (uint8_t)0x68
#define READ (uint8_t)0x69
#define WRIT (uint8_t)0x6A
#define IADD (uint8_t)0x70
#define ISUB (uint8_t)0x71
#define IMUL (uint8_t)0x72
#define IDIV (uint8_t)0x73
#define IMOD (uint8_t)0x74
#define INEG (uint8_t)0x75
#define FADD (uint8_t)0x80
#define FSUB (uint8_t)0x81
#define FMUL (uint8_t)0x82
#define FDIV (uint8_t)0x83
#define FMOD (uint8_t)0x84
#define FNEG (uint8_t)0x85
#define BSHL (uint8_t)0x90
#define BSHR (uint8_t)0x91
#define BAND (uint8_t)0x92
#define BWOR (uint8_t)0x93
#define BXOR (uint8_t)0x94
#define ADRS (uint8_t)0x6b

#define EXFN (uint8_t)0xE0
#define INFN (uint8_t)0xE1
#define FPRM (uint8_t)0xE2
#define FLOC (uint8_t)0xE3
#define GLOB (uint8_t)0xE4
#define DATA (uint8_t)0xE5

// DataType Encoding
#define U8   (uint8_t)0x0
#define U16  (uint8_t)0x1
#define U32  (uint8_t)0x2
#define U64  (uint8_t)0x3
#define I8   (uint8_t)0x4
#define I16  (uint8_t)0x5
#define I32  (uint8_t)0x6
#define I64  (uint8_t)0x7
#define F32  (uint8_t)0x8
#define F64  (uint8_t)0x9
#define STR  (uint8_t)0xA
#define PTR  (uint8_t)0xB
#define VOID (uint8_t)0xF

static const std::map<std::string, uint8_t> type_map = {
    {"u8",   U8},
    {"bool", U8},
    {"u16",  U16},
    {"u32",  U32},
    {"u64",  U64},
    {"i8",   I8},
    {"i16",  I16},
    {"i32",  I32},
    {"i64",  I64},
    {"f32",  F32},
    {"f64",  F64},
    {"str",  STR},
    {"ptr",  PTR},
    {"void", VOID},
};

class ILemitter {
public:
    std::vector<uint8_t> stream;

    void remove_last();
    void no_operation();
    void push_u8(uint8_t x);
    void push_u16(uint16_t x);
    void push_u32(uint32_t x);
    void push_u64(uint64_t x);
    void push_i8(int8_t x);
    void push_i16(int16_t x);
    void push_i32(int32_t x);
    void push_i64(int64_t x);
    void push_f32(float f);
    void push_f64(double d);
    void push_true();
    void push_false();
    void push_str(const char *str);
    void push_function(const char *lbl);
    void push_label(const char *lbl);
    void cast(uint8_t type);
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
    void call_stack(uint8_t return_type, uint32_t *args, uint32_t arg_count);
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
    void address_stack();
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

    void external_function(
        const char *name,
        uint8_t type,
        uint32_t arg_count,
        uint8_t *args);

    void internal_function(const char *name, uint8_t type);
    void function_parameter(const char *func, const char *name, uint8_t type);
    void function_local(const char *func, const char *name, uint8_t type);
    void global(const char *name, uint8_t type);
    void data(const char *name, const char *data);

    void w(uint8_t x) {
        stream.push_back(x);
        // printf("%hhu ", x);
    }

    void w(uint16_t x) {
        w((uint8_t)((x >> 8) & 0xFF));
        w((uint8_t)x);
    }

    void w(uint32_t x) {
        w((uint8_t)((x >> 24) & 0xFF));
        w((uint8_t)((x >> 16) & 0xFF));
        w((uint8_t)((x >> 8) & 0xFF));
        w((uint8_t)x);
    }

    void w(uint64_t x) {
        w((uint8_t)((x >> 56) & 0xFF));
        w((uint8_t)((x >> 48) & 0xFF));
        w((uint8_t)((x >> 40) & 0xFF));
        w((uint8_t)((x >> 32) & 0xFF));
        w((uint8_t)((x >> 24) & 0xFF));
        w((uint8_t)((x >> 16) & 0xFF));
        w((uint8_t)((x >> 8) & 0xFF));
        w((uint8_t)x);
    }

    void w(int8_t x) {
        w((uint8_t)x);
    }

    void w(const char *x) {
        uint32_t length = (uint32_t)strlen(x);
        w(length);

        for(uint32_t i = 0; i < length; i++) {
            w((uint8_t)x[i]);
        }
    }

    void w(int16_t x) {
        w((uint8_t)((x >> 8) & 0xFF));
        w((uint8_t)x);
    }

    void w(int32_t x) {
        w((uint8_t)((x >> 24) & 0xFF));
        w((uint8_t)((x >> 16) & 0xFF));
        w((uint8_t)((x >> 8) & 0xFF));
        w((uint8_t)x);
    }

    void w(int64_t x) {
        w((uint8_t)((x >> 56) & 0xFF));
        w((uint8_t)((x >> 48) & 0xFF));
        w((uint8_t)((x >> 40) & 0xFF));
        w((uint8_t)((x >> 32) & 0xFF));
        w((uint8_t)((x >> 24) & 0xFF));
        w((uint8_t)((x >> 16) & 0xFF));
        w((uint8_t)((x >> 8) & 0xFF));
        w((uint8_t)x);
    }

    void w(float x) {
        const uint8_t *p = reinterpret_cast<uint8_t*>(&x);

        for(unsigned int i = 0; i < sizeof(float); i++) {
            w(p[i]);
        }
    }

    void w(double x) {
        const uint8_t *p = reinterpret_cast<uint8_t*>(&x);

        for(unsigned int i = 0; i < sizeof(double); i++) {
            w(p[i]);
        }
    }
};

#endif // SRC_ILEMITTER_H
