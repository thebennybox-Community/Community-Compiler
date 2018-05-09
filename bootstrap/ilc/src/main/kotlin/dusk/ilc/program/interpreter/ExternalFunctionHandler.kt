package dusk.ilc.program.interpreter

interface ExternalFunctionHandler {
	fun supports(name: String, paramCount: Int): Boolean

	fun call(name: String, args: Array<Any>): Any?
}