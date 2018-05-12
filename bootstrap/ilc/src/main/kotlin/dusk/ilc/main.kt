package dusk.ilc

import dusk.ilc.emitter.*
import dusk.ilc.emitter.nasm.NASMEmitter
import dusk.ilc.parser.*
import dusk.ilc.program.Optimizer
import dusk.ilc.program.interpreter.*
import dusk.ilc.util.Flags
import dusk.ilc.util.Verbose
import java.io.File

enum class EmitterType(val create: () -> Emitter) {
	NASM({ NASMEmitter(false) }),
	WNASM({ NASMEmitter(true) }),
	BIN(BinaryEmitter),
	TEXT(TextEmitter),
	EXE({ ExecutableEmitter(false) }),
	WEXE({ ExecutableEmitter(true) });

	constructor(emitter: Emitter) : this({ emitter })
}

enum class ParserType(val create: () -> Parser) {
	BIN(BinaryParser),
	TEXT(TextParser);

	constructor(parser: Parser) : this({ parser })
}

fun main(args: Array<String>) {
	var outputFile: File? = null
	var inputFile: File? = null
	var emitterType: EmitterType? = null
	var parserType: ParserType? = null

	var interpret = false
	// duskilc -o test.exe -e binary -p textV2 ogl.il

	var i = 0
	while (i < args.size) {
		val str = args[i++]
		if (str == "-o") {
			val result = StringBuilder()
			if (i >= args.size) {
				System.err.println("Expected file path")
				return
			}

			if (args[i].startsWith("\"")) {
				while (i < args.size && !args[i].endsWith("\""))
					result.append(args[i++])
			} else {
				result.append(args[i++])
			}

			outputFile = File(result.toString())
		} else if (str == "-e") {
			val name = args[i++].toUpperCase()
			emitterType = EmitterType.values().find { it.name == name }

			if (emitterType == null) {
				System.err.println("Unrecognized emitter: $name")
				System.err.println("Try one of these:")
				EmitterType.values().forEach { println("\t$it") }
				return
			}
		} else if (str == "-p") {
			val name = args[i++].toUpperCase()
			parserType = ParserType.values().find { it.name == name }

			if (parserType == null) {
				System.err.println("Unrecognized parser: $name")
				System.err.println("Try one of these:")
				ParserType.values().forEach { println("\t$it") }
				return
			}
		} else if (str == "-v") {
			Flags.verbose = true
		} else if (str == "--no-optimization") {
			Flags.optimization = false
		} else if(str == "-i") {
			interpret = true
		} else if(inputFile == null) {
			val result = StringBuilder()
			i--

			if (str.startsWith("\"")) {
				while (i < args.size && !args[i].endsWith("\""))
					result.append(args[i++])
			} else {
				result.append(args[i++])
			}

			inputFile = File(result.toString())
		} else {
			System.err.println("Unexpected \"$str\"")
			return
		}
	}



	if(parserType == null) {
		System.err.println("Expected parser, use -p to specify it.")
		return
	}

	if(inputFile == null) {
		System.err.println("Expected input file")
		return
	}

	if(!inputFile.exists()) {
		System.err.println("Input file \"${inputFile.path}\" does not exist")
		return
	}

	if(interpret) {
		val parser = parserType.create()
		executeInterpreter(inputFile, parser)
		return
	}

	if(emitterType == null) {
		System.err.println("Expected emitter, use -e to specify it.")
		return
	}

	if(outputFile == null) {
		System.err.println("Expected output file, use -o to specify it.")
		return
	}

	val parser = parserType.create()
	val emitter = emitterType.create()
	execute(inputFile, outputFile, emitter, parser)
}

private fun execute(inp: File, out: File, emitter: Emitter, parser: Parser) {
	var instructions = parser.parse(inp.inputStream())
	if(Flags.optimization)
		instructions = Optimizer.optimize(instructions)
	emitter.emit(instructions, out.outputStream())
}

private fun executeInterpreter(inp: File, parser: Parser) {
//	println("### Interpreting ${inp.path}")
	var instructions = parser.parse(inp.inputStream()).toNonScript()
	if(Flags.optimization)
		instructions = Optimizer.optimize(instructions)
	val interpreter = Interpreter(instructions)
	interpreter.addExternalFunctionHandler(ReflectionFunctions(StandardExternalFunctions))
	interpreter.execute()
//	println("### Interpreter done")
}