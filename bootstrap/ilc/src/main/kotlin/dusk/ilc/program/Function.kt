package dusk.ilc.program

import dusk.ilc.opcodes.Type

sealed class Function(
		val name: String,
		val returnType: Type<*>,
		val parameters: Variables
) {
	class External(name: String, returnType: Type<*>, params: Variables): Function(name,  returnType, params)

	class Internal(
			name: String,
			returnType: Type<*>,
			params: Variables,
			val locals: Variables
	): Function(name,  returnType, params)
}
