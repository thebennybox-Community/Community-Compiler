#ifndef SRC_ERROR_H
#define SRC_ERROR_H

#include <string>
#include "Token.h"
#include "AstDefs.h"

enum class ErrorType {
    CompilerError,

    UnexpectedCharacter,

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

#endif // SRC_ERROR_H
