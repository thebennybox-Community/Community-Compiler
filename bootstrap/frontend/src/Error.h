//
// Created by myvar on 4/29/18.
//

#ifndef SRC_ERROR_H
#define SRC_ERROR_H

#include "Token.h"

enum class ErrorType {
    UnexpectedCharacter,
    UnexpectedToken,
    InvalidDec,
};

class Error {
public:
    ErrorType error_type;
    Token token;
};

#endif //SRC_ERROR_H
