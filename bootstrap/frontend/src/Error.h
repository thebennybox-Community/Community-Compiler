#ifndef SRC_ERROR_H
#define SRC_ERROR_H

#include <string>
#include "Token.h"
#include "AstDefs.h"

enum class ErrorType {
    CompilerError,

    UnrecognisedCharacter,
    NewLineInString,
    InvalidEscapeSequence,

    UnexpectedToken,
    InvalidDec,

    TypeNotFound,
    NoType,
    TypeMismatch,
    DuplicateFunctionDeclaration,
    TooManyArguments,
    NotEnoughArguments,
};

class Error {
public:
    Error(ErrorType type, Token token, std::string message):
        type(type), token(token), message(message) {}

    Error(ErrorType type, const AstNode* node, std::string message):
        type(type), message(message), node(node) {}

    ErrorType   type;
    Token       token;
    std::string message;
    const AstNode *node;
};

struct LexerError {
    ErrorType type;
    unsigned int line, column;
    unsigned int offset;
    std::string raw;
    std::string message;

    std::string to_string() {
        return message;
    }
};

#endif // SRC_ERROR_H
