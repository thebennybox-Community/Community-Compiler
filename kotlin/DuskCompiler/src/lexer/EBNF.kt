@file:JvmName("EBNF")

package lexer

import kotlin.coroutines.experimental.buildSequence

data class Tree(val raw: String, val definition: Definition?, val nodes: List<Tree>) {
    fun toPrettyString(indent: Int): String {
        var result = ""

        for((i, tree) in nodes.withIndex()) {
            result += if(i < nodes.size - 1)
                "\n╠${tree.toPrettyString(indent)}".replace("\n ", "\n║ ").replace("╠ ", "║ ")
            else
                "\n╚${tree.toPrettyString(indent)}"

        }

        return ("${definition?.name}: \"${raw.replace("\n", "\\n").asSequence().joinToString("",limit = 100)}\"")+ result.prependIndent(" ".repeat(indent - 1))
    }
}

fun wrapTree(tree: Tree) = Tree(tree.raw, null, if (tree.definition != null) listOf(tree) else tree.nodes.toList())

abstract class Expression {
    abstract fun matches(str: String): Sequence<Tree>
    abstract fun toEBNF(): String
    abstract fun toWeird(): String

    var onMatchEvent: ((str: String) -> Unit)? = null
}

class Definition(val name: String, var expression: Expression) : Expression() {
    override fun toEBNF(): String = name
    override fun toWeird(): String = name

    override fun matches(str: String): Sequence<Tree> = (onMatchEvent?.invoke(str)
            ?: Unit).let { expression.matches(str).map { Tree(it.raw, this, it.nodes.toList()) } }

    override fun toString(): String = "Definition(name = \"$name\")"
}

data class Alternation(private val expressions: List<Expression>) : Expression() {
    override fun toEBNF(): String = "(${expressions.joinToString(" | ") { it.toEBNF() }})"

    override fun toWeird(): String = "(${expressions.joinToString(" | ") { it.toWeird() }})"

    override fun matches(str: String): Sequence<Tree> = (onMatchEvent?.invoke(str)
            ?: Unit).let { expressions.asSequence().flatMap { it.matches(str) }.map(::wrapTree) }

    infix fun or(expression: Expression): Alternation = Alternation(expressions + expression)
}

infix fun Expression.or(other: Expression) = Alternation(listOf(this, other))

data class Concatenation(private val expressions: List<Expression>) : Expression() {
    override fun toEBNF(): String = "(${expressions.joinToString { it.toEBNF() }})"

    override fun toWeird(): String = "(${expressions.joinToString(" ") { it.toWeird() }})"

    override fun matches(str: String): Sequence<Tree> {
        onMatchEvent?.invoke(str)
        var result: Sequence<Tree> = expressions[0].matches(str).map(::wrapTree)

        for (expr in expressions.drop(1)) {
            result = result.flatMap { tree ->
                expr.matches(str.substring(tree.raw.length)).map { wrapTree(it).let { Tree(tree.raw + it.raw, null, tree.nodes + it.nodes) } }
            }
        }

        return result
    }

    operator fun plus(other: Expression) = Concatenation(expressions + other)
}

operator fun Expression.plus(other: Expression) = Concatenation(listOf(this, other))

data class Repetition(private val expression: Expression, private val atLeastOnce: Boolean = false, private val alwaysEverything: Boolean = false) : Expression() {
    override fun toEBNF(): String = expression.toEBNF().let { "${if (atLeastOnce) "$it, " else ""} { ${it.removeSurrounding("(", ")")} }" }

    override fun toWeird(): String = "(${expression.toWeird().removeSurrounding("(", ")")})${if (atLeastOnce) "+" else "*"}"

    private lateinit var lastResult: Sequence<Tree>

    override fun matches(str: String): Sequence<Tree> {
        onMatchEvent?.invoke(str)
        return buildSequence {
            var result: Sequence<Tree> = sequenceOf(Tree("", null, listOf()))

            if (!atLeastOnce)
                yieldAll(result)

            while (result.any()) {
                lastResult = result
                result = result.flatMap { tree ->
                    if (tree.raw.length < str.length)
                        expression.matches(str.substring(tree.raw.length)).map { wrapTree(it).let { Tree(tree.raw + it.raw, null, tree.nodes + it.nodes) } }
                    else
                        sequenceOf()
                }
                yieldAll(result)
            }
        }.let {
            if (alwaysEverything) it.lastOrNull().let { if(it == null) sequenceOf() else lastResult}
            else it.toList().asReversed().asSequence()
        }
    }
}

data class Optional(private val expression: Expression, private val enforceIfThere: Boolean = false) : Expression() {
    override fun toEBNF(): String = "[ ${expression.toEBNF().removeSurrounding("(", ")")} ]"

    override fun toWeird(): String = "(${expression.toWeird().removeSurrounding("(", ")")})?"

    override fun matches(str: String): Sequence<Tree> = (onMatchEvent?.invoke(str)
            ?: Unit).let { expression.matches(str).let {
        if(enforceIfThere )
            if (it.any())
                it
            else
                sequenceOf(Tree("", null, listOf()))
        else
            it + sequenceOf(Tree("", null, listOf()))
    } }
}

data class StringExpression(private val str: String) : Expression() {
    override fun toEBNF(): String = str.replace("\n", "\\n").let { if (it.contains("\"")) "'$it'" else "\"$it\"" }

    override fun toWeird(): String = str.replace("\n", "\\n").let { if (it.contains("\"")) "'$it'" else "\"$it\"" }

    override fun matches(str: String): Sequence<Tree> = (onMatchEvent?.invoke(str)
            ?: Unit).let { if (str.startsWith(this.str)) sequenceOf(Tree(this.str, null, listOf())) else sequenceOf() }
}

fun String.toExpr() = StringExpression(this)

data class SpecialExpression(private val name: String, private val predicate: (String) -> Sequence<Tree>) : Expression() {
    override fun toEBNF(): String = "? $name ?"

    override fun toWeird(): String = "\"$name\""

    override fun matches(str: String): Sequence<Tree> = (onMatchEvent?.invoke(str) ?: Unit).let { predicate(str) }
}

inline fun specialCharacters(name: String, crossinline predicate: (Char) -> Boolean) = SpecialExpression(name, { if (predicate(it[0])) sequenceOf(Tree(it[0].toString(), null, listOf())) else sequenceOf() })

inline fun specialString(name: String, crossinline predicate: (String) -> Int?) = SpecialExpression(name, { str ->
    predicate(str)?.let { sequenceOf(Tree(str.substring(0, it), null, listOf())) } ?: sequenceOf()
})

fun recursiveDefinition(str: String) = object : Expression() {
    private val str: String = str

    override fun matches(str: String): Sequence<Tree> = definitionMutableMap.getValue(this.str).matches(str)
    override fun toEBNF(): String = this.str
    override fun toWeird(): String = this.str
}

private val definitionMutableMap = mutableMapOf<String, Definition>()
val definitionList
    get() = definitionMutableMap.values.toList()

fun define(name: String, definition: () -> Expression) = Definition(name, definition()).apply { definitionMutableMap[this.name] = this }

fun toEBNF(definition: Definition): String = "${definition.name} = ${definition.expression.toEBNF().removeSurrounding("(", ")")};"

fun toWeird(definition: Definition): String = "${definition.name} ::= ${definition.expression.toWeird().removeSurrounding("(", ")")}"