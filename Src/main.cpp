#include <iostream>
#include <fstream>
#include "TokenStream.h"


std::string loadTextFromFile(std::string filepath) {
    std::ifstream stream(filepath);
    std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    return str;
}

int main() {

    TokenStream stream;
    stream.lex(loadTextFromFile("test.ds"));

    for (Token token: stream.tokens) {
        printf("Type:%s, Raw: %s, Col: %d, Row: %d, Offset: %d\n", tokenTypeNames[(int)token.tokenType], token.raw.c_str(), token.column,
               token.row, token.offset);
    }

    for (Error error: stream.errors) {

       printf("ErrorType:%d, Type:%d, Raw: %s, Col: %d, Row: %d, Offset: %d\n",
               error.errorType, error.token.tokenType, error.token.raw.c_str(),
               error.token.column, error.token.row, error.token.offset);
    }

    return 0;
}
