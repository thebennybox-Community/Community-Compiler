#include <iostream>
#include <fstream>
#include "TokenStream.h"

std::string loadTextFromFile(std::string filepath) {
    std::ifstream stream(filepath);
    std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    return str;
}

int main(int argc, char **argv) {
    // argv[1] will contain the file to read
    if (argc < 2) {
        printf("Missing filename in args.\n");
        return 1;
    }

    // Get file using argument as filepath
    std::string fileContents = loadTextFromFile(argv[1]);
    // If the string is empty, the file didn't read properly (or it's empty)
    if (fileContents.empty()) {
        printf("Error reading file: %s\n", argv[1]);
        return 1;
    }

    TokenStream stream;
    stream.lex(fileContents);

    for (Token token : stream.tokens) {
        printf("type: %-18s line: %-4d col: %-3d offset: %-5d raw: \"%s\"\n",
            tokenTypeNames[(int)token.type],
            token.line,
            token.column,
            token.offset,
            token.raw.c_str()
        );
    }

    for (Error error : stream.errors) {
        printf("ErrorType:%d, Type:%d, Raw: %s, Col: %d, Row: %d, Offset: %d\n",
            error.errorType,
            error.token.type,
            error.token.raw.c_str(),
            error.token.column,
            error.token.line,
            error.token.offset
        );
    }

    return 0;
}
