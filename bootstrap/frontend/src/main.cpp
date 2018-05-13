#include "AstPrettyPrinter.h"
#include "CodeGen.h"
#include "Parser.h"
#include "TokenStream.h"
#include <fstream>
#include <iostream>
#include <vector>

static Semantics sem;

void error(TokenStream stream, Parser parser) {
    if(stream.errors.size() != 0) {
        for(Token token : stream.tokens) {
            printf(
                "type: %-18s line: %-4u col: %-3u offset: %-5u raw: \"%s\"\n",
                token_type_names[(int)token.type],
                token.line,
                token.column,
                token.offset,
                token.raw.c_str()
            );
        }

        for(Error error : stream.errors) {
            printf(
                "%s: \"%s\" at %u:%u\n",
                error.message.c_str(),
                error.token.raw.c_str(),
                error.token.line,
                error.token.column
            );
        }
    }

    for(Error error : parser.errors) {
        printf(
            "%s: \"%s\" (%s) at %u:%u\n",
            error.message.c_str(),
            error.token.raw.c_str(),
            token_type_names[(int)error.token.type],
            error.token.line,
            error.token.column
        );
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
    std::vector<Ast> asts;

    ILemitter il;

    for(int i = 2; i < argc; i++) {
        std::string file_contents = load_text_from_file(argv[i]);

        TokenStream stream;
        stream.lex(file_contents);
        toks.push_back(stream);

        Parser parser;
        asts.push_back(parser.parse(stream.tokens));

        error(stream, parser);
    }

    for(size_t i = 0; i < asts.size(); i++) {
        sem.pass1(asts[i]);
    }

    for(size_t i = 0; i < asts.size(); i++) {
        sem.pass2(asts[i]);
    }

    for(size_t i = 0; i < asts.size(); i++) {
        sem.pass3(asts[i]);
    }

    scope.clear();
    args.clear();

    while(!scope_stack.empty()) {
        scope_stack.pop();
    }

    while(!arg_stack.empty()) {
        arg_stack.pop();
    }

    for(size_t i = 0; i < asts.size(); i++) {
        generate_il(asts[i].root, il, sem);
    }

    FILE *file = fopen(argv[1], "wb");
    size_t size = il.stream.size();
    fwrite(&il.stream[0], size, 1, file);
    fclose(file);

    for(auto &ast : asts) {
        delete ast.root;
    }

    return 0;
}
