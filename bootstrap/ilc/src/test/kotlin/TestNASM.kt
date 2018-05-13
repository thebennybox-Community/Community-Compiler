import dusk.ilc.emitter.nasm.NASMEmitter
import dusk.ilc.parser.TextParser
import java.io.File
import kotlin.system.measureTimeMillis

fun main(args: Array<String>) {

//	val list = InstructionList().apply {
//		addPushString("world")
//		addPushString("Hello, %s!%n")
//		addCall("_printf", TypeString, TypeUnsigned32)
//		addDelete()
//	}

	val input = ClassLoader.getSystemClassLoader().getResourceAsStream("print.il")
//	val input = File("test.fdasm").inputStream()
	val list = TextParser.parse(input)
//	val list = BinaryParser.parse(input)

//	Flags.verbose = true

//	BinaryEmitter.emit(list, File("test.dso").outputStream())

	val ms = measureTimeMillis {
		val file = File("test.asm")
		NASMEmitter(true).emit(list, file.outputStream())

		val runtime = Runtime.getRuntime()
		val execNASM = runtime.exec(arrayOf("nasm", "-f", "elf", "-o", "test.o", "test.asm"))
		val exitNASM = execNASM.waitFor()
		if (exitNASM != 0) {
			System.err.println("NASM failed")
			System.err.println(execNASM.errorStream.reader().use { it.readText() })
			return
		}

		val execGCC = runtime.exec(arrayOf(
				"gcc",
				"-m32",
				"-o", "test.exe",
				"test.o",
				"C:\\Windows\\SysWOW64\\opengl32.dll",
				"C:\\Windows\\SysWOW64\\glu32.dll",
				"glut32.dll"
		))
		val exitGCC = execGCC.waitFor()
		if (exitGCC != 0) {
			System.err.println("GCC failed")
			System.err.println(execGCC.errorStream.reader().use { it.readText() })
			return
		}

		println("Executing...")
		ProcessBuilder("test.exe").inheritIO().start().waitFor()
//		val execProgram = runtime.exec(arrayOf("test.exe"))
//		println("EXECUTING PROGRAM:")
//		val reader = execProgram.inputStream.bufferedReader()
//		while(execProgram.isAlive) {
//			println(reader.readLine())
//		}
//		reader.close()
	}
	println("Done, took ${ms}ms")
}