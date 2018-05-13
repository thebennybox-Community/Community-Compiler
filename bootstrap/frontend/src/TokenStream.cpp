#include "TokenStream.h"

#include <cstdint>
#include <map>

static const std::map<std::string, TokenType> keywords = {
    {"var",      TokenType::Var},
    {"let",      TokenType::Let},
    {"fn",       TokenType::Fn},
    {"loop",     TokenType::Loop},
    {"in",       TokenType::In},
    {"continue", TokenType::Continue},
    {"break",    TokenType::Break},
    {"if",       TokenType::If},
    {"else",     TokenType::Else},
    {"return",   TokenType::Return},
    {"struct",   TokenType::Struct},
    {"get",      TokenType::Get},
    {"set",      TokenType::Set},
    {"impl",     TokenType::Impl},
    {"op",       TokenType::Op},
    {"suffix",   TokenType::Suffix},
    {"prefix",   TokenType::Prefix},
    {"infix",    TokenType::Infix},
    {"extern",   TokenType::Extern},
    {"true",     TokenType::Boolean},
    {"false",    TokenType::Boolean},
};

static const std::map<std::string, TokenType> operators = {
    {":", TokenType::Colon},
    {"=", TokenType::Equal},
    {"@", TokenType::At},
};

static constexpr const unsigned char SPACE       = 1 << 0;
static constexpr const unsigned char ALPHA       = 1 << 1;
static constexpr const unsigned char IDENT_START = 1 << 2;
static constexpr const unsigned char IDENT       = 1 << 3;
static constexpr const unsigned char DEC         = 1 << 4;
static constexpr const unsigned char HEX         = 1 << 5;
static constexpr const unsigned char OPERATOR    = 1 << 6;

static constexpr const unsigned char char_info[] = {
    /* 0   NUL   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 1   SOH   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 2   STX   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 3   ETX   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 4   EOT   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 5   ENQ   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 6   ACK   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 7   BEL   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 8   BS    */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 9   TAB   */ SPACE | 0     | 0           | 0     | 0   | 0   | 0,
    /* 10  LF    */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 11  VT    */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 12  FF    */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 13  CR    */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 14  SO    */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 15  SI    */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 16  DLE   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 17  DC1   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 18  DC2   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 19  DC3   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 20  DC4   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 21  NAK   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 22  SYN   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 23  ETB   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 24  CAN   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 25  EM    */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 26  SUB   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 27  ESC   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 28  FS    */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 29  GS    */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 30  RS    */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 31  US    */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 32  Space */ SPACE | 0     | 0           | 0     | 0   | 0   | 0,
    /* 33  !     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 34  "     */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 35  #     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 36  $     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 37  %     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 38  &     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 39  '     */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 40  (     */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 41  )     */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 42  *     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 43  +     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 44  ,     */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 45  -     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 46  .     */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 47  /     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 48  0     */ 0     | 0     | 0           | IDENT | DEC | HEX | 0,
    /* 49  1     */ 0     | 0     | 0           | IDENT | DEC | HEX | 0,
    /* 50  2     */ 0     | 0     | 0           | IDENT | DEC | HEX | 0,
    /* 51  3     */ 0     | 0     | 0           | IDENT | DEC | HEX | 0,
    /* 52  4     */ 0     | 0     | 0           | IDENT | DEC | HEX | 0,
    /* 53  5     */ 0     | 0     | 0           | IDENT | DEC | HEX | 0,
    /* 54  6     */ 0     | 0     | 0           | IDENT | DEC | HEX | 0,
    /* 55  7     */ 0     | 0     | 0           | IDENT | DEC | HEX | 0,
    /* 56  8     */ 0     | 0     | 0           | IDENT | DEC | HEX | 0,
    /* 57  9     */ 0     | 0     | 0           | IDENT | DEC | HEX | 0,
    /* 58  :     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 59  ;     */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 60  <     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 61  =     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 62  >     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 63  ?     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 64  @     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 65  A     */ 0     | ALPHA | IDENT_START | IDENT | 0   | HEX | 0,
    /* 66  B     */ 0     | ALPHA | IDENT_START | IDENT | 0   | HEX | 0,
    /* 67  C     */ 0     | ALPHA | IDENT_START | IDENT | 0   | HEX | 0,
    /* 68  D     */ 0     | ALPHA | IDENT_START | IDENT | 0   | HEX | 0,
    /* 69  E     */ 0     | ALPHA | IDENT_START | IDENT | 0   | HEX | 0,
    /* 70  F     */ 0     | ALPHA | IDENT_START | IDENT | 0   | HEX | 0,
    /* 71  G     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 72  H     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 73  I     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 74  J     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 75  K     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 76  L     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 77  M     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 78  N     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 79  O     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 80  P     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 81  Q     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 82  R     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 83  S     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 84  T     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 85  U     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 86  V     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 87  W     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 88  X     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 89  Y     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 90  Z     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 91  [     */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 92  \     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 93  ]     */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 94  ^     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 95  _     */ 0     | 0     | IDENT_START | IDENT | 0   | 0   | 0,
    /* 96  `     */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 97  a     */ 0     | ALPHA | IDENT_START | IDENT | 0   | HEX | 0,
    /* 98  b     */ 0     | ALPHA | IDENT_START | IDENT | 0   | HEX | 0,
    /* 99  c     */ 0     | ALPHA | IDENT_START | IDENT | 0   | HEX | 0,
    /* 100 d     */ 0     | ALPHA | IDENT_START | IDENT | 0   | HEX | 0,
    /* 101 e     */ 0     | ALPHA | IDENT_START | IDENT | 0   | HEX | 0,
    /* 102 f     */ 0     | ALPHA | IDENT_START | IDENT | 0   | HEX | 0,
    /* 103 g     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 104 h     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 105 i     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 106 j     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 107 k     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 108 l     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 109 m     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 110 n     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 111 o     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 112 p     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 113 q     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 114 r     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 115 s     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 116 t     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 117 u     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 118 v     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 119 w     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 120 x     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 121 y     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 122 z     */ 0     | ALPHA | IDENT_START | IDENT | 0   | 0   | 0,
    /* 123 {     */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 124 |     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 125 }     */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
    /* 126 ~     */ 0     | 0     | 0           | 0     | 0   | 0   | OPERATOR,
    /* 127 DEL   */ 0     | 0     | 0           | 0     | 0   | 0   | 0,
};

