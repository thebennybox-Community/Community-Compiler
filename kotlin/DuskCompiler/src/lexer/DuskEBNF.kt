@file:JvmName("DuskEBNF")

package lexer

val singleLineComment = define("singleLineComment") {
    "//".toExpr() + Repetition(specialCharacters("except new line") { it != '\n' }) + "\n".toExpr()
}

val multiLineComment = define("multilineComment") {
    "/*".toExpr() + Repetition(specialString("except */") { if (it.startsWith("*/")) null else 1 }) + "*/".toExpr()
}

val comment = define("comment") {
    singleLineComment or multiLineComment
}

val whitespace = define("whitespace") {
    Repetition(specialCharacters("whitespace", Char::isWhitespace) or comment, true, true)
}

val possibleWhitespace = define("possWhitespace") { Optional(whitespace, true) }

val booleanLiteral = define("booleanLiteral") {
    "true".toExpr() or "false".toExpr()
}

val numberType = define("numberType") {
    ("i".toExpr() or "u".toExpr() or "f".toExpr()) + ("8".toExpr() or "16".toExpr() or "32".toExpr() or "64".toExpr())
}

val digit = specialCharacters("digit", Char::isDigit)

val numberLiteral = define("numberLiteral") {
    Definition("number", Repetition(digit, true, true) + Optional(".".toExpr() + Repetition(digit))) + Optional(numberType)
}

val stringLiteral = define("stringLiteral") {
    "\"".toExpr() + Repetition(specialString("except unescaped quotation marks") {
        when {
            it.startsWith("\\\"") -> 2
            it.startsWith("\"") -> null
            else -> 1
        }
    }, alwaysEverything = true) + "\"".toExpr()
}

val literal = define("literal") {
    booleanLiteral or numberLiteral or stringLiteral
}

val decKeyword = define("decKeyword") {
    "var".toExpr() or "let".toExpr()
}

const val returnKeyword = "return"
const val infixKeyword = "infix"
const val prefixKeyword = "prefix"
const val suffixKeyword = "suffix"
const val ifKeyword = "if"
const val functionKeyword = "fn"
const val elseKeyword = "else"
const val operatorKeyword = "op"
const val loopKeyword = "loop"
const val inKeyword = "in"

val affixKeywords = define("affixKeywords") {
    infixKeyword.toExpr() or prefixKeyword.toExpr() or suffixKeyword.toExpr()
}/*.apply { onMatchEvent = { println(it) } }*/

val keywords = define("keywords") {
    (returnKeyword.toExpr() or "continue".toExpr() or "break".toExpr() or functionKeyword.toExpr() or operatorKeyword.toExpr() or decKeyword or affixKeywords or ifKeyword.toExpr() or loopKeyword.toExpr() or elseKeyword.toExpr() or loopKeyword.toExpr() or inKeyword.toExpr()) + whitespace
}

val identifier = define("identifier") {
    (specialCharacters("identifier start") { it.isJavaIdentifierStart() && !it.isWhitespace() } + Repetition(specialCharacters("identifier part") { it.isJavaIdentifierPart() && !it.isWhitespace() }, alwaysEverything = true)).let {
        SpecialExpression("identifiers except keywords") { str ->
            if (keywords.matches(str).any())
                sequenceOf()
            else
                it.matches(str)
        }
    }
}

val arrayIdentifier = define("arrayIdentifier") {
    "[".toExpr() + Optional(numberLiteral) + "]".toExpr()
}

val genericSymbol = define("genericSymbol") {
    "<".toExpr() + possibleWhitespace + recursiveDefinition("symbol") + possibleWhitespace + Repetition(",".toExpr() + recursiveDefinition("symbol") + possibleWhitespace) + ">".toExpr()
}

val symbol = define("symbol") {
    identifier + Optional(possibleWhitespace + genericSymbol) + Repetition(possibleWhitespace + arrayIdentifier)
}

val arrayCreation = define("arrayCreation") {
    "[".toExpr() + possibleWhitespace + Optional(recursiveDefinition("expression") + possibleWhitespace + Repetition(",".toExpr() + possibleWhitespace + recursiveDefinition("expression") + possibleWhitespace)) + "]".toExpr()
}

val operator = define("operator") {
    Repetition(specialCharacters("operatorChar") {
        "+*-/=<>!%?".toCharArray().contains(it)
    }, true, true)
}

val infixFunction = define("infixFunction") {
    (recursiveDefinition("affixLessExpression") + whitespace + symbol + whitespace + recursiveDefinition("expression")) or (recursiveDefinition("affixLessExpression") + possibleWhitespace + operator + possibleWhitespace + recursiveDefinition("expression"))
}

