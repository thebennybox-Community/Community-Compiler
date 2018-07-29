package dusk.compiler

class Lexer {
    fun lex(text: String): MutableList<Token> {
        var i = 0      // Offset from start of text
        var line = 1   // Current line number
        var column = 1 // Current column number

        val result: MutableList<Token> = mutableListOf()

        lexer@ while(i < text.length) {
            val token = Token(TokenKind.None, line, column, "")

            when {
                text[i] == '\r' -> {
                    // Could be looking at \r or \r\n
                    // Consume the \r as a new line then check/ignore a \n

                    line++
                    column = 1
                    i++

                    if(text[i] == '\n') {
                        i++
                    }

                    continue@lexer
                }

                text[i] == '\n' -> {
                    line++
                    column = 1
                    i++

                    continue@lexer
                }

                text[i].isWhitespace() -> {
                    while(text[i].isWhitespace()) {
                        i++
                        column++
                    }

                    continue@lexer
                }

                isIdentStart(text[i]) -> {
                    val start = i

                    while(isIdent(text[i])) {
                        i++
                        column++
                    }

                    token.text = text.substring(start, i)

                    token.kind = when(token.text) {
                        "break" -> TokenKind.KeywordBreak
                        "continue" -> TokenKind.KeywordContinue
                        "else" -> TokenKind.KeywordElse
                        "extern" -> TokenKind.KeywordExtern
                        "false" -> TokenKind.KeywordFalse
                        "fn" -> TokenKind.KeywordFn
                        "if" -> TokenKind.KeywordIf
                        "in" -> TokenKind.KeywordIn
                        "impl" -> TokenKind.KeywordImpl
                        "import" -> TokenKind.KeywordImport
                        "infix" -> TokenKind.KeywordInfix
                        "let" -> TokenKind.KeywordLet
                        "loop" -> TokenKind.KeywordLoop
                        "prefix" -> TokenKind.KeywordPrefix
                        "return" -> TokenKind.KeywordReturn
                        "struct" -> TokenKind.KeywordStruct
                        "suffix" -> TokenKind.KeywordSuffix
                        "true" -> TokenKind.KeywordTrue
                        "var" -> TokenKind.KeywordVar
                        else -> TokenKind.Identifier
                    }
                }

                text[i].isDigit() -> {
                    val start = i
                    var isFloat = false

                    // Check for a hex number starting with 0x
                    if(text[i] == '0' && text[i + 1] == 'x') {
                        // Skip 0x
                        i += 2
                        column += 2

                        while(isHexDigit(text[i]) || text[i] == '_') {
                            i++
                            column++
                        }
                    }
                    // Check for a binary number starting with 0b
                    else if(text[i] == '0' && text[i + 1] == 'b') {
                        // Skip 0b
                        i += 2
                        column += 2

                        while(isBinaryDigit(text[i]) || text[i] == '_') {
                            i++
                            column++
                        }
                    }
                    // Default to base 10
                    else {
                        while(text[i].isDigit() || text[i] == '_') {
                            i++
                            column++
                        }

                        // Take fractional part (. followed by digits)
                        if(text[i] == '.' && text[i + 1].isDigit()) {
                            isFloat = true

                            // Skip . and first digit
                            i += 2
                            column += 2

                            while(text[i].isDigit() || text[i] == '_') {
                                i++
                                column++
                            }
                        }
                    }

                    // Keep the end of the number literal in case the suffix
                    // doesn't work out
                    val end = i
                    val endColumn = column

                    // We reject invalid suffixes here rather than in the
                    // parser. u and i can be followed by 8, 16, 32 or 64,
                    // f can be followed by 32 or 64

                    if(text[i] in "uif") {
                        val suffixType = text[i]

                        i++
                        column++

                        // Got a valid letter, consume a number of bits

                        val suffixBitsStart = i

                        while(text[i].isDigit()) {
                            i++
                            column++
                        }

                        val suffixBits = text.substring(suffixBitsStart, i)

                        // Reset if the suffix wasn't valid
                        // Also disallow a u/i suffix on floats
                        if(!(
                             !isIdent(text[i]) &&
                             ((suffixType in "ui" && !isFloat &&
                               suffixBits in arrayOf("8", "16", "32", "64")) ||
                              (suffixType == 'f' &&
                               suffixBits in arrayOf("32", "64"))))) {
                            i = end
                            column = endColumn
                        }
                    }

                    token.text = text.substring(start, i)
                    token.kind = TokenKind.Number
                }

                // Single line comments starting with //
                text[i] == '/' && text[i + 1] == '/' -> {
                    // We don't need to adjust column here as it'll be reset at
                    // the new line

                    // Skip //
                    i += 2

                    while(text[i] != '\r' && text[i] != '\n') {
                        i++
                    }

                    continue@lexer
                }

                // Multiline comments delimited by /* and */
                text[i] == '/' && text[i + 1] == '*' -> {
                    // Skip opening /*
                    i += 2
                    column += 2

                    while(!(text[i] == '*' && text[i + 1] == '/') && i < text.length) {
                        if(text[i] == '\r') {
                            i++
                            line++
                            column = 1

                            if(text[i] == '\n') {
                                i++
                            }
                        } else if(text[i] == '\n') {
                            i++
                            line++
                            column = 1
                        } else {
                            i++
                            column++
                        }
                    }

                    // Skip closing */
                    i += 2
                    column += 2

                    continue@lexer
                }

                isOperator(text[i]) -> {
                    val start = i

                    while(isOperator(text[i])) {
                        i++
                        column++
                    }

                    token.text = text.substring(start, i)

                    token.kind = when(token.text) {
                        ":" -> TokenKind.Colon
                        "@" -> TokenKind.At
                        "..." -> TokenKind.Vararg
                        else -> TokenKind.Operator
                    }
                }

                text[i] == '\"' || text[i] == '\'' -> {
                    val quote = text[i]

                    // Skip opening quote
                    i++
                    column++

                    // We didn't put this first because we don't need the quote
                    val start = i

                    while(text[i] != quote) {
                        if(text[i] == '\\') {
                            i++
                            column++
                        }

                        i++
                        column++
                    }

                    token.text = text.substring(start, i)

                    token.kind = when(quote) {
                        '\"' -> TokenKind.String
                        '\'' -> TokenKind.Char
                        else -> TokenKind.None
                    }

                    token.text = token.text.replace(
                        Regex(
                            """\\u[\dA-Fa-f]{4}|""" +
                            """\\U[\dA-Fa-f]{8}|""" +
                            """\\[abfrntv\\\"\']"""
                        )
                    ) { match ->
                        val value = match.value
                        val sb = StringBuilder()

                        when(value[1]) {
                            'u', 'U' -> {
                                val codepoint = value.substring(2).toInt(16)
                                sb.appendCodePoint(codepoint)
                            }
                            'a' -> sb.append(7.toChar())
                            'b' -> sb.append('\b')
                            'f' -> sb.append(12.toChar())
                            'n' -> sb.append('\n')
                            'r' -> sb.append('\r')
                            't' -> sb.append('\t')
                            'v' -> sb.append(11.toChar())
                            '\\' -> sb.append('\\')
                            '\"' -> sb.append('\"')
                            '\'' -> sb.append('\'')
                            else -> {} // Unreachable
                        }

                        sb.toString()
                    }

                    if(token.kind == TokenKind.Char &&
                       token.text.codePointCount(0, token.text.length) != 1) {
                        println("Invalid char literal: '${token.text}'")
                    }

                    // Skip the closing quote - again, don't need it in the
                    // token text
                    i++
                    column++
                }

                text[i] == ';' -> {
                    token.kind = TokenKind.Semicolon

                    i++
                    column++
                }

                text[i] == ',' -> {
                    token.kind = TokenKind.Comma

                    i++
                    column++
                }

                text[i] == '(' -> {
                    token.kind = TokenKind.Lparen

                    i++
                    column++
                }

                text[i] == ')' -> {
                    token.kind = TokenKind.Rparen

                    i++
                    column++
                }

                text[i] == '{' -> {
                    token.kind = TokenKind.Lbrace

                    i++
                    column++
                }

                text[i] == '}' -> {
                    token.kind = TokenKind.Rbrace

                    i++
                    column++
                }

                text[i] == '[' -> {
                    token.kind = TokenKind.Lsqbracket

                    i++
                    column++
                }

                text[i] == ']' -> {
                    token.kind = TokenKind.Rsqbracket

                    i++
                    column++
                }

                else -> {
                    println("Unrecognised character \"${text[i]}\"")

                    i++
                    column++

                    continue@lexer
                }
            }

            result.add(token)
        }

        result.add(Token(TokenKind.End, line, column, ""))

        return result
    }

    private fun isIdentStart(c: Char): Boolean = c.isLetter() || c == '_'
    private fun isIdent(c: Char): Boolean = c.isLetterOrDigit() || c == '_'
    private fun isOperator(c: Char): Boolean = c in "!#.%&*+-/:<=>^@\\^|~"

    private fun isHexDigit(c: Char): Boolean {
        return c.isDigit() || c in "ABCDEFabcdef"
    }
    private fun isBinaryDigit(c: Char): Boolean = c in "01"
}
