package dusk.ilc.program

import dusk.ilc.opcodes.*
import java.nio.ByteBuffer

class ProgramData(val instructions: List<Instruction>) {

	val functions by lazy { findFunctions() }
	val globals by lazy { findGlobals() }
	val data by lazy { findData() }

	val isScript get() = "main" !in functions

	fun toNonScript(): ProgramData {
		if (!isScript)
			return this

		val new = ArrayList(instructions)

		val returnIndex = new.indexOfFirst { it.opcode == OpCodes.FUNC }

		val mainDef = Instruction(
				OpCodes.INFN,
				ArgIdentifier("main"),
				ArgType(TypeVoid),
				ArgTypeArray(emptyArray())
		)

		val mainDec = Instruction(OpCodes.FUNC, ArgIdentifier("main"))
		val returnInstr = Instruction(OpCodes.RETN)

		if (returnIndex == -1)
			new.add(returnInstr)
		else
			new.add(returnIndex, returnInstr)

		new.add(0, mainDef)
		new.add(0, mainDec)

		return ProgramData(new)
	}

	operator fun get(index: Int): Instruction {
		return instructions[index]
	}

	fun getFunction(index: Int): Function? {
		var i = index
		while (i >= 0) {
			val instr = this[i]
			if (instr.opcode != OpCodes.FUNC) {
				i--
				continue
			}

			val name = (instr.args[0] as ArgIdentifier).value
			return functions[name]
		}

		return null
	}

	private fun findData(): Map<String, ByteBuffer> {
		val data = HashMap<String, ByteBuffer>()

		for (instr in instructions) {
			if (instr.opcode != OpCodes.DATA)
				continue

			val name = (instr.args[0] as ArgIdentifier).value
			val bytes = (instr.args[1] as ArgByteArray).value

			if (name in data) {
				System.err.println("Warning: Duplicate data key \"$name\"")
				continue
			}

			val buffer = ByteBuffer.allocate(bytes.size)
			bytes.forEach { buffer.put(it) }
			buffer.flip()

			data[name] = buffer
		}

		return data
	}

	private fun findGlobals(): Variables {
		val globals = HashMap<String, Type<*>>()

		for (instr in instructions) {
			if (instr.opcode != OpCodes.GLOB)
				continue

			val name = (instr.args[0] as ArgIdentifier).value
			val type = (instr.args[1] as ArgType).value

			if (name in globals) {
				System.err.println("Warning: Duplicate global \"$name\"")
				continue
			}

			globals[name] = type
		}

		return globals
	}

	private fun findFunctions(): Map<String, Function> {
		val locals = findVariables(false)
		val arguments = findVariables(true)
		val functions = HashMap<String, Function>()

		for (instr in instructions) {
			if (instr.opcode != OpCodes.EXFN && instr.opcode != OpCodes.INFN)
				continue

			val name = (instr.args[0] as ArgIdentifier).value
			if (name in functions) {
				System.err.println("Warning: Duplicate function \"$name\"")
				continue
			}

			val returnType = (instr.args[1] as ArgType).value

			functions[name] = if (instr.opcode == OpCodes.EXFN) {
				val paramTypes = (instr.args[2] as ArgTypeArray).value
				val params = paramTypes.mapIndexed { i, t -> "p$i" to t}.toMap()
				Function.External(name, returnType, params)
			} else {
				val params = arguments[name] ?: emptyMap()
				Function.Internal(name, returnType, params, locals[name] ?: emptyMap())
			}
		}

		return functions
	}

	private fun findVariables(arguments: Boolean): Map<String, Variables> {
		val locals = HashMap<String, MutableMap<String, Type<*>>>()

		for (instr in instructions) {
			if (!arguments && instr.opcode != OpCodes.FLOC)
				continue
			if (arguments && instr.opcode != OpCodes.FPRM)
				continue

			val funcName = (instr.args[0] as ArgIdentifier).value
			val name = (instr.args[1] as ArgIdentifier).value
			val type = (instr.args[2] as ArgType).value

			val map = locals.getOrPut(funcName) { LinkedHashMap() }
			if (name in map) {
				System.err.println("Warning: Duplicate local \"$name\" for function \"$funcName\"")
				continue
			}
			map[name] = type
		}

		return locals
	}


}
