package dusk.ilc.program.interpreter

object StandardExternalFunctions {
	fun println(string: String) {
		System.out.println(string)
	}

	fun printf(format: String): Int {
		System.out.printf(format)
		return 0
	}

	fun printf(format: String, int: Int): Int {
		System.out.printf(format, int)
		return 0
	}

	fun printf(format: String, string: String): Int {
		System.out.printf(format, string)
		return 0
	}

	fun putchar(int: Int): Int {
		System.out.print(int.toChar())
		return 0
	}

	fun getline(): String {
		return System.`in`.bufferedReader().readLine()
	}

	fun getchar(): Int {
		return System.`in`.read()
	}
}