//
// Created by myvar on 4/29/18.
//

#ifndef SRC_TOKEN_H
#define SRC_TOKEN_H

#include <string>

#define TokenType_ENUM(name) name
#define TokenType_NAME_ARRAY(name) #name

#define TokenTypes(F)                                                          \
    F(End),                                                                    \
                                                                               \
        F(SingleLineComment), F(MultilineComment),                             \
                                                                               \
        F(StringLiteral), F(IntegerLiteral), F(FloatLiteral), F(Boolean),      \
                                                                               \
        F(OpenParenthesis), F(CloseParenthesis), F(OpenCurlyBracket),          \
        F(CloseCurlyBracket), F(OpenSquareBracket), F(CloseSquareBracket),     \
                                                                               \
        F(Dec), F(Fn), F(Continue), F(Break), F(If), F(Else), F(Return),       \
        F(Struct), F(Get), F(Set), F(Impl), F(At), F(Op), F(Suffix),           \
        F(Prefix), F(Infix), F(Loop), F(In), F(Extern),                        \
                                                                               \
        F(Colon), F(SemiColon), F(Comma), F(Dot), F(Equal), F(CmpEqual),       \
        F(NotEqual), F(GreaterThan), F(GreaterThanEqual), F(LessThan),         \
        F(LessThanEqual), F(CustomOperator),                                   \
                                                                               \
        F(Symbol), F(Unknown),

#define token_type_is_operator(t)                                              \
    (t == TokenType::Equal || t == TokenType::CmpEqual                         \
     || t == TokenType::NotEqual || t == TokenType::GreaterThan                \
     || t == TokenType::GreaterThanEqual || t == TokenType::LessThan           \
     || t == TokenType::LessThanEqual || t == TokenType::CustomOperator        \
     || t == TokenType::Symbol)

enum class TokenType { TokenTypes(TokenType_ENUM) };
static const char *token_type_names[] = {TokenTypes(TokenType_NAME_ARRAY)};

struct Token {
public:
    unsigned int line;
    unsigned int column;
    unsigned int offset;

    std::string raw;

    TokenType type;
};

#endif // SRC_TOKEN_H
