package dusk.ilc.program.interpreter

class ReflectionFunctions(val obj: Any): ExternalFunctionHandler {
	val clazz = obj::class.java

	override fun supports(name: String, paramCount: Int): Boolean {
		return clazz.methods.any { it.name == name && it.parameterCount == paramCount }
	}

	override fun call(name: String, args: Array<Any>): Any? {
		val method = clazz.methods.find { it.name == name && it.parameterCount == args.size }!!
//		println("Given:")
//		args.forEach { println(it::class.java.name) }
//		println("Required:")
//		method.parameterTypes.forEach { println(it.name) }
		return method.invoke(obj, *args)
	}

}