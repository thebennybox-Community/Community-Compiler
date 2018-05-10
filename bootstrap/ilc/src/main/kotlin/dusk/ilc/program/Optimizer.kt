package dusk.ilc.program

import dusk.ilc.opcodes.*
import kotlin.math.max

object Optimizer {

	fun optimize(program: ProgramData): ProgramData {
		val list = program.instructions.toMutableList()
		optimize(list)
		return ProgramData(list)
	}

	private fun optimize(instructions: MutableList<Instruction>) {
		var changed = false

		var i = 0
		loop@ while (i < instructions.size - 1) {
			var iterationChanged = true
			val a = instructions[i++]
			val b = instructions[i]

			if (a.opcode in meta) {
				instructions.removeAt(i - 1)
				instructions.add(0, a)
				// Setting this to false means that this optimization will not cause multiple passes.
				iterationChanged = false
			} else if (b.opcode == OpCodes.DELE) {
				when (a.opcode) {
					in push -> {
						// [push, pop] = []
						instructions.removeAt(i--)
						instructions.removeAt(i)
					}
					in popPush -> {
						// [pop, push, pop] = [pop]
						instructions.removeAt(i--)
					}
					in popPopPush -> {
						// [pop, pop, push, pop] = [pop, pop]
						instructions[--i] = Instruction(OpCodes.DELE)
					}
					else -> iterationChanged = false
				}
			} else if (isNumericPush(a.opcode) && isNumericPush(b.opcode) && i + 1 < instructions.size) {
				val c = instructions[i + 1]
				if (c.opcode in intBinaryArithmetic) {
					i = optimizeIntegerArithmetic(a, b, c, instructions, i, when {
						expectsSignedInt(a.opcode) && expectsSignedInt(b.opcode) -> this::signedToInstruction
						expectsUnsignedInt(a.opcode) && expectsUnsignedInt(b.opcode) -> this::unsignedToInstruction
						else -> continue@loop
					})
				} else if (c.opcode in floatBinaryArithmetic && expectsFloat(a.opcode) && expectsFloat(b.opcode)) {
					i = optimizeFloatArithmetic(a, b, c, instructions, i)
				} else {
					iterationChanged = false
				}
			} else {
				iterationChanged = false
			}

			changed = changed || iterationChanged
		}

		if (changed)
			optimize(instructions)
	}

	/**
	 * Opcodes that push one item with no other side-effects
	 */
	private val push = arrayOf(
			OpCodes.PU08, OpCodes.PU16, OpCodes.PU32, OpCodes.PU64,
			OpCodes.PI08, OpCodes.PI16, OpCodes.PI32, OpCodes.PI64,
			OpCodes.PF32, OpCodes.PF64,
			OpCodes.PTRU, OpCodes.PFLS,
			OpCodes.PSTR, OpCodes.PFUN, OpCodes.PLBL,
			OpCodes.DUPE,
			OpCodes.LLOC, OpCodes.LARG, OpCodes.LGLO,
			OpCodes.ADRL, OpCodes.ADRA, OpCodes.ADRG
	)

	/**
	 * Opcodes that pop one item and push one item with no other side-effects
	 */
	private val popPush = arrayOf(
			OpCodes.CAST, OpCodes.INEG, OpCodes.FNEG
	)

	/**
	 * Opcodes that pop two items and push one item with no other side-effects
	 */
	private val popPopPush = arrayOf(
			OpCodes.CMPE, OpCodes.CMPG, OpCodes.CPGE, OpCodes.CMPL, OpCodes.CPLE, OpCodes.CPNE,
			OpCodes.IADD, OpCodes.ISUB, OpCodes.IMUL, OpCodes.IDIV, OpCodes.IMOD,
			OpCodes.FADD, OpCodes.FSUB, OpCodes.FMUL, OpCodes.FDIV, OpCodes.FMOD,
			OpCodes.BSHL, OpCodes.BSHR, OpCodes.BAND, OpCodes.BWOR, OpCodes.BXOR
	)

	/**
	 * Opcodes that do not execute at runtime
	 */
	private val meta = arrayOf(
			OpCodes.EXFN,
			OpCodes.INFN,
			OpCodes.FPRM,
			OpCodes.FLOC,
			OpCodes.GLOB,
			OpCodes.DATA
	)

	private fun expectsInt(opcode: OpCode): Boolean {
		return expectsSignedInt(opcode) || expectsUnsignedInt(opcode)
	}

	private fun expectsFloat(opcode: OpCode): Boolean {
		return opcode.argumentTypes[0] is FloatType<*>
	}