val prefixFunction = define("prefixFunction") {
    (symbol + whitespace + recursiveDefinition("expression")) or (operator + possibleWhitespace + recursiveDefinition("expression"))
}

val suffixFunction = define("suffixFunction") {
    (recursiveDefinition("affixLessExpression") + whitespace + symbol) or (recursiveDefinition("affixLessExpression") + possibleWhitespace + operator)
}

val invocation = define("invocation") {
    symbol + possibleWhitespace + "(".toExpr() + Optional(possibleWhitespace + recursiveDefinition("expression") + Repetition(possibleWhitespace + ",".toExpr() + possibleWhitespace + recursiveDefinition("expression"), alwaysEverything = true)) + possibleWhitespace + ")".toExpr()
}

fun ifFun(semicolon: Boolean, name: String) = define(name) {
    ifKeyword.toExpr() + possibleWhitespace + "(".toExpr() + recursiveDefinition("expression") + ")".toExpr() + possibleWhitespace + (recursiveDefinition("codeBlock") or (recursiveDefinition("expression") + (if (semicolon) possibleWhitespace + ";".toExpr() else possibleWhitespace))) +
            elseKeyword.toExpr() + possibleWhitespace + (recursiveDefinition("codeBlock") or (recursiveDefinition("expression") + (if (semicolon) possibleWhitespace + ";".toExpr() else possibleWhitespace))).let { if (semicolon) Optional(it) else it }

}

val ifExpression = ifFun(false, "ifExpression")

val ifStatement = ifFun(true, "ifStatement")

val affixLessExpression = define("affixLessExpression") {
    invocation or literal or symbol or arrayCreation or ifExpression or recursiveDefinition("codeBlock") or
            ("(".toExpr() + possibleWhitespace + recursiveDefinition("expression") + possibleWhitespace + ")".toExpr())
}

val expression = define("expression") {
    affixLessExpression or infixFunction or suffixFunction or prefixFunction
}

val explicitReturnStatement = define("explicitReturnStatement") {
    (returnKeyword.toExpr() + whitespace + expression + possibleWhitespace + ";".toExpr())
}

val returnStatement = define("returnStatement") {
    explicitReturnStatement or expression
}

val decStatement = define("decStatement") {
    decKeyword + whitespace + symbol + Optional(possibleWhitespace + ":".toExpr() + possibleWhitespace + symbol) + Optional(possibleWhitespace + "=".toExpr() + possibleWhitespace + expression)
}

val attribute = define("attribute") {
    "@".toExpr() + symbol
}

val normalFunction = define("normalFunction") {
    functionKeyword.toExpr() + whitespace + symbol
}

val operatorFunction = define("operatorFunction") {
    operatorKeyword.toExpr() + whitespace + operator
}

val functionStatement = define("functionStatement") {
    Repetition(attribute + whitespace, alwaysEverything = true) + Optional(affixKeywords + whitespace) + (normalFunction or operatorFunction) + possibleWhitespace + "(".toExpr() + possibleWhitespace + Optional(symbol + possibleWhitespace + ":".toExpr() + possibleWhitespace + symbol + possibleWhitespace + Repetition(",".toExpr() + possibleWhitespace + symbol + possibleWhitespace + ":".toExpr() + possibleWhitespace + symbol + possibleWhitespace)) + ")".toExpr() + possibleWhitespace + Optional(":".toExpr() + possibleWhitespace + symbol + possibleWhitespace) + recursiveDefinition("codeBlock")
}

val loopStatement = define("loopStatement") {
    loopKeyword.toExpr() + possibleWhitespace + "(".toExpr() + possibleWhitespace + (expression or (symbol + possibleWhitespace + inKeyword.toExpr() + possibleWhitespace + expression)) + possibleWhitespace + ")".toExpr() + possibleWhitespace + (recursiveDefinition("codeBlock") or (recursiveDefinition("expression") + possibleWhitespace + ";".toExpr()))
}

val operation = ((decStatement or explicitReturnStatement or expression) + possibleWhitespace + ";".toExpr()) or functionStatement or ifStatement or loopStatement

val codeBlock = define("codeBlock") {
    "{".toExpr() + possibleWhitespace + Repetition(operation + possibleWhitespace) + Optional(returnStatement + possibleWhitespace) + "}".toExpr()
}

val file = define("file") {
    possibleWhitespace + Repetition(operation + possibleWhitespace, alwaysEverything = true)
}
