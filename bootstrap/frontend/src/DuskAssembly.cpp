#include "DuskAssembly.h"

#include <fstream>
#include "TokenStream.h"
#include "Parser.h"
#include "Terminal.h"
#include "AstPrettyPrinter.h"

static std::string read_file(std::string filename) {
    std::ifstream stream(filename);
    std::string str(
        (std::istreambuf_iterator<char>(stream)),
        std::istreambuf_iterator<char>());
    return str;
}

ScopeContext *DuskAssembly::find_scope(AstNamespace *ns) {
    for(auto scope : this->scopes) {
        if(scope->ns->name == ns->name) {
            return scope;
        }
    }

    return nullptr;
}

void DuskAssembly::queue_file(std::string filename) {
    this->queued_files.push_back(filename);
}

void DuskAssembly::compile_write_binary(std::string out_file) {

}

Ast DuskAssembly::parse_file(std::string filename) {
    Ast ast;
    ast.file_contents = read_file(filename);

    TokenStream token_stream;
    token_stream.lex(ast.file_contents);
    ast.tokens = token_stream.tokens;

    if(handle_errors(token_stream.errors, ast.file_contents, ast.tokens)) {
        Parser parser;
        Ast tmp = parser.parse(token_stream.tokens);

        // Discard result of first pass
        delete tmp.root;

        if(handle_errors(parser.errors, ast.file_contents, ast.tokens)) {
            Ast tmp = parser.parse(token_stream.tokens);
            ast.root = tmp.root;
            ast.tokens = token_stream.tokens;
        }
    }

    return ast;
}

bool DuskAssembly::semantic_analysis(Ast &ast) {
    return semantic_analyse_node(ast.root);
}

bool DuskAssembly::semantic_analyse_node(AstNode *node) {
    switch(node->node_type) {
    case AstNodeType::AstBlock:

        break;

    case AstNodeType::AstString:

        break;

    case AstNodeType::AstNumber:

        break;

    case AstNodeType::AstBoolean:

        break;

    case AstNodeType::AstArray:

        break;
    case AstNodeType::AstDec:

        break;
    case AstNodeType::AstIf:

        break;
    case AstNodeType::AstFn:

        break;
    case AstNodeType::AstFnCall:

        break;
    case AstNodeType::AstLoop:

        break;
    case AstNodeType::AstContinue:

        break;
    case AstNodeType::AstBreak:

        break;
    case AstNodeType::AstStruct:

        break;
    case AstNodeType::AstImpl:

        break;
    case AstNodeType::AstAttribute:

        break;
    case AstNodeType::AstAffix:

        break;
    case AstNodeType::AstUnaryExpr:

        break;
    case AstNodeType::AstBinaryExpr:

        break;
    case AstNodeType::AstIndex:

        break;
    case AstNodeType::AstType:

        break;
    case AstNodeType::AstSymbol:

        break;
    case AstNodeType::AstReturn:

        break;
    case AstNodeType::AstExtern:

        break;
    case AstNodeType::AstUse:

        break;
    case AstNodeType::AstNamespace:

        break;
    }
}

bool DuskAssembly::generate_code(Ast &ast) {
    return generate_code_node(ast.root);
}

bool DuskAssembly::generate_code_node(AstNode *node) {

}

bool DuskAssembly::handle_errors(
    std::vector<Error> errors,
    std::string file_contents, std::vector<Token> tokens
) {
    for(Error error : errors) {
        printf(
            "\n%s%s @ %s%s%d%s:%s%d%s\n",
            term_fg[TermColour::Yellow],
            error.message.c_str(),
            term_reset,
            term_fg[TermColour::Blue], error.line, term_reset,
            term_fg[TermColour::Blue], error.column, term_reset
        );
        syntax_highlight_print_error(
            file_contents, tokens,
            error.line, error.offset, error.count);
    }
}

