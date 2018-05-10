package dusk.ilc.program.interpreter

import dusk.ilc.opcodes.*
import dusk.ilc.program.Function
import dusk.ilc.program.ProgramData
import dusk.ilc.util.Verbose
import java.util.Stack

class Interpreter(val program: ProgramData) {

	private val funcHandlers = ArrayList<ExternalFunctionHandler>()

	private val scopeStack = Stack<Scope>()

	var index: Int
		get() = scopeStack.peek().index
		private set(value) {
			scopeStack.peek().index = value
		}

	val nextInstruction: Instruction?
		get() = if(hasRemaining) program[index] else null

	val hasRemaining: Boolean
		get() = scopeStack.isNotEmpty() && index in program.instructions.indices

	val currentScope: Scope?
		get() = if(scopeStack.isEmpty()) null else scopeStack.peek()

	init {
		val func = getFunction("main")
		if(func != null)
			callFunction(func)
	}

	fun addExternalFunctionHandler(handler: ExternalFunctionHandler) {
		funcHandlers.add(handler)
	}

	fun execute() {
		while(hasRemaining)
			step()
	}

	private fun push(value: Any) {
		scopeStack.peek().stack.push(value)
	}

	private fun pop(): Any {
		return scopeStack.peek().stack.pop()
	}

	fun step() {
		if(!hasRemaining)
			return

		val instr = program[index++]
		Verbose.println("### $instr")
//		println("Scope: ${scopeStack.size}")
//		println("Executing (${index-1}) $instr")

		when(instr.opcode) {
			OpCodes.NOOP -> {}
			OpCodes.PTRU -> push(1.toByte())
			OpCodes.PFLS -> push(0.toByte())
			OpCodes.PFUN -> {
				push(program.functions[(instr.args[0] as ArgIdentifier).value]!!)
			}
			is OpCodes.Push -> push(instr.args[0].value!!)
			OpCodes.CAST -> TODO()
			OpCodes.SWAP -> {
				val a = pop()
				val b = pop()
				push(a)
				push(b)
			}
			OpCodes.DUPE -> {
				val v = pop()
				push(v)
				push(v)
			}
			OpCodes.DELE -> pop()

			OpCodes.FUNC -> {
				val function = getFunction((instr.args[0] as ArgIdentifier).value)
				while(index < program.instructions.size && program.getFunction(index) == function)
					index++
			}
			OpCodes.RETN -> {
				val scope = scopeStack.pop()
				if(scope.returnType != TypeVoid)
					push(scope.stack.pop())
			}
			OpCodes.CALL -> {
				val functionName = (instr.args[0] as ArgIdentifier).value
				val func = (getFunction(functionName)
						?: throw IllegalStateException("Attempt to call undefined function $functionName"))
				callFunction(func)
			}
			OpCodes.CALS -> callFunction(pop() as Function)

			OpCodes.CMPE -> compare { a, b -> a == b }
			OpCodes.CMPG -> compare { a, b -> a > b }
			OpCodes.CPGE -> compare { a, b -> a >= b }
			OpCodes.CMPL -> compare { a, b -> a < b }
			OpCodes.CPLE -> compare { a, b -> a <= b }
			OpCodes.CPNE -> compare { a, b -> a != b }

			OpCodes.LABL -> {}
			OpCodes.JUMP -> jump((instr.args[0] as ArgIdentifier).value)
			OpCodes.JEQZ -> jumpConditional(instr.args[0]) { it == 0.0 }
			OpCodes.JNEZ -> jumpConditional(instr.args[0]) { it != 0.0 }
			OpCodes.JGTZ -> jumpConditional(instr.args[0]) { it > 0.0 }
			OpCodes.JGEZ -> jumpConditional(instr.args[0]) { it >= 0.0 }
			OpCodes.JLTZ -> jumpConditional(instr.args[0]) { it < 0.0 }
			OpCodes.JLEZ -> jumpConditional(instr.args[0]) { it <= 0.0 }

			OpCodes.LLOC -> {
				push(scopeStack.peek().locals[(instr.args[0] as ArgIdentifier).value]!!)
			}
			OpCodes.LARG -> {
				push(scopeStack.peek().args[(instr.args[0] as ArgIdentifier).value]!!)
			}
			OpCodes.LGLO -> TODO()
			OpCodes.ADRL -> TODO()
			OpCodes.ADRA -> TODO()
			OpCodes.ADRG -> TODO()
			OpCodes.SLOC -> {
				scopeStack.peek().locals[(instr.args[0] as ArgIdentifier).value] = pop()
			}
			OpCodes.SARG -> {
				scopeStack.peek().args[(instr.args[0] as ArgIdentifier).value] = pop()
			}
			OpCodes.SGLO -> TODO()

			OpCodes.READ -> TODO()
			OpCodes.WRIT -> TODO()

			OpCodes.IADD -> binaryIntegerOp { a, b -> a + b }
			OpCodes.ISUB -> binaryIntegerOp { a, b -> a - b }
			OpCodes.IMUL -> binaryIntegerOp { a, b -> a * b }
			OpCodes.IDIV -> binaryIntegerOp { a, b -> a / b }
			OpCodes.IMOD -> binaryIntegerOp { a, b -> a % b }
			OpCodes.INEG -> TODO()

			OpCodes.FADD -> binaryFloatOp { a, b -> a + b }
			OpCodes.FSUB -> binaryFloatOp { a, b -> a - b }
			OpCodes.FMUL -> binaryFloatOp { a, b -> a * b }
			OpCodes.FDIV -> binaryFloatOp { a, b -> a / b }
			OpCodes.FMOD -> binaryFloatOp { a, b -> a % b }
			OpCodes.FNEG -> TODO()

			OpCodes.BSHL -> binaryIntegerOp { a, b -> a shl b.toInt() }
			OpCodes.BSHR -> binaryIntegerOp { a, b -> a shr b.toInt() }
			OpCodes.BAND -> binaryIntegerOp { a, b -> a and b }
			OpCodes.BWOR -> binaryIntegerOp { a, b -> a or b }
			OpCodes.BXOR -> binaryIntegerOp { a, b -> a xor  b }

			OpCodes.INFN, OpCodes.EXFN, OpCodes.FPRM, OpCodes.FLOC, OpCodes.DATA -> {}

			else -> throw UnsupportedOperationException(instr.toString())
		}
	}

