package dusk.ilc.emitter.nasm

import dusk.ilc.opcodes.Argument

class NASMInstructionList(val list: MutableList<NASMInstruction> = ArrayList()): MutableList<NASMInstruction> by list {

	fun nop() = add(NOP)

	fun push(source: Long) = add(Push(NASMValue.LiteralInt(source)))
	fun push(source: Argument<*>) = add(Push(NASMEmitter.convert(source)))

	fun mov(target: NASMValue.Target, source: NASMValue.Source) = add(Move(target, source))
	fun lea(target: NASMValue.Target, source: NASMValue.Source) = add(LoadEffectiveAddress(target, source))

	fun push(source: NASMValue.Source) = add(Push(source))
	fun pop(target: NASMValue.Target) = add(Pop(target))
	fun label(name: String) = add(Label(name))
	fun global(name: String) = add(Global(name))
	fun extern(name: String) = add(Extern(name))
	fun call(name: String) = add(Call(NASMValue.Raw(name)))
	fun call(target: NASMValue) = add(Call(target))

	fun cmp(a: NASMValue.Source, b: NASMValue.Source) = add(Compare(a, b))
	fun jmp(name: String) = add(Jump(name))
	fun je(name: String) = add(JumpEqual(name))
	fun jne(name: String) = add(JumpNotEqual(name))
	fun jg(name: String) = add(JumpGreater(name))
	fun jge(name: String) = add(JumpGreaterEqual(name))
	fun jl(name: String) = add(JumpLess(name))
	fun jle(name: String) = add(JumpLessEqual(name))

	fun add(a: NASMValue.Source, b: NASMValue.Source) = add(Add(a, b))
	fun sub(a: NASMValue.Source, b: NASMValue.Source) = add(Subtract(a, b))
	fun mul(a: NASMValue.Source, b: NASMValue.Source) = add(Multiply(a, b))
	fun div(source: NASMValue.Source) = add(Divide(source))
	fun shl(a: NASMValue.Source, b: NASMValue.Source) = add(ShiftLeft(a, b))
	fun shr(a: NASMValue.Source, b: NASMValue.Source) = add(ShiftRight(a, b))
	fun and(a: NASMValue.Source, b: NASMValue.Source) = add(And(a, b))
	fun or(a: NASMValue.Source, b: NASMValue.Source) = add(Or(a, b))
	fun xor(a: NASMValue.Source, b: NASMValue.Source) = add(Xor(a, b))

	fun neg(source: NASMValue.Source) = add(Negate(source))

	fun leave() = add(Leave)
	fun ret() = add(Ret)

	fun finit() = add(FloatingPointInit)
	fun fld(source: NASMValue.Source) = add(FloatingPointLoad(source))
	fun fstp(target: NASMValue.Target) = add(FloatingPointStore(target))
	fun fadd() = add(FloatingPointAdd)
	fun fsub() = add(FloatingPointSubtract)
	fun fmul() = add(FloatingPointMultiply)
	fun fdiv() = add(FloatingPointDivide)

	fun dataByteArray(name: String, bytes: ByteArray) = add(DataByteArray(name, bytes))
	fun dataDouble(name: String, value: Double) = add(DataDouble(name, value))

	fun comment(content: String) = add(Comment(content))
}