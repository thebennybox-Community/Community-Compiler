package dusk.compiler

enum class TokenKind {
    End,
    None,
    Identifier,
    Number,
    String,
    Char,
    Vararg,

    KeywordBreak,
    KeywordContinue,
    KeywordElse,
    KeywordExtern,
    KeywordFalse,
    KeywordFn,
    KeywordIf,
    KeywordIn,
    KeywordImpl,
    KeywordImport,
    KeywordInfix,
    KeywordLet,
    KeywordLoop,
    KeywordPrefix,
    KeywordReturn,
    KeywordStruct,
    KeywordSuffix,
    KeywordTrue,
    KeywordVar,

    Semicolon,
    Colon,
    Comma,
    Lparen,
    Rparen,
    Lbrace,
    Rbrace,
    Lsqbracket,
    Rsqbracket,
    At,
    Operator,
}

data class Token(
    var kind: TokenKind,
    var line: Int, var column: Int,
    var text: String
)
