import dusk.ilc.parser.BinaryParser
import dusk.ilc.parser.TextParser
import dusk.ilc.program.interpreter.Interpreter
import dusk.ilc.program.interpreter.ReflectionFunctions

fun main(args: Array<String>) {
	val input = ClassLoader.getSystemClassLoader().getResourceAsStream("test.fil")
	val program = BinaryParser.parse(input).toNonScript()
	val interpreter = Interpreter(program)
	interpreter.addExternalFunctionHandler(ReflectionFunctions(ExternalTest))

	interpreter.execute()
}

object ExternalTest {
	fun println(string: String) {
		System.out.println(string)
	}

	fun printlnInt(int: Int) {
		System.out.println(int)
	}

	fun printf(format: String) {
		System.out.printf(format)
	}

	fun printf(format: String, int: Int) {
		System.out.printf(format, int)
	}

	fun putchar(int: Int): Int {
		System.out.print(int.toChar())
		return 0
	}
}