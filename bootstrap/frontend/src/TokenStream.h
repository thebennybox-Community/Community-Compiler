//
// Created by myvar on 4/29/18.
//

#ifndef SRC_TOKENSTREAM_H
#define SRC_TOKENSTREAM_H

#include <string>
#include <vector>
#include "Error.h"

class TokenStream {
public:
    std::vector<Error> errors;
    std::vector<Token> tokens;

    void lex(std::string src);
};


#endif //SRC_TOKENSTREAM_H