	private inline fun jumpConditional(target: Argument<*>, body: (Double) -> Boolean) {
		val name = (target as ArgIdentifier).value
		val value = (pop() as Number).toDouble()
		if(body(value))
			jump(name)
	}

	private inline fun compare(body: (Double, Double) -> Boolean) {
		val original = pop() as Number
		val a = (original).toDouble()
		val b = (pop() as Number).toDouble()

		if(body(b, a))
			push(1.toByte())
		else
			push(0.toByte())
	}

	private inline fun binaryIntegerOp(body: (Long, Long) -> Long) {
		val original = pop() as Number
		val a = (original).toLong()
		val b = (pop() as Number).toLong()

		val result = body(b, a)
		when(original) {
			is Byte -> push(result.toByte())
			is Short -> push(result.toShort())
			is Int -> push(result.toInt())
			is Long -> push(result)
			else -> push(result.toInt())
		}
	}

	private inline fun binaryFloatOp(body: (Double, Double) -> Double) {
		val original = pop() as Number
		val a = (original).toDouble()
		val b = (pop() as Number).toDouble()

		val result = body(b, a)

		when(original) {
			is Float -> push(result.toFloat())
			is Double -> push(result)
			else -> push(result.toFloat())
		}
	}

	private fun getFunction(name: String): Function? {
		return program.functions[name]
	}

	private fun callFunction(func: Function) {
		if(func is Function.External) {
			callExternalFunction(func)
			return
		}

		val index = program.instructions.indexOfFirst {
			it.opcode == OpCodes.FUNC && it.args[0].value == func.name
		}

		val scope = Scope(func.returnType)
		for((k, _) in func.parameters) {
			scope.args[k] = pop()
		}
		scopeStack.push(scope)

		this.index = index + 1
	}

	private fun callExternalFunction(func: Function) {
		val args = ArrayList<Any>()
		for (parameter in func.parameters) {
			args.add(pop())
		}

		val handler = funcHandlers.firstOrNull { it.supports(func.name, args.size) }
		if(handler == null)
			throw IllegalStateException("No external function matching signature: ${func.name} $args")
		val returned = handler.call(func.name, args.toArray())
		if(func.returnType != TypeVoid) {
			push(returned ?: 0)
		}
	}

	private fun jump(name: String) {
		val index = program.instructions.indexOfFirst {
			it.opcode == OpCodes.LABL && it.args[0].value == name
		}

		if(index == -1)
			throw IllegalStateException("Attempt to jump to undefined label: $name")

		this.index = index
	}

	class Scope(val returnType: Type<*>) {
		val stack = Stack<Any>()
		val locals: MutableMap<String, Any> = HashMap()
		val args: MutableMap<String, Any> = HashMap()
		var index = 0
	}
}