package dusk.ilc.opcodes

data class Instruction(val opcode: OpCode, val args: List<Argument<*>> = emptyList()) {
	constructor(opcode: OpCode, vararg args: Argument<*>) : this(opcode, args.toList())

	override fun toString(): String {
		if (args.isEmpty())
			return opcode.name

		return "${opcode.name} ${args.joinToString(" ")}"
	}
}