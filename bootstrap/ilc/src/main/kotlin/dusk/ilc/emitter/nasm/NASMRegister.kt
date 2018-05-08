package dusk.ilc.emitter.nasm

import dusk.ilc.util.signChar
import kotlin.math.absoluteValue

enum class NASMRegister: NASMValue.Source, NASMValue.Target {
	RAX,
	RBX,
	RCX,
	RDX,
	RSI,
	RDI,
	RBP,
	RSP,

	EAX,
	EBX,
	ECX,
	EDX,
	ESI,
	EDI,
	EBP,
	ESP,

	AX,
	BX,
	CX,
	DX,
	SI,
	DI,
	BP,
	SP,

	AXL,
	BXL,
	CXL,
	DXL,
	SIL,
	DIL,
	BPL,
	SPL;

	override val text = name

	operator fun plus(offset: Int) = NASMRegisterOffset(this, offset)
	operator fun minus(offset: Int) = NASMRegisterOffset(this, -offset)

	val deref = this + 0
}

data class NASMRegisterOffset(val register: NASMRegister, val offset: Int): NASMValue.Source, NASMValue.Target {
	override val text = when (offset) {
		0 -> "[${register.text}]"
		else -> "[${register.text}${offset.signChar}${offset.absoluteValue}]"
	}
}