#include "AstPrettyPrinter.h"
#include "CodeGen.h"
#include "Parser.h"
#include "TokenStream.h"
#include <fstream>
#include <iostream>
#include <vector>

void error(TokenStream stream, Parser parser) {
    if(stream.errors.size() != 0) {
        for(Token token : stream.tokens) {
            printf(
                "type: %-18s line: %-4u col: %-3u offset: %-5u raw: \"%s\"\n",
                token_type_names[(int)token.type],
                token.line,
                token.column,
                token.offset,
                token.raw.c_str());
        }

        for(Error error : stream.errors) {
            printf(
                "Error type: %-4d "
                "type: %-18s line: %-4u col: %-3u offset: %-5u raw: \"%s\"\n",
                (int)error.error_type,
                token_type_names[(int)error.token.type],
                error.token.line,
                error.token.column,
                error.token.offset,
                error.token.raw.c_str());
        }
    }

    for(Error error : parser.errors) {
        printf(
            "Error type: %-4d "
            "type: %-18s line: %-4d col: %-3d offset: %-5d raw: \"%s\"\n",
            (int)error.error_type,
            token_type_names[(int)error.token.type],
            error.token.line,
            error.token.column,
            error.token.offset,
            error.token.raw.c_str());
    }
}

std::string load_text_from_file(std::string filepath) {
    std::ifstream stream(filepath);
    std::string str(
        (std::istreambuf_iterator<char>(stream)),
        std::istreambuf_iterator<char>());
    return str;
}

int main(int argc, char **argv) {
    if(argc < 3) {
        printf("Missing filename in args.\n");
        return 1;
    }

    std::vector<TokenStream> toks;
    std::vector<Ast *> asts;

    ILemitter il;

    CodeGen gen;

    for(int i = 2; i < argc; i++) {

        std::string file_contents = load_text_from_file(argv[i]);
        if(file_contents.empty()) {
            // printf("Error reading file: %s\n", argv[i]);
            // return 1;
        }

        // printf("%s\n", argv[i]);

        TokenStream stream;
        stream.lex(file_contents);
        toks.push_back(stream);

        Parser parser;
        asts.push_back(parser.parse(stream.tokens));

        error(stream, parser);

        // syntax_highlight_print(file_contents, stream);
    }

    for(auto a : asts) {
        gen.sem.pass1(*a);
    }

    for(auto a : asts) {
        gen.sem.pass2(*a);
    }

    for(auto a : asts) {
        gen.sem.pass3(*a);
    }

    for(auto a : asts) {
        gen.generateIL(a->root, il);
        // pretty_print_ast(*a);
    }

    auto o     = argv[1];
    FILE *file = fopen(o, "wb");
    if(file == NULL) {
        // Some error thing here
    }

    auto x = il.stream.size();
    fwrite(&il.stream[0], x, 1, file);
    fclose(file);

    for(auto a : asts) {
        delete a;
    }

    return 0;
}
