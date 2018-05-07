#include "ILemmiter.h"

void ILemmiter::no_operation() {
    w(NOOP);
}

void ILemmiter::push_u8(unsigned char x) {
    w(PU08);
    w(x);
}

void ILemmiter::push_u16(unsigned short x) {
    w(PU16);
    w(x);
}

void ILemmiter::push_u32(unsigned int x) {
    w(PU32);
    w(x);
}

void ILemmiter::push_u64(unsigned long x) {
    w(PU64);
    w(x);
}

void ILemmiter::push_i8(char x) {
    w(PI08);
    w(x);
}

void ILemmiter::push_i16(short x) {
    w(PI16);
    w(x);
}

void ILemmiter::push_i32(int x) {
    w(PI32);
    w(x);
}

void ILemmiter::push_i64(long x) {
    w(PI64);
    w(x);
}

void ILemmiter::push_f32(float f) {
    w(PF32);
    w(f);
}

void ILemmiter::push_f64(double d) {
    w(PF64);
    w(d);
}

void ILemmiter::push_true() {
    w(PTRU);
}

void ILemmiter::push_false() {
    w(PFLS);
}

void ILemmiter::push_str(char *str) {
    w(PSTR);
    w(str);
}

void ILemmiter::push_function(char *lbl) {
    w(PFUN);
    w(lbl);
}

void ILemmiter::push_label(char *lbl) {
    w(PLBL);
    w(lbl);
}

void ILemmiter::cast(unsigned char type) {
    w(CAST);
    w(type);
}

void ILemmiter::_delete() {
    w(DELE);
}

void ILemmiter::swap() {
    w(SWAP);
}

void ILemmiter::duplicate() {
    w(DUPE);
}

void ILemmiter::compare_equal() {
    w(CMPE);
}

void ILemmiter::compare_greater_than() {
    w(CMPG);
}

void ILemmiter::compare_greateror_equal() {
    w(CPGE);
}

void ILemmiter::compare_less_than() {
    w(CMPL);
}

void ILemmiter::compare_lessor_equal() {
    w(CPGE);
}

void ILemmiter::function(char *lbl) {
    w(FUNC);
    w(lbl);
}

void ILemmiter::_return() {
    w(RETN);
}

void ILemmiter::call(char *lbl) {
    w(CALL);
    w(lbl);
}

void ILemmiter::call_stack(unsigned int return_type, unsigned int args[],
                           unsigned int total) {
    w(CALS);
    w(return_type);
    w(total);
    for(unsigned int i = 0; i < total; i++) { w(args[i]); }
}

void ILemmiter::label(char *lbl) {
    w(LABL);
    w(lbl);
}

void ILemmiter::jump(char *lbl) {
    w(LABL);
    w(lbl);
}

void ILemmiter::jump_equal_zero(char *lbl) {
    w(JEQZ);
    w(lbl);
}

void ILemmiter::jump_not_equal_zero(char *lbl) {
    w(JNEZ);
    w(lbl);
}

void ILemmiter::jump_greater_than_zero(char *lbl) {
    w(JGTZ);
    w(lbl);
}

void ILemmiter::jump_greater_equal_zero(char *lbl) {
    w(JGEZ);
    w(lbl);
}

void ILemmiter::jump_less_than_zero(char *lbl) {
    w(JLTZ);
    w(lbl);
}

void ILemmiter::jump_less_equal_zero(char *lbl) {
    w(JLEZ);
    w(lbl);
}

void ILemmiter::load_local(char *lbl) {
    w(LLOC);
    w(lbl);
}

void ILemmiter::store_local(char *lbl) {
    w(SLOC);
    w(lbl);
}

void ILemmiter::address_local(char *lbl) {
    w(ADRL);
    w(lbl);
}

void ILemmiter::load_argument(char *lbl) {
    w(LARG);
    w(lbl);
}

void ILemmiter::store_argument(char *lbl) {
    w(SARG);
    w(lbl);
}

void ILemmiter::address_argument(char *lbl) {
    w(ADRA);
    w(lbl);
}

void ILemmiter::load_global(char *lbl) {
    w(LGLO);
    w(lbl);
}

void ILemmiter::store_global(char *lbl) {
    w(SGLO);
    w(lbl);
}

void ILemmiter::address_global(char *lbl) {
    w(ADRG);
    w(lbl);
}

void ILemmiter::read() {
    w(READ);
}

void ILemmiter::write() {
    w(WRIT);
}

void ILemmiter::integer_add() {
    w(IADD);
}

void ILemmiter::integer_subtract() {
    w(ISUB);
}

void ILemmiter::integer_multiply() {
    w(IMUL);
}

void ILemmiter::integer_divide() {
    w(IDIV);
}

void ILemmiter::integer_remainder() {
    w(IMOD);
}

void ILemmiter::integer_negate() {
    w(INEG);
}

void ILemmiter::float_add() {
    w(FADD);
}

void ILemmiter::float_subtract() {
    w(FSUB);
}

void ILemmiter::float_multiply() {
    w(FMUL);
}

void ILemmiter::float_divide() {
    w(FDIV);
}

void ILemmiter::float_remainder() {
    w(FMOD);
}

void ILemmiter::float_negate() {
    w(FNEG);
}

void ILemmiter::bitwise_left_shift() {
    w(BSHL);
}

void ILemmiter::bitwise_right_shift() {
    w(BSHR);
}

void ILemmiter::bitwise_and() {
    w(BAND);
}

void ILemmiter::bitwise_or() {
    w(BWOR);
}

void ILemmiter::bitwise_xor() {
    w(BXOR);
}
