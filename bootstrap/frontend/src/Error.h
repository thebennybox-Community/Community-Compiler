#ifndef SRC_ERROR_H
#define SRC_ERROR_H

#include <string>
#include "Token.h"

enum class ErrorType {
    UnexpectedCharacter,

    UnexpectedToken,
    InvalidDec,
};

class Error {
public:
    ErrorType   type;
    Token       token;
    std::string message;
};

#endif // SRC_ERROR_H
