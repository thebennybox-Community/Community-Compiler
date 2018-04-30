#include <iostream>
#include <fstream>
#include "TokenStream.h"


std::string loadTextFromFile(std::string filepath) {
	std::ifstream stream(filepath);
	std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	return str;
}

int main(int argc, char** argv) {

	// argv[1] will contain the file to read
	if (!argv[1]) {
		printf("Missing filename in args.");
		return 1;
	}

	// Get file using argument as filepath
	std::string fileContents = loadTextFromFile((std::string) argv[1]);
	// If the string is empty, the file didn't read properly (or it's empty)
	if (fileContents.empty()) {
		printf("Error reading file: %s", argv[1]);
		return 1;
	}

	TokenStream stream;
	stream.lex(fileContents);

	for (Token token: stream.tokens) {
		printf("Type:%s, Raw: %s, Col: %d, Row: %d, Offset: %d\n",
			tokenTypeNames[(int) token.tokenType],
			token.raw.c_str(),
			token.column,
			token.row,
			token.offset
		);
    }

    for (Error error: stream.errors) {
		printf("ErrorType:%d, Type:%d, Raw: %s, Col: %d, Row: %d, Offset: %d\n",
			error.errorType,
			error.token.tokenType,
			error.token.raw.c_str(),
			error.token.column,
			error.token.row,
			error.token.offset
		);
    }

    return 0;
}