	private fun expectsSignedInt(opcode: OpCode): Boolean {
		return opcode.argumentTypes[0] is IntegerType<*>
	}

	private fun expectsUnsignedInt(opcode: OpCode): Boolean {
		return opcode.argumentTypes[0] is UnsignedType<*>
	}

	private val intBinaryArithmetic = arrayOf(
			OpCodes.IADD, OpCodes.ISUB, OpCodes.IMUL, OpCodes.IDIV, OpCodes.IMOD
	)

	private val floatBinaryArithmetic = arrayOf(
			OpCodes.FADD, OpCodes.FSUB, OpCodes.FMUL, OpCodes.FDIV, OpCodes.FMOD
	)

	private fun isNumericPush(code: OpCode): Boolean {
		if (code !is OpCodes.Push)
			return false

		val type = code.argumentTypes[0]
		return type is IntegerType<*> || type is UnsignedType<*> || type is FloatType<*>
	}

	private inline fun optimizeIntegerArithmetic(
			a: Instruction,
			b: Instruction,
			c: Instruction,
			instructions: MutableList<Instruction>,
			index: Int,
			instructionFunc: (Int, Long) -> Instruction
	): Int {
		var i = index
		val va = (a.args[0].value as Number).toLong()
		val vb = (b.args[0].value as Number).toLong()

		val result = executeIntegerArithmetic(c, va, vb)

		val sa = a.opcode.argumentTypes[0].size
		val sb = b.opcode.argumentTypes[0].size
		val size = max(sa, sb)

		instructions.removeAt(i + 1)
		instructions.removeAt(i--)
		instructions[i] = instructionFunc(size, result)
		return i
	}

	private fun optimizeFloatArithmetic(a: Instruction, b: Instruction, c: Instruction, instructions: MutableList<Instruction>, index: Int): Int {
		var i = index
		val va = (a.args[0].value as Number).toDouble()
		val vb = (b.args[0].value as Number).toDouble()

		val result = executeFloatArithmetic(c, va, vb)

		val sa = a.opcode.argumentTypes[0].size
		val sb = b.opcode.argumentTypes[0].size
		val size = max(sa, sb)

		instructions.removeAt(i + 1)
		instructions.removeAt(i--)
		instructions[i] = floatToInstruction(size, result)
		return i
	}

	private fun signedToInstruction(size: Int, result: Long) = when (size) {
		1 -> Instruction(OpCodes.PI08, ArgInt8(result.toByte()))
		2 -> Instruction(OpCodes.PI16, ArgInt16(result.toShort()))
		4 -> Instruction(OpCodes.PI32, ArgInt32(result.toInt()))
		8 -> Instruction(OpCodes.PI64, ArgInt64(result))
		else -> throw IllegalStateException("Unexpected state in optimizer $size")
	}

	private fun unsignedToInstruction(size: Int, result: Long) = when (size) {
		1 -> Instruction(OpCodes.PU08, ArgUnsigned8(result.toByte()))
		2 -> Instruction(OpCodes.PU16, ArgUnsigned16(result.toShort()))
		4 -> Instruction(OpCodes.PU32, ArgUnsigned32(result.toInt()))
		8 -> Instruction(OpCodes.PU64, ArgUnsigned64(result))
		else -> throw IllegalStateException("Unexpected state in optimizer $size")
	}

	private fun floatToInstruction(size: Int, result: Double) = when (size) {
		4 -> Instruction(OpCodes.PF32, ArgFloat32(result.toFloat()))
		8 -> Instruction(OpCodes.PF64, ArgFloat64(result))
		else -> throw IllegalStateException("Unexpected state in optimizer $size")
	}

	private fun executeIntegerArithmetic(instr: Instruction, a: Long, b: Long): Long {
		return when (instr.opcode) {
			OpCodes.IADD -> a + b
			OpCodes.ISUB -> a - b
			OpCodes.IMUL -> a * b
			OpCodes.IDIV -> a / b
			OpCodes.IMOD -> a % b
			else -> throw IllegalStateException("Unexpected state in optimizer")
		}
	}

	private fun executeFloatArithmetic(instr: Instruction, a: Double, b: Double): Double {
		return when (instr.opcode) {
			OpCodes.FADD -> a + b
			OpCodes.FSUB -> a - b
			OpCodes.FMUL -> a * b
			OpCodes.FDIV -> a / b
			OpCodes.FMOD -> a % b
			else -> throw IllegalStateException("Unexpected state in optimizer")
		}
	}
}