package dusk.ilc.emitter

import dusk.ilc.emitter.nasm.NASMEmitter
import dusk.ilc.opcodes.Instruction
import dusk.ilc.program.ProgramData
import java.io.File
import java.io.OutputStream
import java.nio.file.Files

class ExecutableEmitter(val underscores: Boolean): Emitter {

	override fun emit(data: ProgramData, target: OutputStream) {
//		val nasmFile = File("dusk", ".asm")
//		val oFile = File.createTempFile("dusk", ".o")
//		val exeFile = File.createTempFile("dusk", ".exe")
		val nasmFile = File(".nasm.tmp")
		val oFile = File(".o.tmp")
		val exeFile = File(".exe.tmp")

		nasmFile.deleteOnExit()
		oFile.deleteOnExit()
		exeFile.deleteOnExit()
		NASMEmitter(underscores).emit(data, nasmFile.outputStream())

		val runtime = Runtime.getRuntime()
		val execNASM = runtime.exec(arrayOf("nasm", "-f", "elf", "-o", "\"${oFile.absolutePath}\"", "\"${nasmFile.absolutePath}\""))
		val exitNASM = execNASM.waitFor()
		if (exitNASM != 0) {
			System.err.println("NASM failed")
			System.err.println(execNASM.errorStream.reader().use { it.readText() })
			target.close()
			return
		}

		val execGCC = runtime.exec(arrayOf("gcc", "-o", "\"${exeFile.absolutePath}\"", "\"${oFile.absolutePath}\""))
		val exitGCC = execGCC.waitFor()
		if (exitGCC != 0) {
			System.err.println("GCC failed")
			System.err.println(execGCC.errorStream.reader().use { it.readText() })
			target.close()
			return
		}

		Files.copy(exeFile.toPath(), target)
	}
}