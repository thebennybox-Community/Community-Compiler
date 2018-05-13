#include <fstream>
#include <iostream>
#include <vector>
#include "AstPrettyPrinter.h"
#include "CodeGen.h"
#include "Parser.h"
#include "TokenStream.h"
#include "Terminal.h"

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

    bool errors_occurred = false;

    for(int i = 2; i < argc; i++) {
        std::string file_contents = load_text_from_file(argv[i]);

        TokenStream stream;
        stream.lex(file_contents);
        toks.push_back(stream);

        if(!stream.errors.empty()) {
            errors_occurred = true;
            for(LexerError error : stream.errors) {
                printf("\n%s%s @ %s%s%d%s:%s%d%s\n",
                       term_fg[TermColour::Yellow],
                       error.to_string().c_str(),
                       term_reset,
                       term_fg[TermColour::Blue], error.line, term_reset,
                       term_fg[TermColour::Blue], error.column, term_reset);
                syntax_highlight_print_line(
                    file_contents, stream, error.offset, error.raw.size());
            }
        } else {
            Parser parser;
            asts.push_back(parser.parse(stream.tokens));

            if(!parser.errors.empty()) {
                errors_occurred = true;
                for(Error error : parser.errors) {
                    printf("\n-----------------------------\n\n");
                    if(error.type == ErrorType::UnexpectedToken) {
                        printf(
                            "%s, got \"%s\" (%s) at %u:%u\n",
                            error.message.c_str(),
                            error.token.raw.c_str(),
                            token_type_names[(int)error.token.type],
                            error.token.line,
                            error.token.column
                        );
                    } else {
                        printf(
                            "%s: \"%s\" (%s) at %u:%u\n",
                            error.message.c_str(),
                            error.token.raw.c_str(),
                            token_type_names[(int)error.token.type],
                            error.token.line,
                            error.token.column
                        );
                    }
                    syntax_highlight_print_line(
                        file_contents, stream, error.token.offset, error.token.raw.size());
                }
            }
        }
    }

    if(errors_occurred) {
        for(auto &ast : asts) {
            delete ast.root;
        }

        printf("\n------------------------\nErrors occurred, exiting\n");
        return 1;
    }

    Semantics sem;

    for(size_t i = 0; i < asts.size(); i++) {
        sem.pass1(asts[i]);
    }

    for(size_t i = 0; i < asts.size(); i++) {
        sem.pass2(asts[i]);
    }

    for(size_t i = 0; i < asts.size(); i++) {
        sem.pass3(asts[i]);
    }

    if(!sem.errors.empty()) {
        for(Error error : sem.errors) {
            printf("%s\n", error.message.c_str());
        }
        return 1;
    }

    scope.clear();
    args.clear();

    while(!scope_stack.empty()) {
        scope_stack.pop();
    }

    while(!arg_stack.empty()) {
        arg_stack.pop();
    }

    ILemitter il;

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
