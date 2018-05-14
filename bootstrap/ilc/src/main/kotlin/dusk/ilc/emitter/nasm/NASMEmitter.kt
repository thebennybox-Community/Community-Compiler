package dusk.ilc.emitter.nasm

import dusk.ilc.opcodes.*
import dusk.ilc.program.Function
import dusk.ilc.program.ProgramData
import dusk.ilc.util.*
import java.io.OutputStream
import kotlin.math.absoluteValue

class NASMEmitter(val underscores: Boolean) : dusk.ilc.emitter.Emitter {

	override fun emit(data: ProgramData, target: OutputStream) {
		val writer = target.bufferedWriter()

		val realInstructions = ArrayList<Instruction>()
		data.toNonScript().instructions.mapTo(realInstructions, ::escape)

		val program = ProgramData(realInstructions)
		val nasm = convert(program)

		val sectionText = NASMInstructionList()
		val sectionData = ArrayList<NASMData>()
		program.functions.values.mapTo(sectionText) {
			when (it) {
				is Function.External -> Extern(it.name)
				is Function.Internal -> Global(it.name)
			}
		}

		for (instr in nasm) {
			when (instr) {
				is NASMData -> sectionData.add(instr)
				else -> sectionText.add(instr)
			}
		}

		if (underscores)
			addUnderscores(sectionText, program)

		if(Flags.optimization)
			NASMOptimizer.optimize(sectionText)

		writer.appendln("SECTION .text")
		sectionText.forEach { writer.appendln(it.text) }

		writer.appendln().appendln("SECTION .data")
		sectionData.forEach { writer.appendln(it.text) }

		writer.close()
	}

