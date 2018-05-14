#include "ILemitter.h"

void ILemitter::remove_last()
{
    stream.pop_back();
}

void ILemitter::no_operation() {
    w(NOOP);
}

void ILemitter::push_u8(uint8_t x) {
    w(PU08);
    w(x);
}

void ILemitter::push_u16(uint16_t x) {
    w(PU16);
    w(x);
}

void ILemitter::push_u32(uint32_t x) {
    w(PU32);
    w(x);
}

void ILemitter::push_u64(uint64_t x) {
    w(PU64);
    w(x);
}

void ILemitter::push_i8(int8_t x) {
    w(PI08);
    w(x);
}

void ILemitter::push_i16(int16_t x) {
    w(PI16);
    w(x);
}

void ILemitter::push_i32(int32_t x) {
    w(PI32);
    w(x);
}

void ILemitter::push_i64(int64_t x) {
    w(PI64);
    w(x);
}

void ILemitter::push_f32(float f) {
    w(PF32);
    w(f);
}

void ILemitter::push_f64(double d) {
    w(PF64);
    w(d);
}

void ILemitter::push_true() {
    w(PTRU);
}

void ILemitter::push_false() {
    w(PFLS);
}

void ILemitter::push_str(const char *str) {
    w(PSTR);
    w(str);
}

void ILemitter::push_function(const char *lbl) {
    w(PFUN);
    w(lbl);
}

void ILemitter::push_label(const char *lbl) {
    w(PLBL);
    w(lbl);
}

void ILemitter::cast(uint8_t type) {
    w(CAST);
    w(type);
}

void ILemitter::_delete() {
    w(DELE);
}

void ILemitter::swap() {
    w(SWAP);
}

void ILemitter::duplicate() {
    w(DUPE);
}

void ILemitter::compare_equal() {
    w(CMPE);
}

void ILemitter::compare_greater_than() {
    w(CMPG);
}

void ILemitter::compare_greateror_equal() {
    w(CPGE);
}

void ILemitter::compare_less_than() {
    w(CMPL);
}

void ILemitter::compare_lessor_equal() {
    w(CPGE);
}

void ILemitter::function(const char *lbl) {
    w(FUNC);
    w(lbl);
}

void ILemitter::_return() {
    w(RETN);
}

void ILemitter::call(const char *lbl) {
    w(CALL);
    w(lbl);
}

void ILemitter::call_stack(
    uint8_t return_type, uint32_t *args, uint32_t arg_count) {
    w(CALS);
    w(return_type);
    w(arg_count);

    for(uint32_t i = 0; i < arg_count; i++) {
        w(args[i]);
    }
}

void ILemitter::label(const char *lbl) {
    w(LABL);
    w(lbl);
}

void ILemitter::jump(const char *lbl) {
    w(JUMP);
    w(lbl);
}

void ILemitter::jump_equal_zero(const char *lbl) {
    w(JEQZ);
    w(lbl);
}

void ILemitter::jump_not_equal_zero(const char *lbl) {
    w(JNEZ);
    w(lbl);
}

void ILemitter::jump_greater_than_zero(const char *lbl) {
    w(JGTZ);
    w(lbl);
}

void ILemitter::jump_greater_equal_zero(const char *lbl) {
    w(JGEZ);
    w(lbl);
}

void ILemitter::jump_less_than_zero(const char *lbl) {
    w(JLTZ);
    w(lbl);
}

void ILemitter::jump_less_equal_zero(const char *lbl) {
    w(JLEZ);
    w(lbl);
}

void ILemitter::load_local(const char *lbl) {
    w(LLOC);
    w(lbl);
}

void ILemitter::store_local(const char *lbl) {
    w(SLOC);
    w(lbl);
}

void ILemitter::address_local(const char *lbl) {
    w(ADRL);
    w(lbl);
}

void ILemitter::address_stack() {
    w(ADRS);
}

void ILemitter::load_argument(const char *lbl) {
    w(LARG);
    w(lbl);
}

void ILemitter::store_argument(const char *lbl) {
    w(SARG);
    w(lbl);
}

void ILemitter::address_argument(const char *lbl) {
    w(ADRA);
    w(lbl);
}

void ILemitter::load_global(const char *lbl) {
    w(LGLO);
    w(lbl);
}

void ILemitter::store_global(const char *lbl) {
    w(SGLO);
    w(lbl);
}

void ILemitter::address_global(const char *lbl) {
    w(ADRG);
    w(lbl);
}

void ILemitter::read() {
    w(READ);
}

void ILemitter::write() {
    w(WRIT);
}

void ILemitter::integer_add() {
    w(IADD);
}

void ILemitter::integer_subtract() {
    w(ISUB);
}

void ILemitter::integer_multiply() {
    w(IMUL);
}

void ILemitter::integer_divide() {
    w(IDIV);
}

void ILemitter::integer_remainder() {
    w(IMOD);
}

void ILemitter::integer_negate() {
    w(INEG);
}

void ILemitter::float_add() {
    w(FADD);
}

void ILemitter::float_subtract() {
    w(FSUB);
}

void ILemitter::float_multiply() {
    w(FMUL);
}

void ILemitter::float_divide() {
    w(FDIV);
}

void ILemitter::float_remainder() {
    w(FMOD);
}

void ILemitter::float_negate() {
    w(FNEG);
}

void ILemitter::bitwise_left_shift() {
    w(BSHL);
}

void ILemitter::bitwise_right_shift() {
    w(BSHR);
}

void ILemitter::bitwise_and() {
    w(BAND);
}

void ILemitter::bitwise_or() {
    w(BWOR);
}

void ILemitter::bitwise_xor() {
    w(BXOR);
}

void ILemitter::external_function(
    const char *name,
    uint8_t type,
    uint32_t arg_count,
    uint8_t *args) {
    w(EXFN);
    w(name);
    w(type);
    w(arg_count);

    for(uint32_t i = 0; i < arg_count; i++) {
        w(args[i]);
    }
}

void ILemitter::internal_function(const char *name, uint8_t type) {
    w(INFN);
    w(name);
    w(type);
}

void ILemitter::function_parameter(
    const char *func, const char *name, uint8_t type) {
    w(FPRM);
    w(func);
    w(name);
    w(type);
}

void ILemitter::function_local(
    const char *func, const char *name, uint8_t type) {
    w(FLOC);
    w(func);
    w(name);
    w(type);
}

void ILemitter::global(const char *name, uint8_t type) {
    w(GLOB);
    w(name);
    w(type);
}

void ILemitter::data(const char *name, const char *data) {
    w(DATA);
    w(name);
    w(data);
}