static inline bool is_digit(char c) {
    return char_info[(unsigned char)c] & DEC;
}

static inline bool is_hex_digit(char c) {
    return char_info[(unsigned char)c] & HEX;
}

static inline bool is_identifier(char c) {
    return char_info[(unsigned char)c] & IDENT;
}

static inline bool is_operator(char c) {
    return char_info[(unsigned char)c] & OPERATOR;
}

void TokenStream::lex(std::string src) {
    for(i = 0; i < src.size(); (void)0) {
        Token token;
        token.line   = line;
        token.column = column;
        token.offset = i;
        token.type   = TokenType::Unknown;

        switch(src[i]) {
        case '\n': {
            i++;
            line++;
            column = 1;

            continue;
        }

        case ' ':
        case '\t': {
            i++, column++;

            continue;
        }

        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
        case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
        case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
        case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
        case 'Y': case 'Z':
        case '_':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
        case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
        case 's': case 't': case 'u': case 'v': case 'w': case 'x':
        case 'y': case 'z': {
            unsigned int start = i;

            while(is_identifier(src[i])) {
                i++, column++;
            }

            unsigned int length = i - start;

            token.type = TokenType::Symbol;
            token.raw  = src.substr(start, length);

            if(keywords.count(token.raw) != 0) {
                token.type = keywords.at(token.raw);
            }

            break;
        }

        case '0': {
            if(src[i + 1] == 'x') {
                token.type = TokenType::HexLiteral;

                unsigned int start = i;

                i += 2; // Skip 0x

                while(is_hex_digit(src[i])) {
                    i++, column++;
                }

                if(src[i] == 'u') {
                    unsigned int start        = i;
                    unsigned int saved_column = column;

                    i++, column++;

                    while(is_digit(src[i])) {
                        i++, column++;
                    }

                    if(!is_identifier(src[i])) {
                        std::string bits =
                            src.substr(start + 1, i - (start + 1));

                        if(
                            bits != "8"  && bits != "16" &&
                            bits != "32" && bits != "64"
                        ) {
                            i      = start; // Invalid suffix, backtrack
                            column = saved_column;
                        }
                    } else {
                        i      = start; // Invalid suffix, backtrack
                        column = saved_column;
                    }
                }

                unsigned int length = i - start;

                token.raw = src.substr(start, length);

                break;
            }

            // Fall through
        }

        case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {
            token.type = TokenType::IntegerLiteral;

            unsigned int start = i;

            while(is_digit(src[i])) {
                i++, column++;
            }

            if(src[i] == '.') {
                // Floating point
                token.type = TokenType::FloatLiteral;

                i++, column++;

                while(is_digit(src[i])) {
                    i++, column++;
                }

                if(src[i] == 'e' || src[i] == 'E') {
                    // Exponent
                    i++, column++;

                    // Check for sign of exponent
                    if(src[i] == '+' || src[i] == '-') {
                        i++, column++;
                    }

                    while(is_digit(src[i])) {
                        i++, column++;
                    }
                }
            }

            if(src[i] == 'f' ||
                    (
                        (src[i] == 'i' || src[i] == 'u') &&
                        token.type == TokenType::IntegerLiteral
                    )
              ) {
                unsigned int start        = i;
                unsigned int saved_column = column;

                i++, column++;

                while(is_digit(src[i])) {
                    i++, column++;
                }

                if(!is_identifier(src[i])) {
                    std::string bits =
                        src.substr(start + 1, i - (start + 1));

                    if(!(((bits == "8" || bits == "16") && src[start] != 'f') ||
                            bits == "32" || bits == "64")
                      ) {
                        i      = start; // Invalid suffix, backtrack
                        column = saved_column;
                    } else if(src[start] == 'f') {
                        token.type = TokenType::FloatLiteral;
                    }
                } else {
                    i      = start; // Invalid suffix, backtrack
                    column = saved_column;
                }
            }

            unsigned int length = i - start;

            token.raw = src.substr(start, length);

            break;
        }

        case ',':
            i++, column++;
            token.type = TokenType::Comma;
            token.raw  = ",";
            break;

        case '.':
            i++, column++;
            token.type = TokenType::Dot;
            token.raw  = ".";
            break;

        case ';':
            i++, column++;
            token.type = TokenType::SemiColon;
            token.raw  = ";";
            break;

        case '(':
            i++, column++;
            token.type = TokenType::OpenParenthesis;
            token.raw  = "(";
            break;

        case ')':
            i++, column++;
            token.type = TokenType::CloseParenthesis;
            token.raw  = ")";
            break;

        case '{':
            i++, column++;
            token.type = TokenType::OpenCurlyBracket;
            token.raw  = "{";
            break;

        case '}':
            i++, column++;
            token.type = TokenType::CloseCurlyBracket;
            token.raw  = "}";
            break;

        case '[':
            i++, column++;
            token.type = TokenType::OpenSquareBracket;
            token.raw  = "[";
            break;

        case ']':
            i++, column++;
            token.type = TokenType::CloseSquareBracket;
            token.raw  = "]";
            break;

        case '/': {
            if(src[i + 1] == '/') {
                i += 2; // Skip //

                unsigned int start = i;

                while(i < src.size() && src[i] != '\n') {
                    i++, column++;
                }

                unsigned int length = i - start;

                token.type = TokenType::SingleLineComment;
                token.raw  = src.substr(start, length);

                break;
            } else if(src[i + 1] == '*') {
                i += 2; // Skip /*

                unsigned int start = i;

                while(
                    i + 1 < src.size() &&
                    !(src[i] == '*' && src[i + 1] == '/')
                ) {
                    if(src[i] == '\n') {
                        line++;
                        column = 1;
                    } else {
                        column++;
                    }

                    i++;
                }

                unsigned int length = i - start;

                i += 2; // Skip */

                token.type = TokenType::MultilineComment;
                token.raw  = src.substr(start, length);

                break;
            }

            // Fall through
        }

        case '!': case '#': case '$': case '%': case '&': case '*':
        case '+': case '-': case ':': case '<': case '=': case '>':
        case '?': case '@': case '~': case '^': case '|': case '\\': {
            unsigned int start = i;

            while(is_operator(src[i])) {
                i++, column++;
            }

            unsigned int length = i - start;

            token.type = TokenType::CustomOperator;
            token.raw  = src.substr(start, length);

            if(operators.count(token.raw) != 0) {
                token.type = operators.at(token.raw);
            }

            break;
        }

        case '"': {
            i++, column++; // Skip opening "

            unsigned int start = i, start_column = column, error_line = line;

            while(i < src.size() && src[i] != '"') {
                if(src[i] == '\\') {
                    i++, column++;
                } else if(src[i] == '\n') {
                    // TODO: Should we stop parsing a string here?
                    error(ErrorType::NewLineInString,
                          line, column, i, "\n",
                          "Unexpected new line in string");

                    i++;
                    line++;
                    column = 1;

                    continue;
                }

                i++, column++;
            }

            unsigned int length = i - start;

            i++, column++; // Skip closing "

            token.type = TokenType::StringLiteral;
            token.raw  = src.substr(start, length);

            for(unsigned int j = 0; j < token.raw.size(); j++) {
                if(token.raw[j] == '\n') {
                    error_line++;
                } else if(token.raw[j] == '\\') {
                    if(token.raw[j + 1] == 'n') {
                        token.raw.replace(j, 2, "\n");
                    } else if(token.raw[j + 1] == 't') {
                        token.raw.replace(j, 2, "\t");
                    } else if(token.raw[j + 1] == '\\') {
                        token.raw.replace(j, 2, "\\");
                    } else if(token.raw[j + 1] == '"') {
                        token.raw.replace(j, 2, "\"");
                    } else {
                        error(ErrorType::InvalidEscapeSequence,
                              error_line, start_column + j, start + j,
                              token.raw.substr(j, 2),
                              "Unexpected character in escape sequence");
                    }
                }
            }

            break;
        }

        default: {
            error(ErrorType::UnrecognisedCharacter,
                  line, column, i, std::string(1, src[i]),
                  "Unrecognised character in input");

            i++, column++;

            continue;
        }
        }

        this->tokens.push_back(std::move(token));
    }

    Token end_token;
    end_token.line   = line;
    end_token.column = column;
    end_token.offset = (unsigned int)src.size();
    end_token.raw    = "";
    end_token.type   = TokenType::End;

    this->tokens.push_back(end_token);
}

void TokenStream::error(
    ErrorType type,
    unsigned int line, unsigned int column, unsigned int offset,
    std::string raw, std::string message
) {
    this->errors.push_back({type, line, column, offset, raw, message});
}