	companion object {

		fun convert(data: ProgramData): List<NASMInstruction> {
			return data.instructions.indices.flatMap { convert(data, it) }
		}

		fun convert(data: ProgramData, index: Int): List<NASMInstruction> {
			val result = NASMInstructionList()

			val instr = data.instructions[index]
			Verbose.doIf {
				result.comment("")
				result.comment(instr.toString())
			}

			when (instr.opcode) {
				OpCodes.NOOP -> result.nop()

				OpCodes.PSTR -> {
					val name = "ro$$index"
					result.dataByteArray(name, (instr.args[0] as ArgString).value.toByteArray() + 0)
					result.mov(NASMRegister.EAX, NASMValue.Raw(name))
					result.push(NASMRegister.EAX)
				}
				OpCodes.PTRU -> result.push(1)
				OpCodes.PFLS -> result.push(0)
				OpCodes.PF64 -> {
					val name = "ro$$index"
					val value = (instr.args[0] as ArgFloat64).value

					result.dataDouble(name, value)
					result.sub(NASMRegister.ESP, NASMValue.LiteralInt(8))
					result.finit()
					result.fld(NASMValue.Raw("[$name]"))
					result.fstp(NASMRegister.ESP + 0)
				}
				is OpCodes.Push -> result.push(convert(instr.args[0]))

				OpCodes.CAST -> result.cast(typeNameMap[(instr.args[0] as ArgIdentifier).value]!!)
				OpCodes.DELE -> result.pop(NASMRegister.EAX)
				OpCodes.SWAP -> {
					result.pop(NASMRegister.EAX)
					result.pop(NASMRegister.EBX)
					result.push(NASMRegister.EAX)
					result.push(NASMRegister.EBX)
				}
				OpCodes.DUPE -> {
					result.pop(NASMRegister.EAX)
					result.push(NASMRegister.EAX)
					result.push(NASMRegister.EAX)
				}

				OpCodes.CMPE -> result.compare(index, result::je)
				OpCodes.CMPG -> result.compare(index, result::jg)
				OpCodes.CPGE -> result.compare(index, result::jge)
				OpCodes.CMPL -> result.compare(index, result::jl)
				OpCodes.CPLE -> result.compare(index, result::jge)
				OpCodes.CPNE -> result.compare(index, result::jne)

				OpCodes.FUNC -> {
					val name = (instr.args[0] as ArgIdentifier).value
					result.label(name)
					result.push(NASMRegister.EBP)
					result.mov(NASMRegister.EBP, NASMRegister.ESP)

					val func = data.functions[name]
					if (func == null)
						throw IllegalStateException("Declaration of undefined function $name")
					if (func !is Function.Internal)
						throw IllegalStateException("Declaration of external function")

					var localsSize = func.locals.values.sumBy { it.size }
					localsSize += localsSize % 16

					result.sub(NASMRegister.ESP, NASMValue.LiteralInt(localsSize.toLong()))
				}
				OpCodes.RETN -> {
					val func = data.getFunction(index)
					if (func != null && func.returnType != TypeVoid)
						result.pop(NASMRegister.EAX)
					result.leave()
					result.ret()
				}
				OpCodes.CALL -> {
					val name = (instr.args[0] as ArgIdentifier).value
					val func = data.functions[name]
							?: throw IllegalStateException("Attempt to CALL undefined function: $name")

					result.call(name)

					val paramSize = func.parameters.values.sumBy { it.size }
					result.add(NASMRegister.ESP, NASMValue.LiteralInt(paramSize.toLong()))

					if (func.returnType != TypeVoid)
						result.push(NASMRegister.EAX)
				}
				OpCodes.CALS -> {
					result.pop(NASMRegister.EAX)
					result.call(NASMRegister.EAX)
				}
				OpCodes.LABL -> {
					result.label((instr.args[0] as ArgIdentifier).value)
				}
				OpCodes.JUMP -> result.jmp((instr.args[0] as ArgIdentifier).value)
				OpCodes.JEQZ -> result.conditionalJump((instr.args[0] as ArgIdentifier).value, result::je)
				OpCodes.JNEZ -> result.conditionalJump((instr.args[0] as ArgIdentifier).value, result::jne)
				OpCodes.JGTZ -> result.conditionalJump((instr.args[0] as ArgIdentifier).value, result::jg)
				OpCodes.JGEZ -> result.conditionalJump((instr.args[0] as ArgIdentifier).value, result::jge)
				OpCodes.JLTZ -> result.conditionalJump((instr.args[0] as ArgIdentifier).value, result::jl)
				OpCodes.JLEZ -> result.conditionalJump((instr.args[0] as ArgIdentifier).value, result::jle)

				//TODO: Named locals/args
				OpCodes.LLOC -> {
					result.mov(NASMRegister.EAX, NASMRegister.EBP + localOffset(data.getFunction(index)!!, instr.args[0]))
					result.push(NASMRegister.EAX)
				}
				OpCodes.LARG -> {
					result.mov(NASMRegister.EAX, NASMRegister.EBP + argOffset(data.getFunction(index)!!, instr.args[0]))
					result.push(NASMRegister.EAX)
				}
				OpCodes.ADRL -> {
					result.lea(NASMRegister.EAX, NASMRegister.EBP + localOffset(data.getFunction(index)!!, instr.args[0]))
					result.push(NASMRegister.EAX)
				}
				OpCodes.ADRA -> {
					result.lea(NASMRegister.EAX, NASMRegister.EBP + argOffset(data.getFunction(index)!!, instr.args[0]))
					result.push(NASMRegister.EAX)
				}
				OpCodes.ADRS -> {
					result.pop(NASMRegister.EAX)
					result.lea(NASMRegister.EBX, NASMRegister.EAX.deref)
					result.push(NASMRegister.EBX)
				}
				OpCodes.SLOC -> {
					result.pop(NASMRegister.EAX)
					result.mov(NASMRegister.EBP + localOffset(data.getFunction(index)!!, instr.args[0]), NASMRegister.EAX)
				}
				OpCodes.SARG -> {
					result.pop(NASMRegister.EAX)
					result.mov(NASMRegister.EBP + argOffset(data.getFunction(index)!!, instr.args[0]), NASMRegister.EAX)
				}
				OpCodes.LGLO -> TODO()
				OpCodes.SGLO -> TODO()
				OpCodes.ADRG -> TODO()

				OpCodes.READ -> {
					result.pop(NASMRegister.EAX)
					result.mov(NASMRegister.ECX, NASMRegister.EAX + 0)
					result.push(NASMRegister.ECX)
				}
				OpCodes.WRIT -> {
					result.pop(NASMRegister.ECX)
					result.pop(NASMRegister.EAX)
					result.mov(NASMRegister.ECX + 0, NASMRegister.EAX)
				}

				OpCodes.IADD -> result.binaryOp(result::add)
				OpCodes.ISUB -> result.binaryOp(result::sub)
				OpCodes.IMUL -> result.binaryOp(result::mul)
				OpCodes.IDIV -> {
					result.pop(NASMRegister.ECX)
					result.pop(NASMRegister.EAX)
					result.mov(NASMRegister.EDX, NASMValue.LiteralInt(0))
					result.div(NASMRegister.ECX)
					result.push(NASMRegister.EAX)
				}
				OpCodes.IMOD -> {
					result.pop(NASMRegister.ECX)
					result.pop(NASMRegister.EAX)
					result.mov(NASMRegister.EDX, NASMValue.LiteralInt(0))
					result.div(NASMRegister.ECX)
					result.push(NASMRegister.EDX)
				}
				OpCodes.INEG -> {
					result.pop(NASMRegister.EAX)
					result.neg(NASMRegister.EAX)
					result.push(NASMRegister.EAX)
				}

				OpCodes.FADD -> result.floatBinaryOp(result::fadd)
				OpCodes.FSUB -> result.floatBinaryOp(result::fsub)
				OpCodes.FMUL -> result.floatBinaryOp(result::fmul)
				OpCodes.FDIV -> result.floatBinaryOp(result::fdiv)
				OpCodes.FMOD -> TODO()
				OpCodes.FNEG -> {
					result.push(NASMValue.LiteralFloat32(-1.0f))
					result.floatBinaryOp(result::fmul)
				}

				OpCodes.BSHL -> result.binaryOp(result::shl)
				OpCodes.BSHR -> result.binaryOp(result::shr)
				OpCodes.BAND -> result.binaryOp(result::and)
				OpCodes.BWOR -> result.binaryOp(result::or)
				OpCodes.BXOR -> result.binaryOp(result::xor)

				OpCodes.INFN, OpCodes.EXFN, OpCodes.FPRM, OpCodes.FLOC, OpCodes.DATA -> {}
//				OpCodes.INFN -> result.global((instr.args[0] as ArgIdentifier).value)
//				OpCodes.EXFN -> result.extern((instr.args[0] as ArgIdentifier).value)

				else -> throw UnsupportedOperationException("NASM emitter doesn't support opcode: " + instr.opcode.name)
			}

			return result
		}

		fun convert(arg: Argument<*>): NASMValue.Source {
			return when (arg) {
				is ArgUnsigned8 -> NASMValue.LiteralInt(arg.value.toLong() and 0xFF)
				is ArgUnsigned16 -> NASMValue.LiteralInt(arg.value.toLong() and 0xFFFF)
				is ArgUnsigned32 -> NASMValue.LiteralInt(arg.value.toLong() and 0xFFFFFFFF)
				is ArgUnsigned64 -> NASMValue.LiteralInt(arg.value)
				is ArgInt8 -> NASMValue.LiteralInt(arg.value.toLong())
				is ArgInt16 -> NASMValue.LiteralInt(arg.value.toLong())
				is ArgInt32 -> NASMValue.LiteralInt(arg.value.toLong())
				is ArgInt64 -> NASMValue.LiteralInt(arg.value)
				is ArgFloat32 -> NASMValue.LiteralFloat32(arg.value)
				is ArgFloat64 -> NASMValue.LiteralFloat64(arg.value)
				is ArgIdentifier -> NASMValue.Raw(arg.value)
				else -> throw UnsupportedOperationException("Convert $arg to NASM")
			}
		}

		private fun NASMInstructionList.cast(type: Type<*>) {
			if (type.size == 64 && type != TypeFloat64)
				throw UnsupportedOperationException()

			when (type) {
				TypeFloat32 -> {
					finit()
					fld(NASMRegister.ESP.deref)
					pop(NASMRegister.EAX)
					sub(NASMRegister.ESP, NASMValue.LiteralInt(4))
					fstp(NASMRegister.ESP.deref)
				}
				TypeFloat64 -> {
					finit()
					fld(NASMRegister.ESP.deref)
					pop(NASMRegister.EAX)
					sub(NASMRegister.ESP, NASMValue.LiteralInt(8))
					fstp(NASMRegister.ESP.deref)
				}
				else -> {
					val register = getCastRegister(type)
					pop(register)
					push(register)
				}
			}

		}

		private fun getCastRegister(type: Type<*>) = when (type) {
			TypeUnsigned8, TypeInt8 -> NASMRegister.AXL
			TypeUnsigned16, TypeInt16 -> NASMRegister.AX
			TypeUnsigned32, TypeInt32 -> NASMRegister.EAX
			else -> throw UnsupportedOperationException()
		}

		private inline fun NASMInstructionList.binaryOp(func: (NASMValue.Source, NASMValue.Source) -> Boolean) {
			pop(NASMRegister.ECX)
			pop(NASMRegister.EAX)
			func(NASMRegister.EAX, NASMRegister.ECX)
			push(NASMRegister.EAX)
		}

		private inline fun NASMInstructionList.floatBinaryOp(func: () -> Boolean) {
			finit()
			fld(NASMRegister.ESP.deref)
			pop(NASMRegister.EAX)
			fld(NASMRegister.ESP.deref)
			pop(NASMRegister.EAX)
			func()
			sub(NASMRegister.ESP, NASMValue.LiteralInt(4))
			fstp(NASMRegister.ESP.deref)
		}

		private inline fun NASMInstructionList.compare(index: Int, func: (String) -> Boolean) {
			pop(NASMRegister.EAX)
			pop(NASMRegister.EBX)
			cmp(NASMRegister.EAX, NASMRegister.EBX)
			val ifLabel = "dat_a_$index"
			val endIfLabel = "dat_b_$index"
			func(ifLabel)
			push(0)
			jmp(endIfLabel)
			label(ifLabel)
			push(1)
			label(endIfLabel)
		}

		private inline fun NASMInstructionList.conditionalJump(name: String, func: (String) -> Boolean) {
			pop(NASMRegister.EAX)
			cmp(NASMRegister.EAX, NASMValue.LiteralInt(0))
			func(name)
		}

		private fun localOffset(function: Function, arg: Argument<*>): Int {
			function as Function.Internal

			val name = (arg as ArgIdentifier).value
			if(name !in function.locals)
				throw IllegalStateException("Attempt to use undefined local: $name")
			val offset = function.locals.entries.takeWhile { it.key != name }.sumBy { it.value.size }

			return -(4 + offset)
		}

		private fun argOffset(function: Function, arg: Argument<*>): Int {
			val name = (arg as ArgIdentifier).value
			if(name !in function.parameters)
				throw IllegalStateException("Attempt to use undefined argument: $name")
			val offset = function.parameters.entries.takeWhile { it.key != name }.sumBy { it.value.size }

			return 8 + offset
		}

		private fun escape(text: String): String {
			if (text.isEmpty())
				return "_"
			val result = text.toCharArray()
			var changed = false
			if (!isValidStart(result[0])) {
				result[0] = '_'
				changed = true
			}
			for (i in 1 until result.size) {
				if (!isValidMiddle(result[i])) {
					result[i] = '_'
					changed = true
				}
			}
			if (!changed)
				return text

			val string = String(result)
			val unique = text.hashCode().absoluteValue.toString(36)
			return "$string$$unique"
		}

		private fun isValidStart(char: Char): Boolean {
			return char.isLetter() || char in "_"
		}

		private fun isValidMiddle(char: Char): Boolean {
			return char.isLetterOrDigit() || char in "_"
		}

		private fun escape(instruction: Instruction): Instruction {
			return Instruction(instruction.opcode, instruction.args.map(this::escape))
		}

		private fun escape(arg: Argument<*>): Argument<*> = when (arg) {
			is ArgIdentifier -> ArgIdentifier(escape(arg.value))
//			is ArgTypeArray -> ArgTypeArray(arg.value.map(this::escape).toTypedArray())
			else -> arg
		}

		private fun addUnderscores(text: NASMInstructionList, program: ProgramData) {
			text.replaceAll {
				when {
					it is Call && it.target is NASMValue.Raw -> Call(NASMValue.Raw("_${it.target.text}"))
					it is Extern -> Extern("_${it.name}")
					it is Global -> Global("_${it.name}")
					it is Push -> {
						if (it.source is NASMValue.Raw) {
							Push(NASMValue.Raw("_${it.source.text}"))
						} else {
							it
						}
					}
					it is Label && it.name in program.functions -> Label("_${it.name}")
					else -> it
				}
			}
		}
	}
}