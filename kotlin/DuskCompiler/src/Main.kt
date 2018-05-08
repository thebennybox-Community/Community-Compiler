import lexer.*
import java.nio.file.Files
import java.nio.file.Paths
import kotlin.streams.asSequence

fun main(args: Array<String>) {
    definitionList.forEach { println(it.toEBNF()) }

    val content = Files.lines(Paths.get(args[0])).asSequence().joinToString("\n")
    val lexed = file.matches(content)

    println(lexed.toList().map(::removeWhitespace).joinToString("\n${"=".repeat(100)}\n") { it.toPrettyString(2) })
}

fun removeWhitespace(tree: Tree): Tree {
    return Tree(tree.raw, tree.definition, tree.nodes.filter { it.definition != whitespace && it.definition != possibleWhitespace }.map(::removeWhitespace))
}