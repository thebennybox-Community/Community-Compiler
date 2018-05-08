import dusk.ilc.emitter.BinaryEmitter
import dusk.ilc.parser.TextParser
import java.io.File

fun main(args: Array<String>) {
	val input = ClassLoader.getSystemClassLoader().getResourceAsStream("example.tdso")
	val parsed = TextParser.parse(input)

	BinaryEmitter.emit(parsed, File("example.dso").outputStream())
}