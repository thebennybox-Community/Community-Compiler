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
    std::vector<LexerError> errors;

    /**
     * Lexes a string into a list of tokens.
     *
     * @param src The source code to lex
     */
    void lex(std::string src);

private:
    /** The current line number of the lexer */
    unsigned int line = 1;

    /** The current column number of the lexer */
    unsigned int column = 1;

    /** The offset of the current character */
    unsigned int i = 0;

    /**
     * Writes an error with a given type and message to the error list.
     *
     * @param type    The type of error to add
     * @param raw     The raw text of the token that caused the error
     * @param message The error message
     */
    void error(
        ErrorType type,
        unsigned int line, unsigned int column, unsigned int offset,
        std::string raw, std::string message
    );
};

#endif // SRC_TOKENSTREAM_H
