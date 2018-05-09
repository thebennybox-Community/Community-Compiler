package dusk.ilc.opcodes

abstract class OpCode(
		val id: Int,
		vararg val argumentTypes: Type<*>
) {
	val name = javaClass.simpleName.replace('_', ' ')
}