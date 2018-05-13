import dusk.ilc.parser.TextParser
import dusk.ilc.program.Optimizer
import dusk.ilc.program.interpreter.*
import dusk.ilc.util.Flags

fun main(args: Array<String>) {
//	println("### Interpreting ${inp.path}")
	val input = ClassLoader.getSystemClassLoader().getResourceAsStream("input.il")
	var instructions = TextParser.parse(input).toNonScript()
	if (Flags.optimization)
		instructions = Optimizer.optimize(instructions)
	val interpreter = Interpreter(instructions)
	interpreter.addExternalFunctionHandler(ReflectionFunctions(StandardExternalFunctions))
	interpreter.execute()
//	println("### Interpreter done")

//	val input = ClassLoader.getSystemClassLoader().getResourceAsStream("print.il")
//	val program = TextParser.parse(input).toNonScript()
////	val program = ProgramData(emptyList())
//	val interpreter = Interpreter(program)
//	interpreter.addExternalFunctionHandler(ReflectionFunctions(ExternalTest))
//	interpreter.execute()

//	interpreter.execute()
}

//object ExternalTest {
//	fun println(string: String) {
//		System.out.println(string)
//	}
//
//	fun printlnInt(int: Int) {
//		System.out.println(int)
//	}
//
//	fun printf(format: String) {
//		System.out.printf(format)
//	}
//
//	fun printf(format: String, int: Int) {
//		System.out.printf(format, int)
//	}
//
//	fun putchar(int: Int): Int {
//		System.out.print(int.toChar())
//		return 0
//	}
//
//	fun assert(value: Number) {
//		if (value.toLong() == 0L)
//			System.err.println("Test failed")
//	}
//}