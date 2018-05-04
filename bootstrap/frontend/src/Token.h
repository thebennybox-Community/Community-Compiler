//
// Created by myvar on 4/29/18.
//

#ifndef SRC_TOKEN_H
#define SRC_TOKEN_H

#include <string>

#define TokenType_ENUM(name) name
#define TokenType_NAME_ARRAY(name) #name

#define TokenTypes(F) \
    F(SingleLineComment), \
    F(MultilineComment), \
    \
    F(StringLiteral), \
    F(NumberLiteral), \
    F(Boolean), \
    \
    F(OpenParenthesis), \
    F(CloseParenthesis), \
    F(OpenCurlyBracket), \
    F(CloseCurlyBracket), \
    F(OpenSquareBracket), \
    F(CloseSquareBracket), \
    \
    F(Var), \
    F(Let), \
    F(Fn), \
    F(Continue), \
    F(Break), \
    F(If), \
    F(Else), \
    F(Return), \
    F(Struct), \
    F(Get), \
    F(Set), \
    F(Impl), \
    F(At), \
    F(Op), \
    F(Suffix), \
    F(Prefix), \
    F(Infix), \
    F(Loop), \
    F(In), \
    \
    F(Colon), \
    F(SemiColon), \
    F(Comma), \
    F(Dot), \
    F(Equal), \
    F(Plus), \
    F(Minus), \
    F(Mul), \
    F(Divide), \
    F(Mod), \
    F(CmpEqual), \
    F(NotEqual), \
    F(GreaterThan), \
    F(GreaterThanEqual), \
    F(LessThan), \
    F(LessThanEqual), \
    F(Not), \
    F(And), \
    F(Or), \
    F(Xor), \
    F(BitNot), \
    F(BitAnd), \
    F(BitOr), \
    F(BitXor), \
    F(CustomOperator), \
    \
    F(Symbol), \
    F(Unknown), \

enum class TokenType {
    TokenTypes(TokenType_ENUM)
};
static const char *tokenTypeNames[] = {
    TokenTypes(TokenType_NAME_ARRAY)
};

struct Token {
public:
    int line;
    int column;
    int offset;

    std::string raw;

    TokenType type;
};


#endif //SRC_TOKEN_H
