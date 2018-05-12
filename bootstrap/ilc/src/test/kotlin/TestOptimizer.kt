import dusk.ilc.emitter.TextEmitter
import dusk.ilc.parser.TextParser
import dusk.ilc.program.Optimizer
import dusk.ilc.util.OutputStreamWrapper

fun main(args: Array<String>) {
	val input = ClassLoader.getSystemClassLoader().getResourceAsStream("optimize.il")
	val program = TextParser.parse(input)

	val optimized = Optimizer.optimize(program)
	TextEmitter.emit(optimized, OutputStreamWrapper(System.out))
	println("-".repeat(10))
	println("Previous size: ${program.instructions.size}")
	println("Optimized size: ${optimized.instructions.size}")
}