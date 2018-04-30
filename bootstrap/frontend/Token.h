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
    F(OpenCurlyBracket), \
    F(CloseCurlyBracket), \
    F(StringLiteral), \
    F(NumberLiteral), \
    F(Symbol), \
    F(Boolean), \
    F(Dec), \
    F(Fn), \
    F(Colon), \
    F(SemiColon), \
    F(Comma), \
    F(Dot), \
    F(Equal), \
    F(Plus), \
    F(Minus), \
    F(Divide), \
    F(Mul), \
    F(OpenBracket), \
    F(CloseBracket), \
    F(OpenSquareBracket), \
    F(CloseSquareBracket), \
    F(Loop), \
    F(In), \
    F(CmpEqual), \
    F(GreaterThan), \
    F(LessThan), \
    F(Not), \
    F(NotEqual), \
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
    F(Unknown), \

enum class TokenType {
TokenTypes(TokenType_ENUM)
};
static const char *tokenTypeNames[] = {
        TokenTypes(TokenType_NAME_ARRAY)
        };

        class Token {

            public:
            int column;
            int row;
            int offset;

            std::string raw;

            TokenType tokenType;

        };


#endif //SRC_TOKEN_H
