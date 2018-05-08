#ifndef SRC_TOKENSTREAM_H
#define SRC_TOKENSTREAM_H

#include <string>
#include <vector>
#include "Error.h"

class TokenStream {
public:
    /** The list of tokens created by the lexer */
    std::vector<Token> tokens;

    /** The list of errors generated while lexing */
    std::vector<Error> errors;

    /**
     * Lexes a string into a list of tokens.
     *
     * @param src The source code to lex
     */
    void lex(std::string src);
};

#endif // SRC_TOKENSTREAM_H
