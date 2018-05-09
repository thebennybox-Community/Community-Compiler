package dusk.ilc.emitter.nasm

import dusk.ilc.util.toUnsignedListString

interface NASMInstruction {
	val text: String
}
interface NASMData: NASMInstruction

abstract class AbstractInstruction(override val text: String) : NASMInstruction

private typealias S = NASMValue.Source
private typealias T = NASMValue.Target
private typealias Instr = AbstractInstruction



object NOP : Instr("nop")

data class Push(val source: S) : Instr("push ${source.text}")
data class Pop(val target: T) : Instr("pop ${target.text}")
data class Label(val name: String) : Instr("$name:")
data class Global(val name: String) : Instr("global $name")
data class Extern(val name: String) : Instr("extern $name")
data class Call(val target: NASMValue) : Instr("call ${target.text}")

data class Negate(val source: S) : Instr("neg ${source.text}")
data class Compare(val a: S, val b: S) : Instr("cmp ${a.text}, ${b.text}")

data class Jump(val name: String) : Instr("jmp $name")
data class JumpEqual(val name: String) : Instr("je $name")
data class JumpNotEqual(val name: String) : Instr("jne $name")
data class JumpLess(val name: String) : Instr("jl $name")
data class JumpLessEqual(val name: String) : Instr("jle $name")
data class JumpGreater(val name: String) : Instr("jg $name")
data class JumpGreaterEqual(val name: String) : Instr("jge $name")

object Leave: Instr("leave")
object Ret: Instr("ret")

data class Add(override val a: S, override val b: S) : BinaryOp("add")
data class Subtract(override val a: S, override val b: S) : BinaryOp("sub")
data class Multiply(override val a: S, override val b: S) : BinaryOp("imul")
data class Divide(val source: S) : Instr("div ${source.text}")
data class ShiftLeft(override val a: S, override val b: S) : BinaryOp("shl")
data class ShiftRight(override val a: S, override val b: S) : BinaryOp("shr")
data class And(override val a: S, override val b: S) : BinaryOp("and")
data class Or(override val a: S, override val b: S) : BinaryOp("or")
data class Xor(override val a: S, override val b: S) : BinaryOp("xor")

data class Move(val target: T, val source: S) : Instr("mov ${target.text}, ${source.text}")
data class LoadEffectiveAddress(val target: T, val source: S) : Instr("lea ${target.text}, ${source.text}")

data class FloatingPointLoad(val source: S) : Instr("fld qword ${source.text}")
data class FloatingPointStore(val target: T) : Instr("fstp qword ${target.text}")
object FloatingPointAdd : Instr("fadd")
object FloatingPointSubtract : Instr("fsub")
object FloatingPointMultiply : Instr("fmul")
object FloatingPointDivide : Instr("fdiv")

object FloatingPointInit: Instr("finit")

data class Comment(val content: String) : Instr("; $content")

abstract class BinaryOp(val code: String) : NASMInstruction {
	abstract val a: S
	abstract val b: S

	override val text get() = "$code ${a.text}, ${b.text}"
}

class DataByteArray(val name: String, val bytes: ByteArray) : NASMData {
	override val text = "$name db ${bytes.toUnsignedListString()}"
}

class DataDouble(val name: String, val value: Double): NASMData {
	override val text = "$name dq $value"
}