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
    InvalidDecl,

    TypeNotFound,
    NoType,
    TypeMismatch,
    DuplicateFunctionDeclaration,
    TooManyArguments,
    NotEnoughArguments,
};

struct Error {
    ErrorType type;
    unsigned int line, column;
    unsigned int offset, count;
    std::string message;

    // Don't break the semantic analyser now
    // Remove these when it's rewritten
    Error(ErrorType, AstNode*, std::string) {}
    Error(ErrorType type, unsigned int line, unsigned int column,
            unsigned int offset, unsigned int count, std::string message):
        type(type), line(line), column(column), offset(offset), count(count),
        message(message) {}
};

#endif // SRC_ERROR_H
