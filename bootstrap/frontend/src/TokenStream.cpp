//
// Created by myvar on 4/29/18.
//

#include "TokenStream.h"

#include <cstdint>
#include <map>

static const std::map<std::string, TokenType> keywords = {
    {"var",      TokenType::Dec},
    {"let",      TokenType::Dec},
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
    {":",  TokenType::Colon},
    {".",  TokenType::Dot},
    {"=",  TokenType::Equal},
    {"@",  TokenType::At},
};

bool is_space(char c) {
    return c == ' ' || c == '\t';
}

bool is_lower(char c) {
    return c >= 'a' && c <= 'z';
}

bool is_upper(char c) {
    return c >= 'A' && c <= 'Z';
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_alpha(char c) {
    return is_lower(c) || is_upper(c);
}

bool is_identifier_start(char c) {
    return is_alpha(c) || c == '_';
}

bool is_identifier(char c) {
    return is_identifier_start(c) || is_digit(c);
}

bool is_operator(char c) {
    return std::string("~!@#$%^&*_+-=|\\/<>?.:").find(c) != std::string::npos;
}

void TokenStream::lex(std::string src) {
    int line = 1, column = 1;

    for(int i = 0; i < src.size(); (void)0) {
        Token token;
        token.line = line;
        token.column = column;
        token.offset = i;
        token.type = TokenType::Unknown;

        if(src[i] == '\n') {
            i++;
            line++;
            column = 1;
            continue;
        } else if(is_space(src[i])) {
            i++; column++;
            continue;
        } else if(is_identifier_start(src[i])) {
            int start = i;
            while(is_identifier(src[i])) {
                i++; column++;
            }
            int length = i - start;

            token.type = TokenType::Symbol;
            token.raw = src.substr(start, length);

            if(keywords.count(token.raw) != 0) {
                token.type = keywords.at(token.raw);
            }

            this->tokens.push_back(std::move(token));
        } else if(is_digit(src[i])) {
            token.type = TokenType::IntegerLiteral;
            int start = i;
            while(is_digit(src[i])) {
                i++; column++;
            }

            if(src[i] == '.') {
                // Floating point
                token.type = TokenType::FloatLiteral;
                i++; column++;
                while(is_digit(src[i])) {
                    i++; column++;
                }

                if(src[i] == 'e' || src[i] == 'E') {
                    // Exponent
                    i++; column++;

                    // Check for sign of exponent
                    if(src[i] == '+' || src[i] == '-') {
                        i++; column++;
                    }

                    while(is_digit(src[i])) {
                        i++; column++;
                    }
                }
            }

            if(
                (
                    (src[i] == 'i' || src[i] == 'u') &&
                    token.type == TokenType::IntegerLiteral
                ) ||
                src[i] == 'f'
            ) {
                int saved_i = i;
                int saved_column = column;
                i++; column++;
                while(is_digit(src[i])) {
                    i++; column++;
                }

                if(!is_identifier(src[i])) {
                    std::string bits =
                        src.substr(saved_i + 1, i - (saved_i + 1));

                    if(!(
                        (bits == "8"  && src[saved_i] != 'f') ||
                        (bits == "16" && src[saved_i] != 'f') ||
                        bits == "32" ||
                        bits == "64"
                    )) {
                        i = saved_i; // Invalid suffix, backtrack
                        column = saved_column;
                    } else if(src[saved_i] == 'f') {
                        token.type = TokenType::FloatLiteral;
                    }
                } else {
                    i = saved_i; // Invalid suffix, backtrack
                    column = saved_column;
                }
            }

            int length = i - start;

            token.raw = src.substr(start, length);

            this->tokens.push_back(std::move(token));
        } else if(src[i] == '"') {
            i++; column++; // Skip opening "

            int start = i;
            while(src[i] != '"') {
                if(src[i] == '\\') {
                    i++; column++;
                } else if(src[i] == '\n') {
                    // TODO: Should we stop parsing a string here?
                    this->errors.push_back({
                        ErrorType::UnexpectedCharacter,
                        {
                            line, column, i,
                            "\n", TokenType::Unknown
                        }
                    });
                    line++;
                    i++;
                    column = 1;
                    continue;
                }

                i++; column++;
            }

            int length = i - start;

            i++; column++; // Skip closing "

            token.type = TokenType::StringLiteral;
            token.raw = src.substr(start, length);

            this->tokens.push_back(std::move(token));
        } else if(src[i] == '/' && src[i + 1] == '/') {
            i += 2; // Skip //
            int start = i;
            while(src[i] != '\n') {
                i++; column++;
            }
            int length = i - start;

            token.type = TokenType::SingleLineComment;
            token.raw = src.substr(start, length);

            this->tokens.push_back(std::move(token));
        } else if(src[i] == '/' && src[i + 1] == '*') {
            i += 2; // Skip /*
            int start = i;
            while(!(src[i] == '*' && src[i + 1] == '/')) {
                if(src[i] == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }
                i++;
            }
            int length = i - start;
            i += 2; // Skip */

            token.type = TokenType::MultilineComment;
            token.raw = src.substr(start, length);

            this->tokens.push_back(std::move(token));
        } else if(is_operator(src[i])) {
            int start = i;
            while(is_operator(src[i])) {
                i++; column++;
            }
            int length = i - start;

            token.type = TokenType::CustomOperator;
            token.raw = src.substr(start, length);

            if(operators.count(token.raw) != 0) {
                token.type = operators.at(token.raw);
            }

            this->tokens.push_back(std::move(token));
        } else if(src[i] == ',') {
            i++; column++;
            token.type = TokenType::Comma;
            token.raw = ",";

            this->tokens.push_back(std::move(token));
        } else if(src[i] == ';') {
            i++; column++;
            token.type = TokenType::SemiColon;
            token.raw = ";";

            this->tokens.push_back(std::move(token));
        } else if(src[i] == '(') {
            i++; column++;
            token.type = TokenType::OpenParenthesis;
            token.raw = "(";

            this->tokens.push_back(std::move(token));
        } else if(src[i] == ')') {
            i++; column++;
            token.type = TokenType::CloseParenthesis;
            token.raw = ")";

            this->tokens.push_back(std::move(token));
        } else if(src[i] == '{') {
            i++; column++;
            token.type = TokenType::OpenCurlyBracket;
            token.raw = "{";

            this->tokens.push_back(std::move(token));
        } else if(src[i] == '}') {
            i++; column++;
            token.type = TokenType::CloseCurlyBracket;
            token.raw = "}";

            this->tokens.push_back(std::move(token));
        } else if(src[i] == '[') {
            i++; column++;
            token.type = TokenType::OpenSquareBracket;
            token.raw = "[";

            this->tokens.push_back(std::move(token));
        } else if(src[i] == ']') {
            i++; column++;
            token.type = TokenType::CloseSquareBracket;
            token.raw = "]";

            this->tokens.push_back(std::move(token));
        } else {
            this->errors.push_back({
                ErrorType::UnexpectedCharacter,
                {line, column, i, std::string(1, src[i]), TokenType::Unknown}
            });

            i++; column++;
        }
    }

    Token end_token;
    end_token.line   = 0;
    end_token.column = 0;
    end_token.offset = 0;
    end_token.raw    = "";
    end_token.type = TokenType::End;

    this->tokens.push_back(end_token);
}
