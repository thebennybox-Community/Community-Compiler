package dusk.ilc.program

import dusk.ilc.opcodes.Type

/**
 * Represents a function, external or internal
 *
 * @see External
 * @see Internal
 */
sealed class Function(
		val name: String,
		val returnType: Type<*>,
		val parameters: Variables
) {

	/**
	 * Represents an external function
	 */
	class External(name: String, returnType: Type<*>, params: Variables): Function(name,  returnType, params)

	/**
	 * Represents an internal function
	 */
	class Internal(
			name: String,
			returnType: Type<*>,
			params: Variables,
			val locals: Variables
	): Function(name,  returnType, params)
}
